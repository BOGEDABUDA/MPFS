#ifndef _MPFS_MAIN_TASK_
#define _MPFS_MAIN_TASK_

#define MPFS_SPECIFIC

#ifdef MPFS_SPECIFIC
#  define MPFS_VMSTART ((void*)((uint64_t)0x01 << 39))
#  define MPFS_TRY_TIMES 512
// #  define MPFS_DEBUG
#endif

#define MPFS_PHYS_RES ((uint64_t)0x1 << 40)  // 512GB
#define NORMAL_PAGE_SIZE ((uint64_t)0x1 << 12)
#define HUGE_PAGE_SIZE ((uint64_t)0x1 << 21)
// #define PREFAULT_SIZE ((uint64_t)0x1 << 36)
#define PGD_SIZE ((uint64_t)0x1 << 39)

#define MAP_PGD_ALIGNED 0x10000000  //! MPFS-specific FLAG

int mpfs_generate_page_table_by_prefault();  // build the subtree of page table
                                             // for MPFS
int mpfs_init_main_task();  // record the four PGD entries of the subtree (root
                            // of subtree)
#endif