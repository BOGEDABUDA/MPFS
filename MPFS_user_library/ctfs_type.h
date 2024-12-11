#ifndef CTFS_TYPE_H
#define CTFS_TYPE_H
#include <assert.h>
#include <dirent.h>
#include <errno.h>
#include <error.h>
#include <fcntl.h>
#include <inttypes.h>
#include <linux/falloc.h>
#include <linux/magic.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/statfs.h>
#include <sys/statvfs.h>
#include <sys/types.h>
#include <sys/vfs.h>
#include <time.h>
#include <unistd.h>
#include <x86intrin.h>

#include "ctfs_config.h"
#include "ctfs_util.h"
#include "lib_dax.h"

#ifdef __GNUC__
#  define likely(x) __builtin_expect((x), 1)
#  define unlikely(x) __builtin_expect((x), 0)
#else
#  define likely(x) (x)
#  define unlikely(x) (x)
#endif

/***********************************************
 * clear cache utility
 ***********************************************/
#define cache_wb_one(addr) _mm_clwb(addr)

#define cache_wb(target, size)                                                 \
  for (size_t i = 0; i < size; i += 64)                                        \
  {                                                                            \
    _mm_clwb((void*)target + i);                                               \
  }                                                                            \
  _mm_clwb((void*)target + size - 1);

/* Convensions */
#define PAGE_MASK (~((uint64_t)0x0fff))

typedef uint64_t index_t;
typedef uint64_t relptr_t;  // relative pointer
typedef int8_t pgg_level_t;

typedef volatile char mutex_lock_t;

#ifdef PTHREAD_RWLOCK

typedef struct RWLOCK_T
{
  pthread_rwlock_t prwlock;
  uint8_t initialized;
} rwlock_t;

#else
#  ifdef DIY_MUTEX

typedef struct RWLOCK_T
{
  volatile mutex_lock_t inter_lock;
  volatile mutex_lock_t rwlock;
  volatile uint16_t reader;
} rwlock_t;

#  else

typedef struct
{
  sem_t mutex;
  sem_t wrt;
  int read_count;
} rwlock_t;

#  endif
#endif

#define CAS(ptr, oldval, newval)                                               \
  (__sync_bool_compare_and_swap(ptr, oldval, newval))
#define ATOMIC_PLUS(a, b)                                                      \
  atomic_fetch_add_explicit(&(a), b, memory_order_relaxed)

#define CT_ABS2REL(addr) (((relptr_t)(addr)) - ct_rt.base_addr)
#define CT_REL2ABS(addr) ((void*)((addr) + ct_rt.base_addr))

/* end of Convensions */

/******************************************
 * In-pmem structures
 ******************************************/

/* inode. No need for block map
 * 128B. Provides same functionality as ext4
 */
struct ct_inode
{
  // 64-bit fields
  ino_t i_number;
  size_t i_size;
  relptr_t i_block;
  size_t i_ndirent;  // indicate the size of inode table
  nlink_t i_nlink;

  // 32-bit fields

  rwlock_t i_rwlock __attribute__((aligned(64)));
  uid_t i_uid;
  gid_t i_gid;
  mode_t i_mode;
  // 16-bit fields
  pgg_level_t i_level;
  char i_finish_swap;

  // times
  struct timespec i_atim; /* Time of last access */
  struct timespec i_mtim; /* Time of last modification */
  struct timespec i_ctim; /* Time of last status change */
  time_t i_otim;          /* Time of last open */

  uint32_t i_hash; /* hash value of the path */
};
typedef struct ct_inode ct_inode_t;
typedef ct_inode_t* ct_inode_pt;

struct ct_dirent
{
  __ino64_t d_ino;
  __off64_t d_off;
  unsigned short int d_reclen;
  unsigned char d_type;
  char d_name[CT_MAX_NAME + 1];
};
typedef struct ct_dirent ct_dirent_t;
typedef ct_dirent_t* ct_dirent_pt;

/******************************************
 * In-RAM structures
 ******************************************/

#endif