#define _GNU_SOURCE
#include "lib_dax.h"
#include "ctfs_runtime.h"
#include "ctfs_type.h"
#include "mpfs_main_task.h"
#include "mpfs_parse.h"
#include <fcntl.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

// #define CTFS_MULTI_PROCESS_SYNC

#ifdef RAM_VER
#  include "ctfs_type.h"
#endif

extern ct_runtime_t ct_rt;

int dax_fd = -1;
// pthread_spinlock_t dax_lock;

void dax_stop_access(int key)
{
  pkey_set(key, PKEY_DISABLE_ACCESS);
}

void dax_stop_write(int key)
{
  pkey_set(key, PKEY_DISABLE_WRITE);
}

void dax_grant_access(int key)
{
  pkey_set(key, 0);
}

long dax_reset(const char* path, uint64_t dax_size)
{
  dax_fd = open(path, O_RDWR);
  if (dax_fd == -1)
  {
    perror("DAX RESET: open");
    return -1;
  }
  // reset the magic number of MPFS and reset some flag of MPFS to make MPFS
  // rebuild them in the foll
  long ret = ioctl(dax_fd, DAX_IOCTL_RESET, (uint64_t)0x0);

  close(dax_fd);
  dax_fd = -1;
  return ret;
}

long dax_pswap(dax_ioctl_pswap_t* frame)
{
#ifdef RAM_VER
  memcpy((void*)(frame->ufirst), (void*)(frame->usecond), frame->npgs * 4096);

  cache_wb((void*)frame->ufirst, frame->npgs * 4096);
  return 0;
#else
  long ret = ioctl(dax_fd, DAX_IOCTL_PSWAP, (uint64_t)frame);
  // pthread_spin_unlock(&dax_lock);
  return ret;
#endif
}

long dax_prefault(dax_ioctl_prefault_t* frame)
{
  return ioctl(dax_fd, DAX_IOCTL_PREFAULT, (uint64_t)frame);
}

long dax_init(dax_ioctl_init_t* frame)
{
  long ret = ioctl(dax_fd, DAX_IOCTL_INIT, (uint64_t)frame);
  return ret;
}

long dax_sync_write_lock(void)
{
  dax_ioctl_init_t frame;
  long ret = ioctl(dax_fd, DAX_SYNC_WRITE_LOCK, (uint64_t)&frame);
  return ret;
}

long dax_sync_write_unlock(void)
{
  dax_ioctl_init_t frame;
  long ret = ioctl(dax_fd, DAX_SYNC_WRITE_UNLOCK, (uint64_t)&frame);
  return ret;
}

long dax_sync_read_lock(void)
{
  dax_ioctl_init_t frame;
  long ret = ioctl(dax_fd, DAX_SYNC_READ_LOCK, (uint64_t)&frame);
  return ret;
}

long dax_sync_read_unlock(void)
{
  dax_ioctl_init_t frame;
  long ret = ioctl(dax_fd, DAX_SYNC_READ_UNLOCK, (uint64_t)&frame);
  return ret;
}

long dax_ready()
{
  int ret;
  ioctl(dax_fd, DAX_IOCTL_READY, &ret);
  return ret;
};

void* dax_start(const char* path, dax_ioctl_init_t* frame)
{
  long dax_ret = 0;
  dax_fd = open(path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
  // perror("dax start: open");
  MPFS_PRINT("dax_fd: %d\n", dax_fd);
  if (dax_fd == -1)
  {
    perror("DAX START: open");
    printf("dax_fd: %d\n", dax_fd);
    return NULL;
  }

#ifdef MPFS_SPECIFIC
  void* ret = mmap(NULL, MAP_SIZE, PROT_WRITE | PROT_READ,
                   MAP_SHARED | MAP_PGD_ALIGNED, dax_fd, 0);
  MPFS_PRINT("mmap with MAP_PGD_ALIGNED: %p\n", ret);
#else
  void* ret =
    mmap(NULL, MAP_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, dax_fd, 0);
#endif
  if (ret == NULL)
  {
    return NULL;
  }

  dax_ret = dax_init(frame);

  if (!dax_ret)
    MPFS_PRINT("dax init success!\n");
  else
    MPFS_PRINT("dax init fail\n");

  return ret;
};

void dax_end()
{
  if (dax_fd != -1)
  {
    close(dax_fd);
    dax_fd = -1;
  }
}

void dax_test_cpy(void* buf)
{
  ioctl(dax_fd, DAX_IOCTL_COPYTEST, (unsigned long)buf);
}
