// SPDX-License-Identifier: GPL-2.0
/* Copyright(c) 2016-2018 Intel Corporation. All rights reserved. */
#include "asm/pgtable_64_types.h"
#include "asm/pgtable_types.h"
#include "linux/compiler_attributes.h"
#include "linux/export.h"
#include "linux/kern_levels.h"
#include "linux/mm_types.h"
#include "linux/types.h"
#include "vdso/bits.h"
#include <linux/memremap.h>
#include <linux/pagemap.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/pfn_t.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/dax.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/mman.h>
#include <linux/debugfs.h>
#include <linux/fs.h>
#include <asm/pgalloc.h>
#include "dax-private.h"
#include "bus.h"

#ifdef MPFS

#include <asm/pkeys.h>
long dax_ioctl(struct file *filp, unsigned int type, unsigned long ptr);
static vm_fault_t mpfs_huge_fault(struct vm_fault *vmf, pgd_t **pgd_pp);
static pmd_t mpfs_dax_dump_pmd(struct task_struct *task, struct vm_fault *vmf);
volatile size_t debugfs_PM_PT_index = 0; // decide which huge page to dump
volatile size_t debugfs_DRAM_PT_index = 0; // decide which huge page to dump
volatile pid_t debugfs_Any_DRAM_PT_pid = 0; // decide which process to dump
void PM_page_table_to_DEBUGFS(void);
void DRAM_page_table_to_DEBUGFS(void);
void Any_DRAM_page_table_to_DEBUGFS(struct task_struct *task);

unsigned long dax_pswap_npgs[10] = {
	1, 8, 64, 512, 4096, 32768, 262144, 2097152, 16777216, 134217728
};

#endif
static dax_runtime_t *mpfs_kernel_init(dax_master_page_t *master_page);

static int check_vma(struct dev_dax *dev_dax, struct vm_area_struct *vma,
		     const char *func)
{
	struct dax_region *dax_region = dev_dax->region;
	struct device *dev = &dev_dax->dev;
	unsigned long mask;

	if (!dax_alive(dev_dax->dax_dev))
		return -ENXIO;

	/* prevent private mappings from being established */
	if ((vma->vm_flags & VM_MAYSHARE) != VM_MAYSHARE) {
		dev_info_ratelimited(
			dev, "%s: %s: fail, attempted private mapping\n",
			current->comm, func);
		return -EINVAL;
	}

	mask = dax_region->align - 1;
	if (vma->vm_start & mask || vma->vm_end & mask) {
		dev_info_ratelimited(
			dev,
			"%s: %s: fail, unaligned vma (%#lx - %#lx, %#lx)\n",
			current->comm, func, vma->vm_start, vma->vm_end, mask);
		return -EINVAL;
	}

	if ((dax_region->pfn_flags & (PFN_DEV | PFN_MAP)) == PFN_DEV &&
	    (vma->vm_flags & VM_DONTCOPY) == 0) {
		dev_info_ratelimited(
			dev, "%s: %s: fail, dax range requires MADV_DONTFORK\n",
			current->comm, func);
		return -EINVAL;
	}

	if (!vma_is_dax(vma)) {
		dev_info_ratelimited(dev,
				     "%s: %s: fail, vma is not DAX capable\n",
				     current->comm, func);
		return -EINVAL;
	}

	return 0;
}

/* see "strong" declaration in tools/testing/nvdimm/dax-dev.c */
__weak phys_addr_t dax_pgoff_to_phys(struct dev_dax *dev_dax, pgoff_t pgoff,
				     unsigned long size)
{
	struct resource *res = &dev_dax->region->res;
	phys_addr_t phys;
	//res->start=49058676736
	//res->end=366143864831
	//res->size=317,085,188,095<=>295.31GB
	phys = pgoff * PAGE_SIZE + res->start;
	if (phys >= res->start && phys <= res->end) {
		if (phys + size - 1 <= res->end)
			return phys;
	}

	return -1;
}

static vm_fault_t __dev_dax_pte_fault(struct dev_dax *dev_dax,
				      struct vm_fault *vmf, pfn_t *pfn)
{
	struct device *dev = &dev_dax->dev;
	struct dax_region *dax_region;
	phys_addr_t phys;
	unsigned int fault_size = PAGE_SIZE;

	if (check_vma(dev_dax, vmf->vma, __func__))
		return VM_FAULT_SIGBUS;

	dax_region = dev_dax->region;
	if (dax_region->align > PAGE_SIZE) {
		dev_dbg(dev, "alignment (%#x) > fault size (%#x)\n",
			dax_region->align, fault_size);
		return VM_FAULT_SIGBUS;
	}

	if (fault_size != dax_region->align)
		return VM_FAULT_SIGBUS;

	phys = dax_pgoff_to_phys(dev_dax, vmf->pgoff, PAGE_SIZE);
	if (phys == -1) {
		dev_dbg(dev, "pgoff_to_phys(%#lx) failed\n", vmf->pgoff);
		return VM_FAULT_SIGBUS;
	}

	*pfn = phys_to_pfn_t(phys, dax_region->pfn_flags);

	return vmf_insert_mixed(vmf->vma, vmf->address, *pfn);
}

static vm_fault_t __dev_dax_pmd_fault(struct dev_dax *dev_dax,
				      struct vm_fault *vmf, pfn_t *pfn)
{
	unsigned long pmd_addr = vmf->address & PMD_MASK;
	struct device *dev = &dev_dax->dev;
	struct dax_region *dax_region;
	phys_addr_t phys;
	pgoff_t pgoff;
	unsigned int fault_size = PMD_SIZE;

	if (check_vma(dev_dax, vmf->vma, __func__))
		return VM_FAULT_SIGBUS;

	dax_region = dev_dax->region;
	if (dax_region->align > PMD_SIZE) {
		dev_dbg(dev, "alignment (%#x) > fault size (%#x)\n",
			dax_region->align, fault_size);
		return VM_FAULT_SIGBUS;
	}

	/* dax pmd mappings require pfn_t_devmap() */
	if ((dax_region->pfn_flags & (PFN_DEV | PFN_MAP)) !=
	    (PFN_DEV | PFN_MAP)) {
		dev_dbg(dev, "region lacks devmap flags\n");
		return VM_FAULT_SIGBUS;
	}

	if (fault_size < dax_region->align)
		return VM_FAULT_SIGBUS;
	else if (fault_size > dax_region->align)
		return VM_FAULT_FALLBACK;

	/* if we are outside of the VMA */
	if (pmd_addr < vmf->vma->vm_start ||
	    (pmd_addr + PMD_SIZE) > vmf->vma->vm_end)
		return VM_FAULT_SIGBUS;

	pgoff = linear_page_index(vmf->vma, pmd_addr);
	phys = dax_pgoff_to_phys(dev_dax, pgoff, PMD_SIZE);
	if (phys == -1) {
		dev_dbg(dev, "pgoff_to_phys(%#lx) failed\n", pgoff);
		return VM_FAULT_SIGBUS;
	}

	*pfn = phys_to_pfn_t(phys, dax_region->pfn_flags);

	return vmf_insert_pfn_pmd(vmf, *pfn, vmf->flags & FAULT_FLAG_WRITE);
}

#ifdef CONFIG_HAVE_ARCH_TRANSPARENT_HUGEPAGE_PUD
static vm_fault_t __dev_dax_pud_fault(struct dev_dax *dev_dax,
				      struct vm_fault *vmf, pfn_t *pfn)
{
	unsigned long pud_addr = vmf->address & PUD_MASK;
	struct device *dev = &dev_dax->dev;
	struct dax_region *dax_region;
	phys_addr_t phys;
	pgoff_t pgoff;
	unsigned int fault_size = PUD_SIZE;

	if (check_vma(dev_dax, vmf->vma, __func__))
		return VM_FAULT_SIGBUS;

	dax_region = dev_dax->region;
	if (dax_region->align > PUD_SIZE) {
		dev_dbg(dev, "alignment (%#x) > fault size (%#x)\n",
			dax_region->align, fault_size);
		return VM_FAULT_SIGBUS;
	}

	/* dax pud mappings require pfn_t_devmap() */
	if ((dax_region->pfn_flags & (PFN_DEV | PFN_MAP)) !=
	    (PFN_DEV | PFN_MAP)) {
		dev_dbg(dev, "region lacks devmap flags\n");
		return VM_FAULT_SIGBUS;
	}

	if (fault_size < dax_region->align)
		return VM_FAULT_SIGBUS;
	else if (fault_size > dax_region->align)
		return VM_FAULT_FALLBACK;

	/* if we are outside of the VMA */
	if (pud_addr < vmf->vma->vm_start ||
	    (pud_addr + PUD_SIZE) > vmf->vma->vm_end)
		return VM_FAULT_SIGBUS;

	pgoff = linear_page_index(vmf->vma, pud_addr);
	phys = dax_pgoff_to_phys(dev_dax, pgoff, PUD_SIZE);
	if (phys == -1) {
		dev_dbg(dev, "pgoff_to_phys(%#lx) failed\n", pgoff);
		return VM_FAULT_SIGBUS;
	}

	*pfn = phys_to_pfn_t(phys, dax_region->pfn_flags);

	return vmf_insert_pfn_pud(vmf, *pfn, vmf->flags & FAULT_FLAG_WRITE);
}
#else
static vm_fault_t __dev_dax_pud_fault(struct dev_dax *dev_dax,
				      struct vm_fault *vmf, pfn_t *pfn)
{
	return VM_FAULT_FALLBACK;
}
#endif /* !CONFIG_HAVE_ARCH_TRANSPARENT_HUGEPAGE_PUD */

static vm_fault_t dev_dax_huge_fault(struct vm_fault *vmf,
				     enum page_entry_size pe_size)
{
	struct file *filp = vmf->vma->vm_file;
	unsigned long fault_size;
	vm_fault_t rc = VM_FAULT_SIGBUS;
	int id;
	pfn_t pfn;
	struct dev_dax *dev_dax = filp->private_data;

	dev_dbg(&dev_dax->dev, "%s: %s (%#lx - %#lx) size = %d\n",
		current->comm,
		(vmf->flags & FAULT_FLAG_WRITE) ? "write" : "read",
		vmf->vma->vm_start, vmf->vma->vm_end, pe_size);

	id = dax_read_lock();
	switch (pe_size) {
	case PE_SIZE_PTE:
		fault_size = PAGE_SIZE;
		rc = __dev_dax_pte_fault(dev_dax, vmf, &pfn);
		break;
	case PE_SIZE_PMD:
		fault_size = PMD_SIZE;
		rc = __dev_dax_pmd_fault(dev_dax, vmf, &pfn);
		break;
	case PE_SIZE_PUD:
		fault_size = PUD_SIZE;
		rc = __dev_dax_pud_fault(dev_dax, vmf, &pfn);
		break;
	default:
		rc = VM_FAULT_SIGBUS;
	}

	if (rc == VM_FAULT_NOPAGE) {
		unsigned long i;
		pgoff_t pgoff;

		/*
		 * In the device-dax case the only possibility for a
		 * VM_FAULT_NOPAGE result is when device-dax capacity is
		 * mapped. No need to consider the zero page, or racing
		 * conflicting mappings.
		 */
		pgoff = linear_page_index(vmf->vma,
					  vmf->address & ~(fault_size - 1));
		for (i = 0; i < fault_size / PAGE_SIZE; i++) {
			struct page *page;

			page = pfn_to_page(pfn_t_to_pfn(pfn) + i);
			if (page->mapping)
				continue;
			page->mapping = filp->f_mapping;
			page->index = pgoff + i;
		}
	}
	dax_read_unlock(id);

	return rc;
}

static vm_fault_t dev_dax_fault(struct vm_fault *vmf)
{
	return dev_dax_huge_fault(vmf, PE_SIZE_PTE);
}

static int dev_dax_split(struct vm_area_struct *vma, unsigned long addr)
{
	struct file *filp = vma->vm_file;
	struct dev_dax *dev_dax = filp->private_data;
	struct dax_region *dax_region = dev_dax->region;

	if (!IS_ALIGNED(addr, dax_region->align))
		return -EINVAL;
	return 0;
}

static unsigned long dev_dax_pagesize(struct vm_area_struct *vma)
{
	struct file *filp = vma->vm_file;
	struct dev_dax *dev_dax = filp->private_data;
	struct dax_region *dax_region = dev_dax->region;

	return dax_region->align;
}

static const struct vm_operations_struct dax_vm_ops = {
	.fault = dev_dax_fault,
	.huge_fault = dev_dax_huge_fault,
	.split = dev_dax_split,
	.pagesize = dev_dax_pagesize,
	.UFS_huge_fault = mpfs_huge_fault,
	.UFS_enable = MPFS_UFS_ENABLED,
};

static int dax_mmap(struct file *filp, struct vm_area_struct *vma)
{
	struct dev_dax *dev_dax = filp->private_data;
	int rc, id;

	dev_dbg(&dev_dax->dev, "trace\n");

	/*
	 * We lock to check dax_dev liveness and will re-check at
	 * fault time.
	 */
	id = dax_read_lock();
	rc = check_vma(dev_dax, vma, __func__);
	dax_read_unlock(id);
	if (rc)
		return rc;

	vma->vm_ops = &dax_vm_ops;
	vma->vm_flags |= VM_HUGEPAGE;
	return 0;
}

/* return an unmapped area aligned to the dax region specified alignment */
static unsigned long
dax_get_unmapped_area(struct file *filp, unsigned long addr, unsigned long len,
		      unsigned long pgoff, unsigned long flags)
{
	unsigned long off, off_end, off_align, len_align, addr_align, align;
	struct dev_dax *dev_dax = filp ? filp->private_data : NULL;
	struct dax_region *dax_region;

	if (!dev_dax || addr)
		goto out;

	dax_region = dev_dax->region;
	align = dax_region->align;

//!MPFS: the align is 0x8000000000, which is 512GB
#ifdef MPFS
	if (flags & MAP_PGD_ALIGNED)
		align = PGD_ALIGNED;
#endif

	off = pgoff << PAGE_SHIFT;
	off_end = off + len;
	off_align = round_up(off, align);

	if ((off_end <= off_align) || ((off_end - off_align) < align))
		goto out;

	len_align = len + align;
	if ((off + len_align) < off)
		goto out;

	addr_align = current->mm->get_unmapped_area(filp, addr, len_align,
						    pgoff, flags);
	if (!IS_ERR_VALUE(addr_align)) {
		addr_align += (off - addr_align) & (align - 1);
		return addr_align;
	}
out:
	return current->mm->get_unmapped_area(filp, addr, len, pgoff, flags);
}

static const struct address_space_operations dev_dax_aops = {
	.set_page_dirty = noop_set_page_dirty,
	.invalidatepage = noop_invalidatepage,
};

static int dax_open(struct inode *inode, struct file *filp)
{
	struct dax_device *dax_dev = inode_dax(inode);
	struct inode *__dax_inode = dax_inode(dax_dev);
	struct dev_dax *dev_dax = dax_get_private(dax_dev);

	dev_dbg(&dev_dax->dev, "trace\n");
	inode->i_mapping = __dax_inode->i_mapping;
	inode->i_mapping->host = __dax_inode;
	inode->i_mapping->a_ops = &dev_dax_aops;
	filp->f_mapping = inode->i_mapping;
	filp->f_wb_err = filemap_sample_wb_err(filp->f_mapping);
	filp->private_data = dev_dax;
	inode->i_flags = S_DAX;

	return 0;
}

static int dax_release(struct inode *inode, struct file *filp)
{
	struct dev_dax *dev_dax = filp->private_data;

	dev_dbg(&dev_dax->dev, "trace\n");
	return 0;
}

static const struct file_operations dax_fops = {
	.llseek = noop_llseek,
	.owner = THIS_MODULE,
	.open = dax_open,
	.release = dax_release,
	.get_unmapped_area = dax_get_unmapped_area,
	.mmap = dax_mmap,
	.mmap_supported_flags = MAP_SYNC,
#ifdef MPFS
	.unlocked_ioctl = dax_ioctl,
#endif
};

static void dev_dax_cdev_del(void *cdev)
{
	cdev_del(cdev);
}

static void dev_dax_kill(void *dev_dax)
{
	kill_dev_dax(dev_dax);
}

int dev_dax_probe(struct device *dev)
{
	struct dev_dax *dev_dax = to_dev_dax(dev);
	struct dax_device *dax_dev = dev_dax->dax_dev;
	struct resource *res = &dev_dax->region->res;
	struct inode *inode;
	struct cdev *cdev;
	void *addr;
	int rc;

	/* 1:1 map region resource range to device-dax instance range */
	if (!devm_request_mem_region(dev, res->start, resource_size(res),
				     dev_name(dev))) {
		dev_warn(dev, "could not reserve region %pR\n", res);
		return -EBUSY;
	}

	dev_dax->pgmap.type = MEMORY_DEVICE_DEVDAX;
	addr = devm_memremap_pages(dev, &dev_dax->pgmap);
	if (IS_ERR(addr))
		return PTR_ERR(addr);

	inode = dax_inode(dax_dev);
	cdev = inode->i_cdev;
	cdev_init(cdev, &dax_fops);
	if (dev->class) {
		/* for the CONFIG_DEV_DAX_PMEM_COMPAT case */
		cdev->owner = dev->parent->driver->owner;
	} else
		cdev->owner = dev->driver->owner;
	cdev_set_parent(cdev, &dev->kobj);
	rc = cdev_add(cdev, dev->devt, 1);
	if (rc)
		return rc;

	rc = devm_add_action_or_reset(dev, dev_dax_cdev_del, cdev);
	if (rc)
		return rc;

	run_dax(dax_dev);
	return devm_add_action_or_reset(dev, dev_dax_kill, dev_dax);
}
EXPORT_SYMBOL_GPL(dev_dax_probe);

static int dev_dax_remove(struct device *dev)
{
	/* all probe actions are unwound by devm */
	return 0;
}

static struct dax_device_driver device_dax_driver = {
	.drv = {
		.probe = dev_dax_probe,
		.remove = dev_dax_remove,
	},
	.match_always = 1,
};

static struct dentry *debug_dir, *DRAM_PT, *PM_PT, *Any_DRAM_PT;
void *DRAM_PT_buf, *PM_PT_buf, *Any_DRAM_PT_buf;

struct task_struct *get_task_by_pid(pid_t pid)
{
	struct task_struct *task = NULL;
	rcu_read_lock();
	task = find_task_by_vpid(pid);
	if (task)
		get_task_struct(task);
	rcu_read_unlock();
	return task;
}

// For users to read value from kernel. Actually, it is a function for MPFS to write data to a debugfs file.
static ssize_t mpfs_print_Any_DRAM_PT(struct file *file, char __user *user_buf,
				      size_t count, loff_t *ppos)
{
	struct task_struct *task = NULL;

	if (debugfs_Any_DRAM_PT_pid != 0)
		task = get_task_by_pid(debugfs_Any_DRAM_PT_pid);
	else {
		memset(Any_DRAM_PT_buf, 0, MPFS_DEUBGFS_FS_SIZE);
		sprintf(Any_DRAM_PT_buf, "PID is 0\n");
		return simple_read_from_buffer(user_buf, count, ppos,
					       Any_DRAM_PT_buf,
					       MPFS_DEUBGFS_FS_SIZE);
	}

	if (task == NULL) {
		memset(Any_DRAM_PT_buf, 0, MPFS_DEUBGFS_FS_SIZE);
		sprintf(Any_DRAM_PT_buf, "task is NULL\n");
		return simple_read_from_buffer(user_buf, count, ppos,
					       Any_DRAM_PT_buf,
					       MPFS_DEUBGFS_FS_SIZE);
	}

	Any_DRAM_page_table_to_DEBUGFS(task);
	return simple_read_from_buffer(user_buf, count, ppos, Any_DRAM_PT_buf,
				       MPFS_DEUBGFS_FS_SIZE);
}

// For MPFS to read data from a debugfs file. Actually, it is a function for users to write data to kernel.
static ssize_t mpfs_write_debugfs_Any_DRAM_PT(struct file *file,
					      const char __user *user_buf,
					      size_t count, loff_t *ppos)
{
	char buf[64];
	int buf_size;

	buf_size = min(count, (size_t)(sizeof(buf) - 1));
	if (copy_from_user(buf, user_buf, buf_size))
		return -EFAULT;
	buf[buf_size] = '\0';

	if (sscanf(buf, "%d", &debugfs_Any_DRAM_PT_pid) != 1)
		return -EINVAL;
	printk("debugfs_Any_DRAM_PT_pid: %d\n", debugfs_Any_DRAM_PT_pid);
	return count;
}

static ssize_t mpfs_print_DRAM_PT(struct file *file, char __user *user_buf,
				  size_t count, loff_t *ppos)
{
	DRAM_page_table_to_DEBUGFS();
	return simple_read_from_buffer(user_buf, count, ppos, DRAM_PT_buf,
				       MPFS_DEUBGFS_FS_SIZE);
}

// For MPFS to read data from a debugfs file. Actually, it is a function for users to write data to kernel.
static ssize_t mpfs_write_debugfs_DRAM_PT(struct file *file,
					  const char __user *user_buf,
					  size_t count, loff_t *ppos)
{
	char buf[64];
	int buf_size;

	buf_size = min(count, (size_t)(sizeof(buf) - 1));
	if (copy_from_user(buf, user_buf, buf_size))
		return -EFAULT;
	buf[buf_size] = '\0';

	if (sscanf(buf, "%ld", &debugfs_DRAM_PT_index) != 1)
		return -EINVAL;
	printk("debugfs_DRAM_PT_index: %ld\n", debugfs_DRAM_PT_index);
	return count;
}

// For users to read value from kernel. Actually, it is a function for MPFS to write data to a debugfs file.
static ssize_t mpfs_print_PM_PT(struct file *file, char __user *user_buf,
				size_t count, loff_t *ppos)
{
	PM_page_table_to_DEBUGFS();
	return simple_read_from_buffer(user_buf, count, ppos, PM_PT_buf,
				       MPFS_DEUBGFS_FS_SIZE);
}

// For MPFS to read data from a debugfs file. Actually, it is a function for users to write data to kernel.
static ssize_t mpfs_write_debugfs_PM_PT(struct file *file,
					const char __user *user_buf,
					size_t count, loff_t *ppos)
{
	char buf[64];
	int buf_size;

	buf_size = min(count, (size_t)(sizeof(buf) - 1));
	if (copy_from_user(buf, user_buf, buf_size))
		return -EFAULT;
	buf[buf_size] = '\0';

	if (sscanf(buf, "%ld", &debugfs_PM_PT_index) != 1)
		return -EINVAL;
	printk("debugfs_PM_PT_index: %ld\n", debugfs_PM_PT_index);
	return count;
}

static const struct file_operations mpfs_Any_DRAM_PT_fop = {
	.read = mpfs_print_Any_DRAM_PT,
	.write = mpfs_write_debugfs_Any_DRAM_PT,
};

static const struct file_operations mpfs_DRAM_PT_fop = {
	.read = mpfs_print_DRAM_PT,
	.write = mpfs_write_debugfs_DRAM_PT,
};

static const struct file_operations mpfs_PM_PT_fop = {
	.read = mpfs_print_PM_PT,
	.write = mpfs_write_debugfs_PM_PT,
};

static int __init dax_init(void)
{
	debug_dir = debugfs_create_dir("MPFS", NULL);
	if (!debug_dir) {
		printk(KERN_ERR "Failed to create debugfs directory\n");
		return -ENOMEM;
	}

	Any_DRAM_PT =
		debugfs_create_file("Any_DRAM_Page_Table", 0644, debug_dir,
				    NULL, &mpfs_Any_DRAM_PT_fop);
	if (!Any_DRAM_PT) {
		printk(KERN_ERR "Failed to create debugfs file: Any_DRAM_PT\n");
		return -ENOMEM;
	}

	DRAM_PT = debugfs_create_file("DRAM_Page_Table", 0644, debug_dir, NULL,
				      &mpfs_DRAM_PT_fop);
	if (!DRAM_PT) {
		printk(KERN_ERR "Failed to create debugfs file: DRAM_PT\n");
		return -ENOMEM;
	}

	PM_PT = debugfs_create_file("PM_Page_Table", 0644, debug_dir, NULL,
				    &mpfs_PM_PT_fop);
	if (!PM_PT) {
		printk(KERN_ERR "Failed to create debugfs file: PM_PT\n");
		return -ENOMEM;
	}

	Any_DRAM_PT_buf = kmalloc(MPFS_DEUBGFS_FS_SIZE, GFP_KERNEL);
	if (Any_DRAM_PT_buf == NULL) {
		printk(KERN_ERR
		       "Failed to allocate memory for Any_DRAM_PT_buf\n");
		return -ENOMEM;
	}

	DRAM_PT_buf = kmalloc(MPFS_DEUBGFS_FS_SIZE, GFP_KERNEL);
	if (DRAM_PT_buf == NULL) {
		printk(KERN_ERR "Failed to allocate memory for DRAM_PT_buf\n");
		return -ENOMEM;
	}

	PM_PT_buf = kmalloc(MPFS_DEUBGFS_FS_SIZE, GFP_KERNEL);
	if (PM_PT_buf == NULL) {
		printk(KERN_ERR "Failed to allocate memory for PM_PT_buf\n");
		return -ENOMEM;
	}

	return dax_driver_register(&device_dax_driver);
}

static void __exit dax_exit(void)
{
	kfree(Any_DRAM_PT_buf);
	kfree(DRAM_PT_buf);
	kfree(PM_PT_buf);
	debugfs_remove_recursive(debug_dir);
	dax_driver_unregister(&device_dax_driver);
}

#ifdef MPFS //! start of the MPFS part

// MPFS structures
static volatile struct mpfs_runtime mpfs_rt_instance = {
	.main_task = NULL,
	.pid_of_main_task = 0,
	.vstart_of_main_task = 0,
	.mpfs_status = MPFS_UNINIT,
	.shared_pgds = { { 0 }, { 0 }, { 0 }, { 0 } },
	.PT_reset_enable = false,
	.main_vma = NULL,
	.numof_installed_processes = 0,
};
static volatile struct mpfs_runtime *mpfs_rt = &mpfs_rt_instance;

// ctFS structures
static struct dax_runtime dax_rt_instance = { .master = NULL,
					      .pswap_temp = NULL };
static struct dax_runtime *rt = &dax_rt_instance;
static const char dax_master_magic_word[64] = DAX_MASTER_MAGIC_WORD;
static dax_master_page_t *current_master = NULL;
static struct vm_area_struct *current_vma = NULL;
static unsigned long current_start;
DEFINE_MUTEX(dax_lock);

// MPFS functions

pgdval_t get_pgd_entry(struct task_struct *task, unsigned long addr)
{
	pgdval_t *pgd_base_p = (pgdval_t *)task->mm->pgd;
	pgdval_t *pgd_entry =
		(pgdval_t *)(pgd_base_p + ((addr >> PGDIR_SHIFT) & 0x1ff));
	return *pgd_entry;
}

pgd_t *get_pointer_to_pgd(struct task_struct *task, unsigned long addr)
{
	pgdval_t *pgd_base_p = (pgdval_t *)&(task->mm->pgd->pgd);
	pgd_t *pgd_p = (pgd_t *)(pgd_base_p + ((addr >> PGDIR_SHIFT) & 0x1ff));
	return pgd_p;
}

pud_t *get_pointer_to_pud(pgd_t *pgd_p, unsigned long addr)
{
	pudval_t *pud_base_p =
		(pudval_t *)(((pgd_p->pgd >> 12) << 12) + page_offset_base);
	pud_t *pud_p = (pud_t *)(pud_base_p + ((addr >> 30) & 0x1ff));
	return pud_p;
}

pmd_t *get_pointer_to_pmd(pud_t *pud_p, unsigned long addr)
{
	pmdval_t *pmd_base_p =
		(pmdval_t *)(((pud_p->pud >> 12) << 12) + page_offset_base);
	pmd_t *pmd_p = (pmd_t *)(pmd_base_p + ((addr >> 21) & 0x1ff));
	return pmd_p;
}

pte_t *get_pointer_to_pte(pmd_t *pmd_p, unsigned long addr)
{
	pteval_t *pte_base_p =
		(pteval_t *)(((pmd_p->pmd >> 12) << 12) + page_offset_base);
	pte_t *pte_p = (pte_t *)(pte_base_p + ((addr >> 12) & 0x1ff));
	return pte_p;
}

bool is_valid_kernel_address(void *addr)
{
	if ((unsigned long)addr >= PAGE_OFFSET && (unsigned long)addr < -1) {
		return virt_addr_valid(addr);
	}
	return false;
}
/**
 * @description: Directly return the pte of the addr, if this is a huge page, return NULL. the pointer to pmd is also returned via pmd_pp which is a pointer to the pointer to pmd.
 * @return {*}
 */
pte_t *directly_get_pte(struct task_struct *task, unsigned long addr,
			pgd_t **pgd_pp, pud_t **pud_pp, pmd_t **pmd_pp)
{
	int i;
	pte_t *pte_p = NULL;

	*pgd_pp = get_pointer_to_pgd(task, addr);
	if (virt_addr_valid(*pgd_pp))
		if ((*pgd_pp)->pgd != 0)
			*pud_pp = get_pointer_to_pud(*pgd_pp, addr);
		else {
			*pud_pp = NULL;
			*pmd_pp = NULL;
			return pte_p;
		}
	else {
		*pgd_pp = NULL;
		*pud_pp = NULL;
		*pmd_pp = NULL;
		return pte_p;
	}

	if (virt_addr_valid(*pud_pp))
		if ((*pud_pp)->pud != 0)
			*pmd_pp = get_pointer_to_pmd(*pud_pp, addr);
		else {
			*pmd_pp = NULL;
			return pte_p;
		}
	else {
		*pud_pp = NULL;
		*pmd_pp = NULL;
		return pte_p;
	}

	if (virt_addr_valid(*pmd_pp))
		if ((*pmd_pp)->pmd != 0) {
			if ((*pmd_pp)->pmd & _PAGE_PSE) // if huge, return NULL
				return pte_p;
			else // if pmd stores a normal address
			{
				pte_p = get_pointer_to_pte(*pmd_pp, addr);
				return pte_p;
			}
		} else {
			return pte_p;
		}
	else {
		*pmd_pp = NULL;
		return pte_p;
	}
}

inline static relptr_t alloc_dax_pg(dax_runtime_t *rt)
{
	relptr_t pos;
	pos = bitmap_find_free_region((relptr_t *)rt->bitmap, rt->num_pages, 0);
	//?bitmap is typed as unsigned long--64bits
	clwb(rt->bitmap + (pos / 64));
	return (pos << PAGE_SHIFT);
}

inline static void free_dax_pg(dax_runtime_t *rt, relptr_t page)
{
	unsigned long pos = page >> PAGE_SHIFT;
	bitmap_release_region((relptr_t *)rt->bitmap, pos, 0);
	clwb(rt->bitmap + (pos / 64));
}

relptr_t alloc_dax_512pg(dax_runtime_t *rt)
{
	relptr_t pos;
	pos = bitmap_find_free_region((unsigned long *)rt->bitmap,
				      rt->num_pages, 9);
	clwb(rt->bitmap + (pos / 64));
	return (pos << PAGE_SHIFT);
}

inline static void free_dax_512pg(dax_runtime_t *rt, relptr_t page)
{
	relptr_t pos = page >> PAGE_SHIFT;
	bitmap_release_region((relptr_t *)rt->bitmap, pos, 9);
	clwb(rt->bitmap + (pos / 64));
}

// alloc a huge page (512 normal pages) via bitmap
pgoff_t mpfs_bitmap_512page_index(struct vm_area_struct *vma,
				  unsigned long address)
{
	// static uint64_t all_time = 0;
	// struct timespec64 start, end;
	// ktime_get_real_ts64(&start);
	pgoff_t pgoff;

	pgoff = bitmap_find_free_region((unsigned long *)rt->bitmap,
					rt->num_pages, 9);

	// ktime_get_real_ts64(&end);
	// uint64_t time = end.tv_nsec - start.tv_nsec +
	// 		1000000000 * (end.tv_sec - start.tv_sec);
	// all_time += time;
	// printk("fault-%d: %lld, %lld \n", current->tgid, time, all_time);

	clwb(rt->bitmap + (pgoff / 64));

	//?       <unused>         <master>      <bitmap>       <dump region>
	//? DAX_PSWAP_SHIFT   |-----1 PAGE-----|----....----|-------6 GB-------|
	pgoff +=
		(DAX_PSWAP_SHIFT >> PAGE_SHIFT); //skip the mpfs-reserved region
	pgoff += vma->vm_pgoff;
	return pgoff;
}
// alloc a normal pages via bitmap
pgoff_t mpfs_bitmap_page_index(struct vm_area_struct *vma,
			       unsigned long address)
{
	pgoff_t pgoff;

	pgoff = bitmap_find_free_region((unsigned long *)rt->bitmap,
					rt->num_pages, 0);
	clwb(rt->bitmap + (pgoff / 64));

	//?       <unused>         <master>      <bitmap>       <dump region>
	//? DAX_PSWAP_SHIFT   |-----1 PAGE-----|----....----|-------6 GB-------|
	pgoff +=
		(DAX_PSWAP_SHIFT >> PAGE_SHIFT); //skip the mpfs-reserved region
	pgoff += vma->vm_pgoff;
	return pgoff;
}

static inline pgoff_t mpfs_linear_page_index(struct vm_area_struct *vma,
					     unsigned long address)
{
	pgoff_t pgoff;
	if (unlikely(is_vm_hugetlb_page(vma)))
		return linear_hugepage_index(vma, address);
	//dax device run this way
	pgoff = (address - vma->vm_start) >> PAGE_SHIFT;

	// //?       <unused>         <master>      <bitmap>       <dump region>
	// //? DAX_PSWAP_SHIFT   |-----1 PAGE-----|----....----|-------6 GB-------|
	pgoff += ((DAX_PSWAP_SHIFT + rt->meta_size) >>
		  PAGE_SHIFT); //skip the mpfs-reserved region
	pgoff += vma->vm_pgoff;
	return pgoff;
}

// For DebugFS
void Any_DRAM_page_table_to_DEBUGFS(struct task_struct *task)
{
	int i = 0;
	off_t cursor = 0;
	pgd_t *pgd_p = NULL;
	pud_t *pud_p = NULL;
	pmd_t *pmd_p = NULL;
	pte_t *pte_p = NULL;

	memset(Any_DRAM_PT_buf, 0, MPFS_DEUBGFS_FS_SIZE);
	cursor += sprintf(Any_DRAM_PT_buf + cursor,
			  "Process %d - %lx Huge Page %ld address %lx:\n",
			  task->pid, (unsigned long)task, debugfs_DRAM_PT_index,
			  task->mpfs_vstart + (debugfs_DRAM_PT_index << 21));
	for (i = 0; i < PMD_SIZE / PAGE_SIZE; i++) {
		pte_p = directly_get_pte(task,
					 task->mpfs_vstart +
						 (debugfs_DRAM_PT_index << 21) +
						 i * PAGE_SIZE,
					 &pgd_p, &pud_p, &pmd_p);

		if (pgd_p == NULL || pud_p == NULL ||
		    pmd_p == NULL) { //if there is a page fault
			cursor +=
				sprintf(Any_DRAM_PT_buf + cursor,
					"pgd_p: %lx, pud_p: %lx, pmd_p: %lx\n",
					(unsigned long)pgd_p,
					(unsigned long)pud_p,
					(unsigned long)pmd_p);
			break;
		}

		if (pte_p == NULL)
			cursor +=
				sprintf(Any_DRAM_PT_buf + cursor,
					"P-%d: pgd: %lx pud: %lx pmd: %lx\n", i,
					pgd_p->pgd, pud_p->pud, pmd_p->pmd);
		else
			cursor += sprintf(
				Any_DRAM_PT_buf + cursor,
				"P-%d: pgd: %lx pud: %lx pmd: %lx pte: %lx\n",
				i, pgd_p->pgd, pud_p->pud, pmd_p->pmd,
				pte_p->pte);
	}
	// printk("Process %d DRAM page table to DebugFS: %lx\n", task->pid,
	//        cursor);
}

void DRAM_page_table_to_DEBUGFS(void)
{
	int i = 0;
	off_t cursor = 0;
	pgd_t *pgd_p = NULL;
	pud_t *pud_p = NULL;
	pmd_t *pmd_p = NULL;
	pte_t *pte_p = NULL;

	memset(DRAM_PT_buf, 0, MPFS_DEUBGFS_FS_SIZE);
	cursor += sprintf(DRAM_PT_buf + cursor, "Huge Page %ld:\n",
			  debugfs_DRAM_PT_index);
	for (i = 0; i < PMD_SIZE / PAGE_SIZE; i++) {
		pte_p = directly_get_pte(mpfs_rt->main_task,
					 mpfs_rt->vstart_of_main_task +
						 (debugfs_DRAM_PT_index << 21) +
						 i * PAGE_SIZE,
					 &pgd_p, &pud_p, &pmd_p);

		if (pud_p == NULL || pmd_p == NULL) { //if there is a page fault
			cursor +=
				sprintf(DRAM_PT_buf + cursor,
					"pgd_p: %lx, pud_p: %lx, pmd_p: %lx\n",
					pgd_p, pud_p, pmd_p);
			break;
		}
		if (pte_p == NULL)
			cursor +=
				sprintf(DRAM_PT_buf + cursor,
					"P-%d: pgd: %lx pud: %lx pmd: %lx\n", i,
					pgd_p->pgd, pud_p->pud, pmd_p->pmd);
		else
			cursor += sprintf(
				DRAM_PT_buf + cursor,
				"P-%d: pgd: %lx pud: %lx pmd: %lx pte: %lx\n",
				i, pgd_p->pgd, pud_p->pud, pmd_p->pmd,
				pte_p->pte);
	}
	// printk("DRAM page table to DebugFS: %lx\n", cursor);
}

void PM_page_table_to_DEBUGFS(void)
{
	int i = 0;
	off_t cursor = 0;

	memset(PM_PT_buf, 0, MPFS_DEUBGFS_FS_SIZE);
	cursor += sprintf(PM_PT_buf + cursor, "Huge Page %ld:\n",
			  debugfs_PM_PT_index);
	for (i = 0; i < PMD_SIZE / PAGE_SIZE; i++) {
		cursor += sprintf(
			PM_PT_buf + cursor, "P-%d: %lx at %lx\n", i,
			*(unsigned long *)(rt->dump_region +
					   debugfs_PM_PT_index * 4096 + i * 8),
			(unsigned long)(rt->dump_region +
					debugfs_PM_PT_index * 4096 + i * 8));
	}
	cursor += sprintf(PM_PT_buf + cursor, "\n");
}

static void do_downgrade_huge(struct task_struct *task, unsigned long addr,
			      pmd_t *pmdp);

static vm_fault_t mpfs_huge_fault(struct vm_fault *vmf, pgd_t **pgd_pp)
{
	static uint64_t all_time = 0;
	struct timespec64 start, end;

	struct task_struct *alloc_task = NULL;
	struct mm_struct *alloc_mm = NULL;
	unsigned long alloc_address = vmf->address;

	// check the validation of the vmf
	if (vmf->pud != NULL || vmf->pmd != NULL || vmf->pte != NULL) {
		printk(KERN_ERR "MPFS: HUGE FAULT error!\n");
		return VM_FAULT_SIGBUS;
	}

	if (mpfs_rt->main_task == NULL) {
		// current is the main task
		// the address does not need a translation
		alloc_task = current;
		alloc_mm = current->mm;
		alloc_address = vmf->address;

	} else {
		// current is not main task, we need to translate the page fault to be like in the main task
		alloc_task =
			mpfs_rt->main_task; //main task always play a important role in allocate page
		alloc_mm = mpfs_rt->main_task->mm;
		// translate the address in current task to the address in the main_task
		alloc_address = vmf->address - current->mpfs_vstart +
				mpfs_rt->main_task->mpfs_vstart;

		// translate the vma in current task to the vma in the main_task
		vmf->vma = mpfs_rt->main_vma;
		vmf->address = (alloc_address)&PAGE_MASK;
		vmf->pgoff =
			linear_page_index(mpfs_rt->main_vma, alloc_address);
	}

	p4d_t *p4d_p = p4d_alloc(alloc_mm, *pgd_pp, alloc_address);
	vmf->pud = pud_alloc(alloc_mm, p4d_p, alloc_address);
	vmf->pmd = pmd_alloc(alloc_mm, vmf->pud, alloc_address);

	struct dev_dax *dev_dax = vmf->vma->vm_file->private_data;
	volatile pfn_t pfn;
	unsigned long pmd_addr = vmf->address & PMD_MASK;
	struct device *dev = &dev_dax->dev;
	struct dax_region *dax_region;
	phys_addr_t phys;
	pgoff_t pgoff;
	unsigned int fault_size = PMD_SIZE;

	if (check_vma(dev_dax, vmf->vma, __func__)) {
		printk("VM_FAULT_SIGBUS-1\n");
		return VM_FAULT_SIGBUS;
	}

	dax_region = dev_dax->region;
	if (dax_region->align > PMD_SIZE) {
		dev_dbg(dev, "alignment (%#x) > fault size (%#x)\n",
			dax_region->align, fault_size);
		printk("VM_FAULT_SIGBUS-2\n");
		return VM_FAULT_SIGBUS;
	}

	/* dax pmd mappings require pfn_t_devmap() */
	if ((dax_region->pfn_flags & (PFN_DEV | PFN_MAP)) !=
	    (PFN_DEV | PFN_MAP)) {
		dev_dbg(dev, "region lacks devmap flags\n");
		printk("VM_FAULT_SIGBUS-3\n");
		return VM_FAULT_SIGBUS;
	}

	if (fault_size < dax_region->align) {
		printk("VM_FAULT_SIGBUS-4\n");
		return VM_FAULT_SIGBUS;
	} else if (fault_size > dax_region->align) {
		printk("VM_FAULT_FALLBACK-1\n");
		return VM_FAULT_FALLBACK;
	}

	/* if we are outside of the VMA */
	if (pmd_addr < vmf->vma->vm_start ||
	    (pmd_addr + PMD_SIZE) > vmf->vma->vm_end) {
		printk("VM_FAULT_SIGBUS-5\n");
		return VM_FAULT_SIGBUS;
	}

	// ktime_get_real_ts64(&start);
	pgoff = mpfs_bitmap_512page_index(vmf->vma, pmd_addr);

	// ktime_get_real_ts64(&end);
	// uint64_t time = end.tv_nsec - start.tv_nsec +
	// 		1000000000 * (end.tv_sec - start.tv_sec);
	// all_time += time;
	// printk("fault-%d: %lld, %lld \n", current->tgid, time, all_time);

	//get the physical address of the page
	phys = dax_pgoff_to_phys(dev_dax, pgoff, PMD_SIZE);
	if (phys == -1) {
		dev_dbg(dev, "pgoff_to_phys(%#lx) failed\n", pgoff);
		printk("VM_FAULT_SIGBUS-6\n");
		return VM_FAULT_SIGBUS;
	}

	pmd_t huge_page;
	// 0x8e7 is the typical flags of a huge page
	huge_page.pmd = phys | 0x8e7;
	huge_page.pmd |= ((pmdval_t)0x1 << _PAGE_BIT_SOFTW4);
	huge_page.pmd |= ((pmdval_t)0x1 << _PAGE_BIT_NX);
	// set the pkey bit to 1
	huge_page.pmd |= ((pmdval_t)0x1 << _PAGE_BIT_PKEY_BIT0);
	//populate the pmd in DRAM page table
	set_pmd(vmf->pmd, huge_page);

	//populate the pmd in PM page table (dump region)
	volatile pmd_t pmd = mpfs_dax_dump_pmd(alloc_task, vmf);

	// MPFS_PRINT(
	// 	"MPFS HUGE FAULT: at %lx huge page %ld: %lx, in process %lx\n",
	// 	vmf->address, vmf->pgoff / 512, pmd.pmd,
	// 	(unsigned long)current);

	return VM_FAULT_NOPAGE;
}

/**
 * @description: insert a 8B entry to the dump region
 * @param {struct task_struct *} task: used to specific the address space
 * @param {unsigned long} addr: the address of page in the above address space
 * @return {*}
 */
static pmd_t mpfs_dax_dump_pmd(struct task_struct *task, struct vm_fault *vmf)
{
	void *dump_region = rt->dump_region;
	pmd_t *pmd_p = vmf->pmd;
	unsigned long addr = vmf->address;

	//if it is a HUGE page
	if (pmd_p->pmd != 0 && pmd_p->pmd & _PAGE_PSE) {
		// move the cursor to be aligned with 2MB, the addr may not be aligned with 2MB
		addr = (addr >> 21) << 21;
		// dump region is organized as a linearly list of PTEs,
		// the size of dump region is only 1/512(>>9) of the size of mapped memory.
		// for HUGE page, we only record PMD at the first dump entry of this page
		memcpy(dump_region + ((addr - task->mpfs_vstart) >> 9),
		       (void *)pmd_p, 8);
		return *pmd_p;
		// printk("successfully dump pmd: %lx to dump region at %lx\n",
		//        pmd_p->pmd,
		//        (unsigned long)(dump_region +
		// 		       ((addr - task->mpfs_vstart) >> 9)));
	} else {
		// if it is a normal page, there is a bug here
		// because MPFS only enable huge page fault.
		BUG_ON(1);
		return *pmd_p;
	}
}

static pte_t mpfs_dax_dump_ptes(struct task_struct *task, pte_t *pte_p,
				unsigned long addr)
{
	void *dump_region = rt->dump_region;

	// Although it is a normal page, the dump should be done from the first entry of a PTEs
	BUG_ON(((addr >> 21) << 21) != addr);
	// if the the target dump region does not exist a PMD before, to guarantee data consistency, we must constrcut a fake PMD and store it at the the first entry of PTEs so that even the system crashes, we can still restore from PMD.
	pmd_t fake_pmd = { pte_p->pte | _PAGE_PSE };

	if (virt_addr_valid(pte_p) && (pte_p->pte & _PAGE_PRESENT)) {
		// first polulate the fake PMD
		memcpy(dump_region + ((addr - task->mpfs_vstart) >> 9),
		       (void *)&fake_pmd, 8);
		// dump region is organized as a linearly list of PTEs, the size of dump region is only 1/512(>>9) of the size of mapped memory.
		memcpy(dump_region + ((addr - task->mpfs_vstart) >> 9) + 8,
		       (void *)pte_p + 8, 4088);
		// to guarantee the data consistency, the first entry that stores the PMD
		// should be overwritten at last so that we can still restore from PMD
		// while encountering a system crash.
		memcpy(dump_region + ((addr - task->mpfs_vstart) >> 9),
		       (void *)pte_p, 8);
		return *pte_p;

	} else {
		BUG_ON(1);
		return (pte_t){ 0 };
	}
}

/**
 * @description: dump all PTEs to the dax device
 * @return {*}
 */
static void mpfs_page_table_dax_dump(void)
{
	void *dump_region = rt->dump_region;
	struct task_struct *task = mpfs_rt->main_task;

	// traverse all existing mappings from the start of vaddr of mpfs in main_task with the granularity of PAGE_SIZE
	unsigned long addr;
	for (addr = task->mpfs_vstart; addr < task->mpfs_vstart + MAP_SIZE;) {
		pgd_t *pgd_p = NULL;
		pud_t *pud_p = NULL;
		pmd_t *pmd_p = NULL;
		pte_t *pte_p =
			directly_get_pte(task, addr, &pgd_p, &pud_p, &pmd_p);

		//if it is a HUGE page, then we skip 512 PTEs
		if (pte_p == NULL && pmd_p->pmd & _PAGE_PSE) {
			// move the cursor to be aligned with 2MB
			addr = (addr >> 21) << 21;
			// dump region is organized as a linearly list of PTEs
			// the size of dump region is only 1/512(>>9) of the size of mapped memory.
			// for HUGE page, we only record PMD at the first dump entry of this page
			memcpy(dump_region + ((addr - task->mpfs_vstart) >> 9),
			       pmd_p, 8);

			// so the addr need to skip 4KB*512 = 2MB
			addr += (PAGE_SIZE << 9);
			continue;
		}

		// if it is a normal page, then we dump the whole page that this PTE resides in to the dump_region
		if (pte_p->pte & _PAGE_PRESENT) {
			// we dump all 512 PTEs in the page where pte_p->pte resides in
			// get the address of the first PTE in the page
			pte_p = (pte_t *)(((unsigned long)pte_p >> 12) << 12);
			// move the cursor to be aligned with 2MB
			addr = (addr >> 21) << 21;
			// dump region is organized as a linearly list of PTEs, the size of dump region is only 1/512(<<9) of the size of mapped memory.
			memcpy(dump_region + ((addr - task->mpfs_vstart) >> 9),
			       pte_p, PAGE_SIZE);

			// so the addr need to skip 4KB*512 = 2MB
			addr += (PAGE_SIZE << 9);
			continue;
		}

		//does not find any valid PTE, then we skip this page
		addr += PAGE_SIZE;
	}
}

void mpfs_scan_dax_dump_region(void);
/**
 * @description: restore the page table with the PTEs stored in the dump region
 * @return {*}
 */
static void mpfs_rebuild_page_table_with_dax(struct task_struct *task)
{
	void *dump_region = rt->dump_region;

	mpfs_scan_dax_dump_region();
	MPFS_PRINT(
		"MPFS driver: rebuilding DRAM page table in process %lx pid: %d\n",
		(unsigned long)current, current->pid);
	//traverse the dump_region with the granularity of 2MB
	unsigned long addr;
	for (addr = task->mpfs_vstart; addr < task->mpfs_vstart + MAP_SIZE;
	     addr += PMD_SIZE) {
		// convert the addr to the address of the first PTE in the page
		pte_t *PTE = (pte_t *)(dump_region +
				       ((addr - task->mpfs_vstart) >> 9));

		//check is there any valid PTE in this dax page.
		uint16_t index;
		for (index = 0; index < 512; index++) {
			if (PTE[index].pte & _PAGE_PRESENT) {
				//if there is any valid PTE, then we restore the whole page
				break;
			}
		}
		if (index >= 512) {
			continue; //if any valid PTEs is detected, index will be less than 512
		} else
			; //executing the following code to restore the page table

		pgd_t *pgd;
		p4d_t *p4d;
		pud_t *pud;
		pmd_t *pmd;
		pte_t *pte;

		pgd = pgd_offset(task->mm, addr & PAGE_MASK);
		p4d = p4d_alloc(task->mm, pgd, addr & PAGE_MASK);
		pud = pud_alloc(task->mm, p4d, addr & PAGE_MASK);
		pmd = pmd_alloc(task->mm, pud, addr & PAGE_MASK);

		//if it is a HUGE page, populate the PMD entry
		if (((pmd_t *)PTE)->pmd & _PAGE_PSE) {
			//pmd is stored in the first 8 bytes of PTE
			memcpy((void *)pmd, (void *)PTE, 8);
		} else { //if it is a normal page, populate the PTEs
			pte = pte_alloc_map(task->mm, pmd, addr);
			memcpy((void *)pte, (void *)PTE, PAGE_SIZE);
		}
	}
}

int mpfs_install_shared_PGD(void)
{
	pgd_t *pgd_p = NULL;
	int i = 0;
	bool whether_installed = false;
	while (mpfs_rt->mpfs_status != MPFS_READY)
		; // wait for the shared pgds to be ready
	MPFS_PRINT(
		"MPFS INSTALL: install shared PGDs in process: %lx, pid: %d, tgid: %d\n",
		(unsigned long)current, current->pid, current->tgid);
	for (i = 0; i < NUMOF_SHARED_PGDS; i++) {
		pgd_p = get_pointer_to_pgd(current, current->mpfs_vstart +
							    i * PGDIR_SIZE);
		if (mpfs_rt->shared_pgds[i].pgd && pgd_p->pgd == 0) {
			set_pgd(pgd_p, mpfs_rt->shared_pgds[i]);
			whether_installed = true; // if any pgd is installed
			MPFS_PRINT(
				"=>MPFS INSTALL: install PGD %d in %lx: %lx\n",
				i, (unsigned long)pgd_p,
				mpfs_rt->shared_pgds[i].pgd);
		} else {
			MPFS_PRINT(
				"=>MPFS INSTALL: do not need to share PGD %d in %lx: %lx\n",
				i, (unsigned long)pgd_p,
				mpfs_rt->shared_pgds[i].pgd);
			if (mpfs_rt->shared_pgds[i].pgd != 0) {
				if (((pgd_p->pgd >> 12) << 12) ==
				    ((mpfs_rt->shared_pgds[i].pgd << 12) >>
				     12)) {
					printk(KERN_ERR
					       "MPFS ERROR: shared PGD %d in %lx: %lx has been installed\n",
					       i, (unsigned long)pgd_p,
					       pgd_p->pgd);
				} else {
					printk(KERN_ERR
					       "MPFS ERROR: original PGD %d in %lx: %lx will be overwritten\n",
					       i, (unsigned long)pgd_p,
					       pgd_p->pgd);
				}
			}
		}
	}

	if (whether_installed) {
		mpfs_rt->numof_installed_processes++;
		MPFS_PRINT("MPFS INSTALL: %ld processes are using MPFS\n",
			   mpfs_rt->numof_installed_processes);
		whether_installed = false;
		current->mpfs_enable = MPFS_ENABLED;
	}

	return 0;
}

void mpfs_uninstall_shared_PGD(struct task_struct *task)
{
	pgd_t *pgd_p = NULL;
	pgd_t null_pgd = { 0 };
	bool whether_uninstalled = false;
	int i = 0;
	if (current->mpfs_enable == MPFS_ENABLED &&
	    current->pid == current->tgid) {
		MPFS_PRINT(
			"MPFS UNINSTALL: uninstall shared PGDs in process %lx pid: %d, tgid: %d\n",
			(unsigned long)current, current->pid, current->tgid);
		for (i = 0; i < NUMOF_SHARED_PGDS; i++) {
			pgd_p = get_pointer_to_pgd(
				current, current->mpfs_vstart + i * PGDIR_SIZE);
			if (pgd_p->pgd != 0) {
				if (((mpfs_rt->shared_pgds[i].pgd >> 12)
				     << 12) == ((pgd_p->pgd >> 12) << 12)) {
					MPFS_PRINT(
						"->MPFS UNINSTALL: uninstall PGD %d in %lx: %lx\n",
						i, (unsigned long)pgd_p,
						pgd_p->pgd);
					set_pgd(pgd_p, null_pgd);
					whether_uninstalled =
						true; //if any pgd is uninstalled
				} else {
					printk(KERN_ERR
					       "->MPFS UNINSTALL: ERROR: PGD %lx has been overwritten\n",
					       pgd_p->pgd);
				}
			} else {
				MPFS_PRINT(
					"->MPFS UNINSTALL: do not need to uninstall PGD %d in %lx: %lx\n",
					i, (unsigned long)pgd_p, pgd_p->pgd);
			}
		}

		if (whether_uninstalled) {
			mpfs_rt->numof_installed_processes--;
			MPFS_PRINT(
				"MPFS UNINSTALL: %ld processes are still using MPFS\n",
				mpfs_rt->numof_installed_processes);
			whether_uninstalled = false;
		}
	}
}
EXPORT_SYMBOL_GPL(mpfs_uninstall_shared_PGD);

int mpfs_show_vma(struct file *filp, unsigned long ptr)
{
	struct vm_area_struct *found_vma = find_vma(current->mm, ptr);

	//find key
	unsigned long vma_pkey_mask =
		VM_PKEY_BIT0 | VM_PKEY_BIT1 | VM_PKEY_BIT2 | VM_PKEY_BIT3;
	unsigned long pkey =
		(found_vma->vm_flags & vma_pkey_mask) >> VM_PKEY_SHIFT;

	printk(KERN_INFO "----------MPFS-VMA-----------\n");
	printk(KERN_INFO "current task=> pid: %d  tgid: %d\n", current->pid,
	       current->tgid);
	printk(KERN_INFO "vma_flags:%lx, pkey store in vma: %ld\n",
	       found_vma->vm_flags, pkey);
	printk(KERN_INFO "-----------------------------\n");
	return 0;
}

void mpfs_show_page_table(struct task_struct *task, unsigned long addr,
			  const char *tips)
{
	printk(KERN_INFO "==============Show Page Table===============\n");
	printk(KERN_INFO "Event: %s addr:%lx\n", tips, addr);
	// printk(KERN_INFO "stack trace:\n");
	// dump_stack();
	pte_t *pte_p = NULL;

	pgd_t *pgd_p = get_pointer_to_pgd(task, addr);
	if (pgd_p->pgd == 0)
		printk(KERN_INFO "PGD FAULT\n");
	else {
		pud_t *pud_p = get_pointer_to_pud(pgd_p, addr);
		if (pud_p->pud == 0)
			printk(KERN_INFO "PGD:%lx->PUD FAULT\n", pgd_p->pgd);
		else {
			pmd_t *pmd_p = get_pointer_to_pmd(pud_p, addr);
			if (pmd_p->pmd == 0)
				printk(KERN_INFO
				       "PGD:%lx->PUD:%lx->PMD FAULT\n",
				       pgd_p->pgd, pud_p->pud);
			else {
				if (pmd_p->pmd & _PAGE_PSE) {
					printk(KERN_INFO
					       "PGD:%lx->PUD:%lx->PMD(PSE):%lx\n",
					       pgd_p->pgd, pud_p->pud,
					       pmd_p->pmd);
				} else {
					pte_p = get_pointer_to_pte(pmd_p, addr);
					if (pte_p->pte == 0)
						printk(KERN_INFO
						       "PGD:%lx->PUD:%lx->PMD:%lx->PTE FAULT\n",
						       pgd_p->pgd, pud_p->pud,
						       pmd_p->pmd, pte_p->pte);
					else
						printk(KERN_INFO
						       "PGD:%lx->PUD:%lx->PMD:%lx->PTE:%lx\n",
						       pgd_p->pgd, pud_p->pud,
						       pmd_p->pmd, pte_p->pte);
				}
			}
		}
	}
	printk(KERN_INFO "================================================\n");
}

int mpfs_show_real_page_table_ioctl(struct file *filp, unsigned long ptr)
{
	int i = 0;
	relptr_t *pmdp = NULL;

	dax_ioctl_prefault_t frame;
	if (copy_from_user(&frame, (void *)ptr, sizeof(dax_ioctl_prefault_t))) {
		printk("DAX Prefault: Error: User addr invalid\n");
		return -1;
	}

	unsigned long addr = (unsigned long)frame.addr;

	for (i = 0; i < frame.n_pmd; i++) {
		mpfs_show_page_table(current, addr + i * PMD_SIZE,
				     "Directly show real page table");
	}
	return 0;
}

int mpfs_disable_write_permission(struct file *filp, unsigned long ptr)
{
	pgd_t *pgd_p = NULL;
	int i = 0;
	for (i = 0; i < NUMOF_SHARED_PGDS; i++) {
		pgd_p = get_pointer_to_pgd(current, current->mpfs_vstart +
							    i * PGDIR_SIZE);
		if (mpfs_rt->shared_pgds[i].pgd)
			set_pgd(pgd_p, mpfs_rt->shared_pgds[i]);
	}
	return 0;
}

int mpfs_enable_write_permission(struct file *filp, unsigned long ptr)
{
	// int j = 0;

	// for (j = 0; j < NUMOF_SHARED_PGDS; j++) {
	// 	mpfs_show_page_table(current,
	// 			     current->mpfs_vstart + j * PGDIR_SIZE,
	// 			     "before Enable PGD permission");
	// }
	struct timespec64 start, end;
	ktime_get_real_ts64(&start);

	pgd_t *pgd_p = NULL;
	pgd_t writable_pgd;
	int i = 0;
	MPFS_PRINT(
		"MPFS GRANT ACCESS: set the PGD permission in process %lx pid: %d\n",
		(unsigned long)current, current->pid);
	for (i = 0; i < NUMOF_SHARED_PGDS; i++) {
		pgd_p = get_pointer_to_pgd(current, current->mpfs_vstart +
							    i * PGDIR_SIZE);
		if (mpfs_rt->shared_pgds[i].pgd != 0 &&
		    pgd_p->pgd == mpfs_rt->shared_pgds[i].pgd) {
			writable_pgd.pgd = (pgd_p)->pgd | _PAGE_RW;
			MPFS_PRINT(
				"--MPFS GRANT ACCESS: set PGD %d in %lx: from %lx to %lx\n",
				i, (unsigned long)pgd_p, pgd_p->pgd,
				writable_pgd.pgd);
			set_pgd((pgd_p), writable_pgd);
		} else {
			MPFS_PRINT(
				"--MPFS GRANT ACCESS: fail to set PGD %d in %lx: %lx \n",
				i, (unsigned long)pgd_p, pgd_p->pgd);
		}
	}

	ktime_get_real_ts64(&end);
	uint64_t time = end.tv_nsec - start.tv_nsec +
			1000000000 * (end.tv_sec - start.tv_sec);
	MPFS_PRINT(
		"--MPFS GRANT ACCESS: take %lld nanoseconds to set PGD permission\n",
		time);

	// for (j = 0; j < NUMOF_SHARED_PGDS; j++) {
	// 	mpfs_show_page_table(current,
	// 			     current->mpfs_vstart + j * PGDIR_SIZE,
	// 			     "after Enable PGD permission");
	// }

	return 0;
}

static void do_downgrade_huge(struct task_struct *task, unsigned long addr,
			      pmd_t *pmdp)
{
	// the downgraded page must be aligned with 2MB
	addr = addr >> 21 << 21;

	MPFS_PRINT("MPFS DOWNGRADE: downgrade huge page %ld at %lx\n",
		   (addr - task->mpfs_vstart) >> 21, addr);
	// get the huge page PAT from original PMD entry
	uint64_t PAT = pmdp->pmd >> _PAGE_BIT_PAT_LARGE & 0x01;
	// calculate the new PAT for normal PTE
	PAT = PAT << _PAGE_BIT_PAT;
	// get the huge page ATTR from original PMD entry
	uint64_t ATTR = pmdp->pmd & ~(PAGE_MASK);
	// calculate the new ATTR with PAT for normal PTE
	if (!PAT)
		ATTR = ATTR & ~((uint64_t)0x01 << _PAGE_BIT_PAT);
	else
		ATTR |= ((uint64_t)0x01 << _PAGE_BIT_PAT);

	// get the physical address(including the mkey in the high bits) of huge page, the low 12 bits is used for ATTR, and the 13-bit is used for huge-page PAT, so we need to reset the low 13 bits;
	uint64_t phys_addr = (pmdp->pmd >> 13 << 13);

	// reset PMD entry
	set_pmd(pmdp, __pmd(0));
	// allocate a new PTE group get the physical address of the first PTE in the huge page
	pte_t *pte_p = pte_alloc_map(task->mm, pmdp, addr >> 21 << 21);

	uint64_t index = 0;
	pte_t entry = { 0 };
	// construct the new PTE entry
	entry.pte = (pteval_t)(phys_addr | index | ATTR);
	uint64_t i;
	for (i = 0; i < 512; i++) {
		index = i << 12;
		entry.pte = (pteval_t)(phys_addr | index | ATTR);
		// set the new PTE entry
		set_pte(pte_p + i, entry);
	}
	// dump the new PTEs to the dump region
	mpfs_dax_dump_ptes(task, pte_p, addr);
}

// recover dump region via a pswap log
void mpfs_dax_pswap_recovery(dax_pswap_log_t *log)
{
	void *ufirst_addr =
		(void *)log - (dax_pswap_npgs[log->npgs_enum_index] << 3);
	void *usecond_addr = rt->dump_region + (log->usecond_index << 3);

	MPFS_PRINT(
		"MPFS DAX RECOVERY: recover pswap log at %lx, ufirst_index: %d, usecond_index: %d, npgs:%ld\n",
		(unsigned long)log,
		(uint32_t)(ufirst_addr - rt->dump_region) >> 3,
		(uint32_t)(usecond_addr - rt->dump_region) >> 3,
		dax_pswap_npgs[log->npgs_enum_index]);

	switch (log->status) {
	case PSWAP_ONGOING:
		goto step_2;
		break;
	case PSWAP_UFIRST:
		goto step_3;
		break;
	case PSWAP_USECOND:
		goto step4;
		break;
	default:
		printk(KERN_ERR "MPFS ERROR: invalid pswap log status\n");
		BUG_ON(1);
	}

step_2: //copy the content of usecond to ufirst
	memcpy(ufirst_addr, usecond_addr,
	       dax_pswap_npgs[log->npgs_enum_index] << 3);
	log->status = PSWAP_UFIRST;

step_3: //zero the range of second
	memset(usecond_addr, 0, dax_pswap_npgs[log->npgs_enum_index] << 3);
	log->status = PSWAP_USECOND;

step4: //empty the log
	memset(log, 0, 8);
}

// scan the dump region to find a pswap log
void mpfs_scan_dax_dump_region(void)
{
	int i = 0;
	for (i = 0; i < MAP_SIZE >> PAGE_SHIFT; i++) {
		dax_pswap_log_t *log =
			(dax_pswap_log_t *)(rt->dump_region + (i << 3));
		if (log->magic_word == ATOMIC_PSWAP_LOG_MAIGC_WORD) {
			mpfs_dax_pswap_recovery(log);
		}
	}
}

/**
 * @description: subfunction of pswap_for_mpfs to swap entries in PM dump region atomically
 * @param {unsigned long} ufirst: the starting address of the first group of pages (new)
 * @param {unsigned long} usecond: the starting address of the second group of pages
 * @param {unsigned long} npgs: number of consectuive pages(normal page)
 * @return {int} error code
 */
inline int mpfs_atomic_dax_pswap(unsigned long ufirst, unsigned long usecond,
				 uint8_t npgs_enum_index)
{
	dax_pswap_log_t log = {
		.magic_word = ATOMIC_PSWAP_LOG_MAIGC_WORD,
		.status = PSWAP_ONGOING,
		.npgs_enum_index = npgs_enum_index,
		.usecond_index = (usecond - mpfs_rt->vstart_of_main_task) >> 12,
	};
	//note the ufist and usecond has been converted to be the virtual address in the main_task
	void *ufirst_addr = rt->dump_region +
			    ((ufirst - mpfs_rt->vstart_of_main_task) >> 9);
	void *usecond_addr = rt->dump_region +
			     ((usecond - mpfs_rt->vstart_of_main_task) >> 9);
	void *log_addr =
		ufirst_addr + (dax_pswap_npgs[npgs_enum_index]
			       << 3); // skip the range should be swapped

	BUG_ON(((pte_t *)ufirst_addr)->pte != 0); // the ufirst must be empty
	BUG_ON(((pte_t *)log_addr)->pte != 0); // the log entry must be empty

	//step1. write log to (npgs+1)st entry in usecond
	memcpy(log_addr, &log, sizeof(dax_pswap_log_t));

	//step2. copy the content of usecond to ufirst
	memcpy(ufirst_addr, usecond_addr, dax_pswap_npgs[npgs_enum_index] << 3);
	// switch the status of log
	((dax_pswap_log_t *)log_addr)->status = PSWAP_UFIRST;

	//step3. zero the range of second
	memset(usecond_addr, 0, dax_pswap_npgs[npgs_enum_index] << 3);
	// switch the status of log
	((dax_pswap_log_t *)log_addr)->status = PSWAP_USECOND;

	// stpe4. empty the log
	memset(log_addr, 0, 8);
	return 0;
}

/**
 * @description: swap the DRAM page table and PM dump region
 * @param {unsigned long} ufirst: the starting address of the first group of pages
 * @param {unsigned long} usecond: the starting address of the second group of pages
 * @param {unsigned long} npgs: number of consectuive pages(normal page)
 * @return {int} error code
 */
static int pswap_for_mpfs(unsigned long ufirst, unsigned long usecond,
			  unsigned long npgs)
{
	pgd_t *pgd_p1, *pgd_p2, *pgd_p_swap = NULL;
	pud_t *pud_p1, *pud_p2, *pud_p_swap = NULL;
	pmd_t *pmd_p1, *pmd_p2, *pmd_p_swap = NULL;
	pte_t *pte_p1, *pte_p2, *pte_p_swap = NULL;

	uint64_t numof_PTEs;
	uint64_t numof_PMDs;
	uint64_t numof_PUDs;

	uint8_t npgs_enum_index = 0;

	// for populating normal page table entry
	uint64_t ATTR = 0x8c00000000000867; // XD|mpkey|000000000|FLAGs of PTE，
	// FLAGs of PTE is 0x867 which is different from huge page entry in PMD because of PAT bit
	pte_t entry = { 0 };
	uint64_t index = 0;
	uint64_t p = 0, q = 0;
	pgoff_t pgoff = 0;
	uint64_t phys_addr = 0;
	struct dev_dax *dev_dax = mpfs_rt->main_vma->vm_file->private_data;

	// check the validation of the input parameters
	if (ufirst < current->mpfs_vstart ||
	    ufirst > current->mpfs_vstart + MAP_SIZE)
		return EPERM;

	if (usecond < current->mpfs_vstart ||
	    usecond > current->mpfs_vstart + MAP_SIZE)
		return EPERM;

	//convert the virtual address to the address in the main_task
	BUG_ON(mpfs_rt->main_task == NULL);
	ufirst = ufirst - current->mpfs_vstart + mpfs_rt->vstart_of_main_task;
	usecond = usecond - current->mpfs_vstart + mpfs_rt->vstart_of_main_task;

	//only the follwing cases are allowed
	switch (npgs) {
	case one_PTE:
		npgs_enum_index = 0;
		goto PTE_SWAP;
		break;
	case eight_PTE:
		npgs_enum_index = 1;
		goto PTE_SWAP;
		break;
	case sixtyfour_PTE:
		npgs_enum_index = 2;
		goto PTE_SWAP;
		break;
	case one_PMD:
		npgs_enum_index = 3;
		goto PMD_SWAP;
		break;
	case eight_PMD:
		npgs_enum_index = 4;
		goto PMD_SWAP;
		break;
	case sixtyfour_PMD:
		npgs_enum_index = 5;
		goto PMD_SWAP;
		break;
	case one_PUD:
		npgs_enum_index = 6;
		goto PUD_SWAP;
		break;
	case eight_PUD:
		npgs_enum_index = 7;
		goto PUD_SWAP;
		break;
	case sixtyfour_PUD:
		npgs_enum_index = 8;
		goto PUD_SWAP;
		break;
	case one_PGD:
		npgs_enum_index = 9;
		goto PGD_SWAP;
		break;
	default:
		return EINVAL;
	}

PTE_SWAP:
	BUG_ON(1);
	BUG_ON(ufirst >> 12 << 12 != ufirst);
	BUG_ON(usecond >> 12 << 12 != usecond);
	//dmup region pswap must be executed before DRAM page table pswap

	MPFS_PRINT(
		"MPFS PSWAP: PTE ufirst_index: %ld, %ld (Huge), usecond_index: %ld, %ld (Huge)\nnpgs: %ld in process %lx pid: %d tgid: %d\n",
		(ufirst - mpfs_rt->vstart_of_main_task) >> 12,
		(ufirst - mpfs_rt->vstart_of_main_task) >> 21,
		(usecond - mpfs_rt->vstart_of_main_task) >> 12,
		(usecond - mpfs_rt->vstart_of_main_task) >> 21,
		dax_pswap_npgs[npgs_enum_index], (unsigned long)current,
		current->pid, current->tgid);

	pgd_p1 = pgd_offset(mpfs_rt->main_task->mm, ufirst);
	pgd_p2 = pgd_offset(mpfs_rt->main_task->mm, usecond);
	pud_p1 = pud_alloc(mpfs_rt->main_task->mm, (p4d_t *)pgd_p1, ufirst);
	pud_p2 = pud_alloc(mpfs_rt->main_task->mm, (p4d_t *)pgd_p2, usecond);
	pmd_p1 = pmd_alloc(mpfs_rt->main_task->mm, pud_p1, ufirst);
	pmd_p2 = pmd_alloc(mpfs_rt->main_task->mm, pud_p2, usecond);

	//if this is a HUGE page, then we downgrade this page to 512 normal pages
	if (!pmd_none(*pmd_p1)) {
		if (pmd_p1->pmd & _PAGE_PSE) {
			// downgrade the huge page to normal pages, dax dump also be included in this function
			do_downgrade_huge(mpfs_rt->main_task,
					  ufirst >> 21 << 21, pmd_p1);
			pte_p1 = pte_offset_kernel(pmd_p1, ufirst);
			if (pte_none(*pte_p1)) {
				printk(KERN_ERR
				       "MPFS PSWAP: downgrade failed\n");
				return -1;
			}
		} else {
			pte_p1 = pte_offset_kernel(pmd_p1, ufirst);
		}
	} else {
		//pmd_p1 is empty, then we allocate a new PTE group
		pte_p1 = pte_alloc_map(mpfs_rt->main_task->mm, pmd_p1, ufirst);
		// populate normal page here
		pgoff = mpfs_bitmap_512page_index(mpfs_rt->main_vma, ufirst);
		//get the physical address of the page
		phys_addr = dax_pgoff_to_phys(dev_dax, pgoff, PMD_SIZE);
		for (p = 0; p < 512; p++) {
			index = p << 12;
			entry.pte = (pteval_t)(phys_addr | index | ATTR);
			// set the new PTE entry
			set_pte(pte_p1 + p, entry);
		}
		// dump the new PTEs to the dump region
		mpfs_dax_dump_ptes(mpfs_rt->main_task, pte_p1,
				   ufirst >> 21 << 21);
	}

	if (!pmd_none(*pmd_p2)) {
		if (pmd_p2->pmd & _PAGE_PSE) {
			do_downgrade_huge(mpfs_rt->main_task,
					  usecond >> 21 << 21, pmd_p2);
			pte_p2 = pte_offset_kernel(pmd_p2, usecond);
			if (pte_none(*pte_p2)) {
				printk(KERN_ERR "downgrade failed\n");
				return ENOMEM;
			}
		} else {
			pte_p2 = pte_offset_kernel(pmd_p2, usecond);
		}
	} else {
		pte_p2 = pte_alloc_map(mpfs_rt->main_task->mm, pmd_p2, usecond);
		// populate normal page here
		pgoff = mpfs_bitmap_512page_index(mpfs_rt->main_vma, usecond);
		//get the physical address of the page
		phys_addr = dax_pgoff_to_phys(dev_dax, pgoff, PMD_SIZE);
		for (q = 0; q < 512; q++) {
			index = q << 12;
			entry.pte = (pteval_t)(phys_addr | index | ATTR);
			// set the new PTE entry
			set_pte(pte_p2 + q, entry);
		}
		// dump the new PTEs to the dump region
		mpfs_dax_dump_ptes(mpfs_rt->main_task, pte_p2,
				   usecond >> 21 << 21);
	}
	// dax pswap should be done before DRAM page table pswap
	mpfs_atomic_dax_pswap(ufirst, usecond, npgs_enum_index);

	numof_PTEs = npgs / one_PTE;
	pte_p_swap = kmalloc(numof_PTEs * 8, GFP_ATOMIC);
	if (pte_p_swap == NULL) {
		printk(KERN_ERR "kmalloc failed\n");
		return ENOMEM;
	}
	uint64_t m;
	for (m = 0; m < numof_PTEs; m++) {
		// copy the content of pmd_p1 to pmd_p_swap
		pte_p_swap[m].pte = pte_p1[m].pte;
		// copy the content of pte_p2 to pte_p1
		pte_p1[m].pte = pte_p2[m].pte;
		// copy the content of pte_p_swap to pte_p2
		pte_p2[m].pte = pte_p_swap[m].pte;
	}
	return 0;

PMD_SWAP:
	BUG_ON(ufirst >> 21 << 21 != ufirst);
	BUG_ON(usecond >> 21 << 21 != usecond);

	MPFS_PRINT(
		"MPFS PSWAP: PMD ufirst_index: %ld, %ld (Huge), usecond_index: %ld, %ld (Huge)\nnpgs: %ld in process %lx pid: %d tgid: %d\n",
		(ufirst - mpfs_rt->vstart_of_main_task) >> 12,
		(ufirst - mpfs_rt->vstart_of_main_task) >> 21,
		(usecond - mpfs_rt->vstart_of_main_task) >> 12,
		(usecond - mpfs_rt->vstart_of_main_task) >> 21,
		dax_pswap_npgs[npgs_enum_index], (unsigned long)current,
		current->pid, current->tgid);

	//dmup region pswap must be executed before DRAM page table pswap
	mpfs_atomic_dax_pswap(ufirst, usecond, npgs_enum_index);

	pgd_p1 = pgd_offset(mpfs_rt->main_task->mm, ufirst);
	pgd_p2 = pgd_offset(mpfs_rt->main_task->mm, usecond);
	pud_p1 = pud_alloc(mpfs_rt->main_task->mm, (p4d_t *)pgd_p1, ufirst);
	pud_p2 = pud_alloc(mpfs_rt->main_task->mm, (p4d_t *)pgd_p2, usecond);
	pmd_p1 = pmd_alloc(mpfs_rt->main_task->mm, pud_p1, ufirst);
	pmd_p2 = pmd_alloc(mpfs_rt->main_task->mm, pud_p2, usecond);

	numof_PMDs = npgs / one_PMD;
	pmd_p_swap = kmalloc(numof_PMDs * 8, GFP_ATOMIC);
	if (pmd_p_swap == NULL) {
		printk(KERN_ERR "kmalloc failed\n");
		return ENOMEM;
	}
	//TODO: or using memcpy to replace the following loop
	uint64_t i;
	for (i = 0; i < numof_PMDs; i++) {
		// copy the content of pmd_p1 to pmd_p_swap
		pmd_p_swap[i].pmd = pmd_p1[i].pmd;
		// copy the content of pmd_p2 to pmd_p1
		pmd_p1[i].pmd = pmd_p2[i].pmd;
		// copy the content of pmd_p_swap to pmd_p2
		pmd_p2[i].pmd = pmd_p_swap[i].pmd;
	}

	kfree(pmd_p_swap);
	return 0;

PUD_SWAP:
	BUG_ON(ufirst >> 30 << 30 != ufirst);
	BUG_ON(usecond >> 30 << 30 != usecond);

	MPFS_PRINT(
		"MPFS PSWAP: PUD ufirst_index: %ld, %ld (Huge), usecond_index: %ld, %ld (Huge)\nnpgs: %ld in process %lx pid: %d tgid: %d\n",
		(ufirst - mpfs_rt->vstart_of_main_task) >> 12,
		(ufirst - mpfs_rt->vstart_of_main_task) >> 21,
		(usecond - mpfs_rt->vstart_of_main_task) >> 12,
		(usecond - mpfs_rt->vstart_of_main_task) >> 21,
		dax_pswap_npgs[npgs_enum_index], (unsigned long)current,
		current->pid, current->tgid);

	//dmup region pswap must be executed before DRAM page table pswap
	mpfs_atomic_dax_pswap(ufirst, usecond, npgs_enum_index);

	pgd_p1 = pgd_offset(mpfs_rt->main_task->mm, ufirst);
	pgd_p2 = pgd_offset(mpfs_rt->main_task->mm, usecond);
	pud_p1 = pud_alloc(mpfs_rt->main_task->mm, (p4d_t *)pgd_p1, ufirst);
	pud_p2 = pud_alloc(mpfs_rt->main_task->mm, (p4d_t *)pgd_p2, usecond);

	numof_PUDs = npgs / one_PUD;
	pud_p_swap = kmalloc(numof_PUDs, GFP_ATOMIC);
	if (pud_p_swap == NULL) {
		printk(KERN_ERR "kmalloc failed\n");
		return ENOMEM;
	}
	//TODO: or using memcpy to replace the following loop
	uint64_t j;
	for (j = 0; j < numof_PUDs; j++) {
		// copy the content of pud_p1 to pud_p_swap
		pud_p_swap[j].pud = pud_p1[j].pud;
		// copy the content of pud_p2 to pud_p1
		pud_p1[j].pud = pud_p2[j].pud;
		// copy the content of pud_p_swap to pud_p2
		pud_p2[j].pud = pud_p_swap[j].pud;
	}

	kfree(pud_p_swap);
	return 0;

//TODO: swapping PGD is unnecessary, because the PGD is the top directory of the page table.
PGD_SWAP:
	BUG_ON(1); //PGD swap is not allowed now.
	printk(KERN_ERR "fail to swap PGD\n");
	return -1;
}

int mpfs_init_main_task(struct file *filp, unsigned long ptr)
{
	struct task_struct *main_process = current;
	int i = 0;
	// Check if the new process was created successfully
	if (mpfs_rt->main_task == NULL) {
		if (!main_process) {
			printk(KERN_ERR "Error: Initializing main_task\n");
			return -1;
		} else {
			mpfs_rt->main_task = main_process;
			MPFS_PRINT("MPFS driver: main_task is %lx, pid:%d\n",
				   (unsigned long)mpfs_rt->main_task,
				   mpfs_rt->main_task->pid);
			mpfs_rt->pid_of_main_task = main_process->pid;
			mpfs_rt->vstart_of_main_task = ptr;
			current->mpfs_vstart = ptr;
			if (main_process->mm->pgd != NULL) {
				for (i = 0; i < NUMOF_SHARED_PGDS; i++) {
					mpfs_rt->shared_pgds[i].pgd =
						get_pgd_entry(
							main_process,
							mpfs_rt->vstart_of_main_task +
								i * PGDIR_SIZE) &
						(~_PAGE_RW);
					MPFS_PRINT(
						"MPFS initializes shared PGD %d: %lx in process: %lx, pid: %d\n",
						i, mpfs_rt->shared_pgds[i].pgd,
						(unsigned long)current,
						current->pid);
				}
				mpfs_rt->mpfs_status = MPFS_READY;
				return 0;
			} else {
				printk(KERN_ERR "ERROR: PGD is NULL\n");
				return -1;
			}
		}
	}
	return 0;
}

int mpfs_update_vstart_for_main_task(struct file *filp, unsigned long ptr)
{
	struct task_struct *main_process = current;
	main_process->mpfs_vstart = ptr;
	//! we must update mpfs_rt->main_vma while running main_task
	mpfs_rt->main_vma = find_vma(current->mm, ptr);
	// if MPFS does not reset the dump region, we need to rebuild the page table
	if (!mpfs_rt->PT_reset_enable) {
		//rebuild the page table according to info stored in dump region.
		mpfs_rebuild_page_table_with_dax(current);
		// we execute rebuilding here because this function will only be executed in main task
		// all the pgd, pud, pmd, pte should only be allocated by main task to avoid the memory leak
		// and this function will be executed before any page fault happens. so it is safe to rebuild the page table here, because the page table is not used by any other process.
	} else { // if MPFS reset the dump region
		//reset the dump region reset flag
		mpfs_rt->PT_reset_enable = false;
	}
	return 0;
}

int mpfs_create_main_task(void)
{
	char *argv[] = { "/bin/bash", "-c",
			 "/home/dingbo/Desktop/ctFS_qemu/main_task", NULL };
	char *envp[] = { "HOME=/", "TERM=linux",
			 "PATH=/sbin:/bin:/usr/sbin:/usr/bin", NULL };
	int result;

	MPFS_PRINT("MPFS driver: Creating main_task\n");

	// UMH_WAIT_EXEC will only invoke the user-space program without waiting for it exit
	// since "./main_task" uses a loop to keep the page table, we cannot wait for it.
	result = call_usermodehelper(argv[0], argv, envp, UMH_WAIT_EXEC);
	while (mpfs_rt->main_task == NULL)
		;
	if (result < 0) {
		printk(KERN_ERR
		       "MPFS driver: failed to create a main_task, error: %d\n",
		       result);
	} else {
		MPFS_PRINT("MPFS driver: main_task created successfully\n");
	}
	return result;
}

int mpfs_share_pagetable(struct file *filp, unsigned long ptr)
{
	int result = 0;

	//! update the the vstart of mpfs for the following initialization
	current->mpfs_vstart = ptr;
	if (!mpfs_rt->main_task) {
		mpfs_create_main_task();
		mpfs_install_shared_PGD();
	} else {
		mpfs_install_shared_PGD();
	}

	return result;
}

/*! orignal ctFS functions*/
long dax_handle_reset(struct file *filp)
{
	struct dev_dax *dev_dax;
	phys_addr_t dax_start_addr;
	dax_master_page_t *m_page_p;
	dev_dax = filp->private_data;
	dax_start_addr =
		dax_pgoff_to_phys(dev_dax, 0, PMD_SIZE) + DAX_PSWAP_SHIFT;
	m_page_p = (void *)dax_start_addr + __PAGE_OFFSET;

	if (MASTER_NOT_INIT(m_page_p)) {
		return EINVAL;
	} else {
		// set a bad magic word to trigger reset in dax_handle_init()
		strcpy(m_page_p->magic_word, "badbeef");
		clwb(m_page_p);

		// enable mpfs dump region reset once
		mpfs_rt->PT_reset_enable = true;
		return 0;
	}
}

inline void mpfs_release_shared_page_table(void)
{
	int i = 0, j = 0;
	pgtable_t pte;
	pte_t *pte_p = NULL;
	pmd_t *pmd_p = NULL;
	pud_t *pud_p = NULL;
	pgd_t *pgd_p = NULL;
	struct task_struct *task = mpfs_rt->main_task;
	struct mm_struct *mm = task->mm;
	unsigned long addr = 0;

	BUG_ON(mpfs_rt->main_task == NULL);

	for (i = 0; i < NUMOF_SHARED_PGDS; i++) {
		if (mpfs_rt->shared_pgds[i].pgd) {
			for (j = 0; j < PGDIR_SIZE / PMD_SIZE; j++) {
				// traverse the page table with the granularity of 2MB
				addr = mpfs_rt->vstart_of_main_task +
				       i * PGDIR_SIZE + j * PMD_SIZE;
				pte_p = directly_get_pte(task, addr, &pgd_p,
							 &pud_p, &pmd_p);

				if (pud_p != NULL) {
					if (pmd_p != NULL) {
						if (pmd_p->pmd & _PAGE_PSE) {
							// if it is a huge page

							// clear the physical address of the huge page
							pmd_clear(pmd_p);

							// if 512 PMD entries have been cleared, PMDs should be released
							if ((j + 1) % 512 ==
							    0) {
								pud_clear(
									pud_p);
								pmd_p = (pmd_t *)((unsigned long)
											  pmd_p &
										  PAGE_MASK);
								pmd_free(mm,
									 pmd_p);
								mm_dec_nr_pmds(
									mm);
							}
							continue;
						} else {
							// it is a normal pointer to PTEs,
							// directly_get_pte() has already check the invalidation of the pointer

							// clear PMD entry (storing the pointer to PTEs) and release PTEs
							if (pte_p != NULL) {
								pte = pmd_pgtable(
									*pmd_p);
								pmd_clear(
									pmd_p);
								pte_free(mm,
									 pte);
								mm_dec_nr_ptes(
									mm);
							}
							// clear PUD entry (storing the pointer to PTEs) and release PMDs
							if ((j + 1) % 512 ==
							    0) {
								pud_clear(
									pud_p);
								pmd_p = (pmd_t *)((unsigned long)
											  pmd_p &
										  PAGE_MASK);
								pmd_free(mm,
									 pmd_p);
								mm_dec_nr_pmds(
									mm);
							}
							// MPFS should not release PUDs while reseting shared page table,
							// so that we do not need to rebuilds the subtree
							continue;
						}
					}
					// MPFS should not release PUDs while reseting shared page table,
					// so that we do not need to rebuilds the subtree
					continue;
				}
				BUG_ON(1); // if the pgd is valid, the pud_p should not be NULL
				continue;
			}
		}
	}
}

//? init master page
void mpfs_kernel_reset(dax_master_page_t *mast_page, unsigned long dax_size)
{
	unsigned int pgs_master, pgs_bitmap, pgs_dump;
	MPFS_PRINT(
		"***** MPFS RESET: reset DAX in process %lx, pid: %d *****\n",
		(unsigned long)current, current->pid);
	/* Reset the master page */
	memset((void *)mast_page, 0, PAGE_SIZE);
	/* Set the magic code */
	strcpy(&(mast_page->magic_word[0]), dax_master_magic_word);
	//? caculate the number of pages in physical device
	mast_page->num_pages = dax_size >> PAGE_SHIFT;

	/* init runtime */
	rt->master = mast_page;
	rt->num_pages = mast_page->num_pages;
	rt->start_paddr = (phys_addr_t)((void *)mast_page - __PAGE_OFFSET);
	rt->start = (void *)mast_page;
	rt->mpk[0] = mm_pkey_alloc(current->mm);
	rt->mpk[1] = mm_pkey_alloc(current->mm);
	rt->mpk[2] = mm_pkey_alloc(current->mm);

	/* init bitmap */
	mast_page->bm_start_offset = PAGE_SIZE;
	//?       <unused>         <master>      <bitmap>       <dump region>
	//? DAX_PSWAP_SHIFT   |-----1 PAGE-----|----....----|-------6 GB-------|
	//? I don't know why the region sized by DAX_PSWAP_SHIFT cannot be used?
	//? maybe it's used by NVDIMM label?
	rt->bitmap = rt->start + mast_page->bm_start_offset;
	//? init the bitmap
	bitmap_zero((unsigned long *)rt->bitmap, rt->num_pages);

	//? set the the first page as used.
	pgs_master = 1;
	bitmap_set((unsigned long *)rt->bitmap, 0, pgs_master);

	//? calculate the size of bitmap, and then set the pages used by bitmap as used.
	pgs_bitmap = ((rt->num_pages >> 3) >> PAGE_SHIFT) + 1;
	bitmap_set((unsigned long *)rt->bitmap, 0 + pgs_master, pgs_bitmap);

	// start of the dump region, (rt->num_pages >> 3) is the size of bitmap.
	rt->dump_region = rt->start + ((pgs_master + pgs_bitmap) << PAGE_SHIFT);
	pgs_dump = MPFS_SIZEOF_DUMP_REGION >> PAGE_SHIFT;
	bitmap_set((unsigned long *)rt->bitmap, 0 + pgs_master + pgs_bitmap,
		   pgs_dump);

	rt->meta_size = ((pgs_master + pgs_bitmap) << PAGE_SHIFT) +
			MPFS_SIZEOF_DUMP_REGION;

	// we cannot reset dump_region to 0 only if PT_reset_enable is set, since it may contain the mapping of virtual to physical.
	if (mpfs_rt->PT_reset_enable) {
		// reset dump region
		memset(rt->dump_region, 0, MAP_SIZE >> PAGE_SHIFT << 3);

		// reset DRAM page table
		// if main_task has been created
		if (mpfs_rt->main_task) {
			// we need to release the used shared page table
			if (mpfs_rt->numof_installed_processes > 0) {
				printk("**MPFS RESET: ERROR: %ld processes are still using MPFS\n",
				       mpfs_rt->numof_installed_processes);
			} else {
				//release the PTEs and PMDs of the shared page table but remain PUDs and PGDs
				mpfs_release_shared_page_table();
			}
		}
	}

	// we need revisit this part, the master_page->pgd_offset now should be the offset of the dump region
	// this part should also be revisited, currently we don't need to alloc persistent pmd anymore since we remove the persistent page table
	// the pswap_temp seems to be never used currently. just remove it?
}

long dax_handle_init(struct file *filp, unsigned long ptr)
{
	struct dev_dax *dev_dax;
	phys_addr_t dax_start_addr;
	dax_master_page_t *m_page_p;
	struct dax_region *dax_region;
	dax_ioctl_init_t frame;
	MPFS_PRINT("MPFS HANDLE INIT: in process %lx, pid: %d, tgid: %d\n",
		   (unsigned long)current, current->pid, current->tgid);
	if (copy_from_user(&frame, (void *)ptr, sizeof(dax_ioctl_init_t))) {
		printk("DAX INIT: Error: User addr invalid: %#lx\n", ptr);
		return -1;
	}
	dev_dax = filp->private_data;
	dax_region = dev_dax->region;
	//? get the dax_start_addr
	//? res->start                    <dax_start_addr>            res->end
	//?  ^                                   ^                          ^
	//?  |----------DAX_PSWAP_SHIFT----------|(master_page)---- ... ----|
	//? note that <dax_start_addr> is a physical address
	//? I don't know why we need to add DAX_PSWAP_SHIFT here
	dax_start_addr =
		dax_pgoff_to_phys(dev_dax, 0, PMD_SIZE) + DAX_PSWAP_SHIFT;

	//? get the virtual address of master_page by adding the __PAGE_OFFSET to physical address
	//? because virtual address - physical address = __PAGE_OFFSET
	m_page_p = (void *)dax_start_addr + __PAGE_OFFSET;

	// if the master page is not initialized or demanded to be reset
	if (MASTER_NOT_INIT(m_page_p)) {
		// reset the file system metadata and init the runtime of MPFS
		mpfs_kernel_reset(m_page_p, dax_region->res.end -
						    dax_region->res.start -
						    DAX_PSWAP_SHIFT);
	} else {
		// directly init MPFS runtime via the metadata stored in PM.
		mpfs_kernel_init(m_page_p);
	}

	frame.space_total = m_page_p->num_pages * PAGE_SIZE;
	frame.mpk_meta = rt->mpk[DAX_MPK_META];
	frame.mpk_file = rt->mpk[DAX_MPK_FILE];
	frame.mpk_default = rt->mpk[DAX_MPK_DEFAULT];
	copy_to_user((void *)ptr, &frame, sizeof(frame));
	return 0;
}

static dax_master_page_t *get_master_page(struct vm_area_struct *vma)
{
	struct file *filp;
	struct dev_dax *dev_dax;
	phys_addr_t dax_start_addr;
	if (likely(vma == current_vma && vma->vm_start == current_start)) {
		return current_master;
	}
	if (unlikely(!vma)) {
		return NULL;
	}
	if (unlikely(!vma_is_dax(vma))) {
		return NULL;
	}
	filp = vma->vm_file;
	if (unlikely(!filp)) {
		return NULL;
	}
	dev_dax = filp->private_data;
	//? get the physical address of the first(0) page of dax_dev
	dax_start_addr =
		dax_pgoff_to_phys(dev_dax, 0, PMD_SIZE) + DAX_PSWAP_SHIFT;
	//? get the virtual address of dax_dev, __PAGE_OFFSET is the distance between va and pa.
	current_master =
		(dax_master_page_t *)((void *)dax_start_addr + __PAGE_OFFSET);
	//? verify the magic word stored in master
	if (unlikely(MASTER_NOT_INIT(current_master))) {
		current_master = NULL;
		return NULL;
	}
	current_start = vma->vm_start;
	current_vma = vma;
	return current_master;
}

static dax_runtime_t *mpfs_kernel_init(dax_master_page_t *master_page)
{
	unsigned int pgs_master, pgs_bitmap, pgs_dump;
	MPFS_PRINT(
		">>>>> MPFS INIT: init MPFS runtime in process %lx, pid: %d <<<<<\n",
		(unsigned long)current, current->pid);
	if (unlikely(master_page != rt->master)) {
		rt->master = master_page;
		rt->num_pages = master_page->num_pages; // already stored in PM
		rt->start_paddr =
			(phys_addr_t)((void *)master_page - __PAGE_OFFSET);
		rt->start = (void *)master_page;
		rt->bitmap = rt->start + master_page->bm_start_offset;

#ifdef MPFS
		pgs_master = 1;
		pgs_bitmap = ((rt->num_pages >> 3) >> PAGE_SHIFT) + 1;
		pgs_dump = MPFS_SIZEOF_DUMP_REGION >> PAGE_SHIFT;
		rt->dump_region =
			rt->start + ((pgs_master + pgs_bitmap) << PAGE_SHIFT);
		rt->meta_size = ((pgs_master + pgs_bitmap) << PAGE_SHIFT) +
				MPFS_SIZEOF_DUMP_REGION;
#else
		rt->pgd = DAX_REL2ABS(master_page->pgd_offset);
#endif
		rt->mpk[0] = mm_pkey_alloc(current->mm);
		rt->mpk[1] = mm_pkey_alloc(current->mm);
		rt->mpk[2] = mm_pkey_alloc(current->mm);
	}
	return rt;
}

long dax_handle_pcow(unsigned long ptr)
{
	dax_ioctl_pcow_t *ctl = kmalloc(sizeof(dax_ioctl_pcow_t), GFP_KERNEL);
	long ret;
	if (copy_from_user((void *)ctl, (void *)ptr,
			   sizeof(dax_ioctl_pcow_t))) {
		kfree(ctl);
		return EFAULT;
	}
	// ret = dax_pcow(ctl->src, ctl->dest, ctl->size, ctl->flag);
	kfree(ctl);
	return ret;
}

long dax_handle_ready(struct file *filp, unsigned long ptr)
{
	struct dev_dax *dev_dax;
	phys_addr_t dax_start_addr;
	dax_master_page_t *m_page_p;
	int ret = 0;

	dev_dax = filp->private_data;
	dax_start_addr =
		dax_pgoff_to_phys(dev_dax, 0, PMD_SIZE) + DAX_PSWAP_SHIFT;
	m_page_p = (void *)dax_start_addr + __PAGE_OFFSET;
	if (ptr == 0) {
		return 0;
	}
	if (MASTER_NOT_INIT(m_page_p)) {
#ifdef PSWAP_DEBUG
		printk("DAX Ready: NOT INITIALIZED\n");
#endif
		ret = 0;
		copy_to_user((void *)ptr, &ret, sizeof(int));
		return 0;
	} else {
#ifdef PSWAP_DEBUG
		printk("DAX Ready: INITIALIZED\n");
#endif
		ret = 1;
		copy_to_user((void *)ptr, &ret, sizeof(int));
		return 0;
	}
}

inline long dax_handle_pswap(unsigned long ptr)
{
	dax_ioctl_pswap_t ctl;
	long ret = 0;
	if (copy_from_user((void *)&ctl, (void *)ptr,
			   sizeof(dax_ioctl_pswap_t))) {
		return EFAULT;
	}
	ret = pswap_for_mpfs(ctl.ufirst, ctl.usecond, ctl.npgs);

	return ret;
}

long dax_ioctl(struct file *filp, unsigned int type, unsigned long ptr)
{
#if PSWAP_DEBUG > 1
	printk("DAX IOCTL: type: %u, ptr: %#lx", type, ptr);
#endif
	switch (type) {
	case DAX_IOCTL_PSWAP:
		return dax_handle_pswap(ptr);
		break;
	case DAX_IOCTL_RESET:
		return dax_handle_reset(filp);
		break;
	case DAX_IOCTL_INIT:
		return dax_handle_init(filp, ptr);
		break;
	case DAX_IOCTL_COW:
		return dax_handle_pcow(ptr);
		break;
	case DAX_IOCTL_READY:
		return dax_handle_ready(filp, ptr);
		break;
		// case DAX_IOCTL_PREFAULT:
		// 	return dax_handle_prefault(ptr);
		// 	break;

	case MPFS_IOCTL_PTSHARE:
		return mpfs_share_pagetable(filp, ptr);
		break;
	case MPFS_IOCTL_MAINTASK:
		return mpfs_init_main_task(filp, ptr);
		break;
	case MPFS_IOCTL_VSTART:
		return mpfs_update_vstart_for_main_task(filp, ptr);
		break;
	case MPFS_IOCTL_SHOWVMA:
		return mpfs_show_vma(filp, ptr);
		break;
	case MPFS_IOCTL_ENABLE_WRITE:
		return mpfs_enable_write_permission(filp, ptr);
		break;
	case MPFS_IOCTL_DISABLE_WRITE:
		return mpfs_disable_write_permission(filp, ptr);
		break;
	// case MPFS_IOCTL_ENABLE_PTE_WRITE:
	// 	return mpfs_enable_write_PTE_permission(filp, ptr);
	// 	break;
	// case MPFS_IOCTL_DOWNGRADE_HUGE:
	// 	return mpfs_downgrade_huge(filp, ptr);
	// 	break;
	// case MPFS_IOCTL_SHOW_DAX_PAGE_TABLE:
	// 	return mpfs_show_dax_page_table_ioctl(filp, ptr);
	// 	break;
	case MPFS_IOCTL_SHOW_REAL_PAGE_TABLE:
		return mpfs_show_real_page_table_ioctl(filp, ptr);
		break;
	default:
		return -1;
	}
}

#endif

MODULE_AUTHOR("Intel Corporation");
MODULE_LICENSE("GPL v2");
module_init(dax_init);
module_exit(dax_exit);
MODULE_ALIAS_DAX_DEVICE(0);
