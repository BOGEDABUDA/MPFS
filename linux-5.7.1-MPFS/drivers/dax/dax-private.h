/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright(c) 2016 Intel Corporation. All rights reserved.
 */
#ifndef __DAX_PRIVATE_H__
#define __DAX_PRIVATE_H__

#include <linux/device.h>
#include <linux/cdev.h>

/* private routines between core files */
struct dax_device;
struct dax_device *inode_dax(struct inode *inode);
struct inode *dax_inode(struct dax_device *dax_dev);
int dax_bus_init(void);
void dax_bus_exit(void);

/**
 * struct dax_region - mapping infrastructure for dax devices
 * @id: kernel-wide unique region for a memory range
 * @target_node: effective numa node if this memory range is onlined
 * @kref: to pin while other agents have a need to do lookups
 * @dev: parent device backing this region
 * @align: allocation and mapping alignment for child dax devices
 * @res: physical address range of the region
 * @pfn_flags: identify whether the pfns are paged back or not
 */
struct dax_region {
	int id;
	int target_node;
	struct kref kref;
	struct device *dev;
	unsigned int align;
	struct resource res;
	unsigned long long pfn_flags;
};

/**
 * struct dev_dax - instance data for a subdivision of a dax region, and
 * data while the device is activated in the driver.
 * @region - parent region
 * @dax_dev - core dax functionality
 * @target_node: effective numa node if dev_dax memory range is onlined
 * @dev - device core
 * @pgmap - pgmap for memmap setup / lifetime (driver owned)
 * @dax_mem_res: physical address range of hotadded DAX memory
 */
struct dev_dax {
	struct dax_region *region;
	struct dax_device *dax_dev;
	int target_node;
	struct device dev;
	struct dev_pagemap pgmap;
	struct resource *dax_kmem_res;
};

static inline struct dev_dax *to_dev_dax(struct device *dev)
{
	return container_of(dev, struct dev_dax, dev);
}

//! /*MPFS structures*/

// #define MPFS_KERNEL_DEBUG

#ifdef MPFS_KERNEL_DEBUG
#define MPFS_PRINT(fmt, ...) printk(KERN_INFO fmt, ##__VA_ARGS__)
#else
#define MPFS_PRINT(fmt, ...)
#endif

#define MPFS //! the key to enable MPFS

#ifdef MPFS

#define MAP_SIZE ((uint64_t)0x1000 << 29) // 2TB
#define PGD_ALIGNED ((uint64_t)1 << 39) // 512GB
#define NUMOF_SHARED_PGDS ((MAP_SIZE) / (PGD_ALIGNED)) //4
#define MPFS_UNINIT 0 //indicate that MPFS is not initialized
#define MPFS_READY 99 //indicate that MPFS shared pgds is already initialized.
#define MPFS_ENABLED                                                           \
	100 //indicate that MPFS is enabled by copying the shared pgds from main task
#define MPFS_SIZEOF_DUMP_REGION (((uint64_t)1 << 30) * 6) //6GB
#define MPFS_UFS_ENABLED 123456
#define MPFS_DEUBGFS_FS_SIZE ((uint64_t)1 << 20) //1MB

struct mpfs_runtime {
	struct task_struct *main_task;
	pid_t pid_of_main_task;
	unsigned long vstart_of_main_task;
	volatile uint8_t mpfs_status; // indicate whether MPFS is ready or not
	pgd_t shared_pgds[NUMOF_SHARED_PGDS]; // store the pgd entries to be shared
	bool PT_reset_enable; // determine whether the dump region and DRAM page table should be reset
	struct vm_area_struct *
		main_vma; // used to replace the normal processes' vma with main task's vma
	size_t numof_installed_processes; // number of processes that have installed MPFS
};
typedef struct mpfs_runtime mpfs_runtime_t;

// all available numbers of pages for pswap
enum dax_pswap_npgs_types {
	one_PTE = 1,
	eight_PTE = 8,
	sixtyfour_PTE = 64,
	one_PMD = 512,
	eight_PMD = 4096,
	sixtyfour_PMD = 32768,
	one_PUD = 262144,
	eight_PUD = 2097152,
	sixtyfour_PUD = 16777216,
	one_PGD = 134217728,
};

#define ATOMIC_PSWAP_LOG_MAIGC_WORD 0x5242
// 8B log for atomic pswap
struct dax_pswap_log {
	uint16_t magic_word; // identify whether it is a valid entry
	uint8_t status; // indicate the status of the pswap
	uint8_t npgs_enum_index; // number of pages to be swapped
	uint32_t usecond_index; // (usecond - current->mpfs_vstart)/PAGE_SIZE
	// we do not need to store the ufirst_index
	// because this log will be stored within the range of ufirst_index
};
typedef struct dax_pswap_log dax_pswap_log_t;

enum dax_pswap_status {
	PSWAP_ONGOING = 0,
	PSWAP_UFIRST = 1, // entries have been copied to usecond
	PSWAP_USECOND = 2, // entries in ufirst have been zeroed
};

enum dax_ioctl_types {
	DAX_IOCTL_INIT = 16,
	DAX_IOCTL_READY,
	DAX_IOCTL_RESET,
	DAX_IOCTL_PSWAP,
	DAX_IOCTL_PREFAULT,
	DAX_IOCTL_COW,
	DAX_IOCTL_COPYTEST,
	//!MPFS
	MPFS_IOCTL_PTSHARE,
	MPFS_IOCTL_MAINTASK,
	MPFS_IOCTL_VSTART,
	MPFS_IOCTL_SHOWVMA,
	MPFS_IOCTL_ENABLE_WRITE,
	MPFS_IOCTL_DISABLE_WRITE,
	MPFS_IOCTL_ENABLE_PTE_WRITE,
	MPFS_IOCTL_DOWNGRADE_HUGE,
	MPFS_IOCTL_SHOW_DAX_PAGE_TABLE,
	MPFS_IOCTL_SHOW_REAL_PAGE_TABLE
	//!MPFS
};

//! original ctFS structures

#define PSWAP_HUGE

typedef unsigned long relptr_t;
#define PSWAP_DEBUG 1
#define DAX_MASTER_MAGIC_WORD "ROBIN's PSWAP DAX V524"
#define DAX_PSWAP_FAST_PGS 128
#define DAX_PSWAP_MASTER_PGS 16
#define DAX_PSWAP_PER_PAGE (PTRS_PER_PTE / 4)
#define DAX_PSWAP_PER_MASTER (PTRS_PER_PMD * DAX_PSWAP_PER_PAGE)
#define DAX_PSWAP_MAX_PGS (DAX_PSWAP_MASTER_PGS * DAX_PSWAP_PER_MASTER)
#define DAX_PSWAP_SHIFT ((uint64_t)0x01 << 29) //512MB

#define DAX_PCOW_MAX_FRAMES 16

#define DAX_PSWAP_STEP1 1 /* we've allocated swap frame, nothing harmful */
#define DAX_PSWAP_STEP2 2 /* we've finished staging swap pairs */
#define DAX_PSWAP_STEP3 3 /* we've done updating the dax page table */
#define DAX_PSWAP_NORMAL 0

#define DAX_MPK_DEFAULT 0
#define DAX_MPK_META 1
#define DAX_MPK_FILE 2

//? Relative Address-->REL
//? Absolute Address-->ABS
#define DAX_REL2ABS(offset)                                                    \
	((void *)((void *)(rt->start) + (unsigned long)offset))
#define DAX_ABS2REL(addr) ((relptr_t)((relptr_t)addr - (relptr_t)(rt->start)))
#define DAX_REL2PHY(rel) (rt->start_paddr + (rel))
#define DAX_REL2PFN(rel) (((rel) + rt->start_paddr) >> PAGE_SHIFT)
#define DAX_HUGEREL2PFN(rel) (((rel) + rt->start_paddr) >> PAGE_SHIFT)
#define MASTER_NOT_INIT(master)                                                \
	memcmp(master->magic_word, dax_master_magic_word, 64)
#define DAX_PTEP_OFFSET(ptep, offset)                                          \
	(ptep + ((offset & ~PMD_MASK) >> PAGE_SHIFT))

/* HUGE page store
 * in PMD entry
 * if the last bit is 1
 * it's huge page
 * the vlaue is the starting
 * relptr_t of 512 consecutive pages
 */

#define DAX_IF_HUGE(value) ((relptr_t)0b01 & (value))
#define DAX_SET_HUGE(value) ((value) | (relptr_t)0b01)
#define DAX_HUGE2REL(value)                                                    \
	((relptr_t)(value) >> 1 << 1) // reset the last bit as 0

struct dax_swap_frame {
	unsigned long first;
	unsigned long first_p;
	unsigned long second;
	unsigned long second_p;
};
typedef struct dax_swap_frame dax_swap_frame_t;

enum dax_ioctl_pcow_flag {
	PCOW_NONE = 0,
	PCOW_MAP,
	PCOW_UNMAP,
};
struct dax_ioctl_pcow {
	unsigned long src;
	unsigned long dest;
	unsigned long size;
	enum dax_ioctl_pcow_flag flag;
};
typedef struct dax_ioctl_pcow dax_ioctl_pcow_t;

struct dax_ioctl_pswap {
	unsigned long ufirst;
	unsigned long usecond;
	unsigned long npgs;
	unsigned long flag;
};
typedef struct dax_ioctl_pswap dax_ioctl_pswap_t;

struct dax_ioctl_prefault {
	void *addr;
	unsigned long n_pmd;
};
typedef struct dax_ioctl_prefault dax_ioctl_prefault_t;

struct dax_runtime {
	struct dax_master_page *master;
	unsigned long num_pages; //! number of normal pages in the dax device
	phys_addr_t start_paddr; //! start of physical address
	// unsigned long vaddr_base; //! start of virtual address in user space

	unsigned long *pswap_temp;
	/* 
		 * pointers in the current session
		 * Can directly dereference
		 */
	unsigned long *bitmap; //! manage the allocation of persistent memory
	void *start; //! the start virtual address of the dax device in kernel space

#ifdef MPFS
	void *dump_region; //! start of the dump region, MPFS only use dump region to persist PTEs
	size_t meta_size; //! size of the metadata including master page, bitmap, dump region
#else
	unsigned long *pgd; //! pointer to the persistent page table
#endif

	// /* used for COW */
	// dax_cow_frame_t *pcow_frame[DAX_PCOW_MAX_FRAMES];
	// enum dax_ioctl_pcow_flag pcow_flag;

	unsigned long *current_dax_ptep; //! has not been used in current version
	unsigned long current_pfn_flags; //! has not been used in current version
	struct vm_area_struct
		*current_vma; //! has not been used in current version

	unsigned char mpk[3];
};
typedef struct dax_runtime dax_runtime_t;

typedef unsigned long relptr_t;
struct dax_master_page {
	char magic_word[64];
	unsigned long num_pages;
	relptr_t bm_start_offset;
	relptr_t pswap_fast_pg;

	//per segment
	relptr_t pgd_offset;
	unsigned long as_vpages;

	unsigned long pswap_padding[2];
	/* used for pswap staging area
		 * Need to pad to cache line
		 */
	unsigned char pswap_state;
	unsigned long pswap_npgs;
	dax_swap_frame_t pswap_frame;
	struct dax_runtime rt;
};
typedef struct dax_master_page dax_master_page_t;

struct dax_ioctl_init {
	// to kernel: virtual memory size
	unsigned long size;
	// from kernel: physical space total
	unsigned long space_total;
	// from kernel: physical space remaining
	unsigned long space_remain;
	// from kernel: protection tag for metadata
	unsigned char mpk_meta;
	// from kernel: protection tag for file data
	unsigned char mpk_file;
	// from kernel: protection tag for default
	unsigned char mpk_default;
};
typedef struct dax_ioctl_init dax_ioctl_init_t;

#endif

void mpfs_uninstall_shared_PGD(struct task_struct *task);
#endif
