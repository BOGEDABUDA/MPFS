#include "ctfs_pgg.h"
#include "ctfs_runtime.h"
#include "ctfs_type.h"
#include <assert.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static const ct_inode_t default_inode = {.i_number  = 0,
                                         .i_size    = 0,
                                         .i_block   = 0,
                                         .i_ndirent = 0,
                                         .i_nlink   = 1,
                                         .i_uid     = 0,
                                         .i_gid     = 0,
                                         .i_mode  = S_IRWXU | S_IRWXG | S_IRWXO,
                                         .i_level = PGG_LVL_NONE

};

void inode_rw_lock(index_t inode_n)
{
  dax_grant_access(ct_rt.mpk[DAX_MPK_DEFAULT]);
  bitlock_acquire(ct_rt.super_blk->inode_rw_lock, inode_n % CT_INODE_RW_SLOTS);
}

void inode_rw_unlock(index_t inode_n)
{
  dax_grant_access(ct_rt.mpk[DAX_MPK_DEFAULT]);
  bitlock_release(ct_rt.super_blk->inode_rw_lock, inode_n % CT_INODE_RW_SLOTS);
}

void inode_rt_lock(index_t inode_n)
{
  dax_grant_access(ct_rt.mpk[DAX_MPK_DEFAULT]);
  bitlock_acquire(ct_rt.super_blk->inode_rt_lock,
                  inode_n % CT_INODE_BITLOCK_SLOTS);
}

void inode_rt_unlock(index_t inode_n)
{
  dax_grant_access(ct_rt.mpk[DAX_MPK_DEFAULT]);
  bitlock_release(ct_rt.super_blk->inode_rt_lock,
                  inode_n % CT_INODE_BITLOCK_SLOTS);
}

#ifdef PTHREAD_RWLOCK

void check_alignment(rwlock_t* lock)
{
  printf("Address of rwlock in Process %d: %p - %d\n", getpid(),
         (void*)&lock->prwlock, lock->initialized);
  // Check if the address is 16-bit aligned
  if (((unsigned long)&lock->prwlock >> 2 << 2) ==
      (unsigned long)&lock->prwlock)
  {
    // printf("Address is 16-bit aligned\n");
  }
  else
  {
    printf("Address is not 16-bit aligned\n");
  }

  // Check if the address is 32-bit aligned
  if (((unsigned long)&lock->prwlock >> 4 << 4) ==
      (unsigned long)&lock->prwlock)
  {
    // printf("Address is 32-bit aligned\n");
  }
  else
  {
    printf("Address is not 32-bit aligned\n");
  }
}

// init rwlock
void init_rwlock(rwlock_t* lock)
{
  pthread_rwlockattr_t attr;

  // if (lock->initialized != 100)
  // {
  pthread_rwlockattr_init(&attr);
  pthread_rwlockattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);

  if (pthread_rwlock_init(&lock->prwlock, &attr) != 0)
  {
    perror("pthread_rwlock_init");
  }
  // check_alignment(lock);
  // printf("Process %d lock %p : %d\n", getpid(), &lock->prwlock,
  //  lock->initialized++);
  // }
}

// acquire write lock
void write_lock(rwlock_t* lock)
{
  // check_alignment(lock);
  int ret = pthread_rwlock_wrlock(&lock->prwlock);
  // if (ret != 0)
  //   printf("wrlock-%d: %d\n", getpid(), ret);
}

// release write lock
void write_unlock(rwlock_t* lock)
{
  // check_alignment(lock);
  int ret = pthread_rwlock_unlock(&lock->prwlock);
  // if (ret != 0)
  //   printf("wrunlock-%d: %d\n", getpid(), ret);
}

// acquire read lock
void read_lock(rwlock_t* lock)
{
  // check_alignment(lock);
  int ret = pthread_rwlock_rdlock(&lock->prwlock);
  // if (ret != 0)
  //   printf("rdlock-%d: %d\n", getpid(), ret);
}

// release read lock
void read_unlock(rwlock_t* lock)
{
  // check_alignment(lock);
  int ret = pthread_rwlock_unlock(&lock->prwlock);
  // if (ret != 0)
  //   printf("rdunlock-%d: %d\n", getpid(), ret);
}

#else
#  ifdef DIY_MUTEX
// init rwlock
void init_rwlock(rwlock_t* lock)
{
  lock->inter_lock = 0;
  lock->rwlock     = 0;
  lock->reader     = 0;
}

void mpfs_mutex_lock(mutex_lock_t* lock)
{
  while (!CAS(lock, 0, 1))
    ;
}

void mpfs_mutex_unlock(mutex_lock_t* lock)
{
  if (!CAS(lock, 1, 0))
    assert(0);
}

// acquire write lock
void write_lock(rwlock_t* lock)
{
  mpfs_mutex_lock(&lock->rwlock);
}

// release write lock
void write_unlock(rwlock_t* lock)
{
  mpfs_mutex_unlock(&lock->rwlock);
}

// acquire read lock
void read_lock(rwlock_t* lock)
{
  mpfs_mutex_lock(&lock->inter_lock);

  if (++lock->reader == 1)
    mpfs_mutex_lock(&lock->rwlock);

  mpfs_mutex_unlock(&lock->inter_lock);
}

// release read lock
void read_unlock(rwlock_t* lock)
{
  mpfs_mutex_lock(&lock->inter_lock);

  if (--lock->reader == 0)
    mpfs_mutex_unlock(&lock->rwlock);

  mpfs_mutex_unlock(&lock->inter_lock);
}

#  else
void init_rwlock(rwlock_t* lock)
{
  sem_init(&lock->mutex, 1, 1);
  sem_init(&lock->wrt, 1, 1);
  lock->read_count = 0;
}

void read_lock(rwlock_t* lock)
{
  sem_wait(&lock->mutex);
  lock->read_count++;
  if (lock->read_count == 1)
  {
    sem_wait(&lock->wrt);
  }
  sem_post(&lock->mutex);
}

void read_unlock(rwlock_t* lock)
{
  sem_wait(&lock->mutex);
  lock->read_count--;
  if (lock->read_count == 0)
  {
    sem_post(&lock->wrt);
  }
  sem_post(&lock->mutex);
}

void write_lock(rwlock_t* lock)
{
  sem_wait(&lock->wrt);
}

void write_unlock(rwlock_t* lock)
{
  sem_post(&lock->wrt);
}
#  endif
#endif

inline void inode_wb(ct_inode_pt inode)
{
  cache_wb(inode, sizeof(ct_inode_t));
}

#define INODE_LOCK_OFFSET(n) (n % CT_INODE_BITLOCK_SLOTS)

int inode_resize_lvl(ct_inode_pt inode, pgg_level_t lvl)
{
  if (inode->i_level == PGG_LVL_NONE)
  {
#ifdef CTFS_DEBUG
    printf("PGG_LVL_NONE case!\n");
#endif
    relptr_t blk   = pgg_allocate(lvl);
    inode->i_block = blk;
    inode->i_level = lvl;
    inode->i_size  = pgg_size[lvl];
    return 1;
  }
  if (inode->i_level < lvl)
  {
    // upgrade size
#ifdef CTFS_DEBUG
    printf("pswap resize!\n");
    ct_inode_t dbg_inode = *inode;
#endif
    relptr_t new            = pgg_allocate(lvl);
    dax_ioctl_pswap_t pswap = {.ufirst  = CT_REL2ABS(new),
                               .usecond = CT_REL2ABS(inode->i_block),
                               .npgs    = pgg_size[inode->i_level] >> 12};
    dax_pswap(&pswap);  //! bug here, there may be a problem in the new pswap()
#ifdef CTFS_DEBUG
    ct_inode_t dbg_inode2 = *inode;
#endif
    pgg_deallocate(inode->i_level, inode->i_block);
    inode->i_block = new;
    inode->i_size  = pgg_size[lvl];
    inode->i_level = lvl;
    cache_wb_one(inode);
    return 1;
  }
  if (inode->i_level > lvl)
  {
    // do nothing now.
  }
  return 0;
}

int inode_resize(ct_inode_pt inode, size_t size)
{
  // decide the level of file size
  pgg_level_t lvl = pgg_get_lvl(size);
#ifdef CTFS_HACK
  //! this is the reason why ctfs allocate at least 2MB without
  //! downgrading huge page
  if (lvl < PGG_LVL3)
  {
    lvl = PGG_LVL3;
  }
#endif
  int ret       = inode_resize_lvl(inode, lvl);
  inode->i_size = size;
  ct_time_stamp(&inode->i_ctim);
  ct_time_stamp(&inode->i_mtim);
  inode_wb(inode);
  return ret;
}

index_t inode_alloc()
{
  ctfs_lock_acquire(ct_rt.super_blk->inode_bmp_lock);
  if (ct_super->inode_bmp_touched == ct_super->inode_used)
  {
    // need touch more inode bmp
    memset(ct_rt.inode_bmp + (ct_super->inode_bmp_touched >> 3), 0,
           CT_PAGE_SIZE);
    ct_super->inode_bmp_touched += CT_PAGE_SIZE << 3;
  }
  int64_t res =
    find_free_bit(ct_rt.inode_bmp, CT_SIZE_MAX_INODE, ct_super->inode_hint);
  index_t ret;
  if (res == -1)
  {
    // !!!out of inode
    ctfs_lock_release(ct_rt.super_blk->inode_bmp_lock);
    return 0;
  }
  else
  {
    ret = (uint64_t)res;
  }
  set_bit(ct_rt.inode_bmp, ret);
  ct_super->inode_hint = ret;
  ct_super->inode_used++;
  cache_wb_one(&ct_super->inode_used);
  ctfs_lock_release(ct_rt.super_blk->inode_bmp_lock);
  return ret;
}

void inode_dealloc(index_t index)
{
  ctfs_lock_acquire(ct_rt.super_blk->inode_bmp_lock);
  assert(index < CT_SIZE_MAX_INODE);
  clear_bit(ct_rt.inode_bmp, index);
  ctfs_lock_release(ct_rt.super_blk->inode_bmp_lock);
}

static int inode_dir_fill(ct_inode_frame_t* frame)
{
  ct_inode_pt c = frame->current;
  inode_resize(frame->current, 2 * sizeof(ct_dirent_t));
  ct_dirent_pt cur_dirent = CT_REL2ABS(c->i_block);
  cur_dirent[0].d_ino     = c->i_number;
  strcpy(cur_dirent[0].d_name, ".");
  cur_dirent[0].d_type = DT_DIR;
  cur_dirent[1].d_ino  = frame->parent->i_number;
  strcpy(cur_dirent[1].d_name, "..");
  cur_dirent[1].d_type = DT_DIR;
  c->i_size            = 2 * sizeof(ct_dirent_t);
  c->i_ndirent         = 2;
  c->i_mode |= S_IFDIR;
#ifdef DAX_DEBUGGING
  ct_inode_t in   = *c;
  ct_dirent_t dir = *cur_dirent;
#endif
  return 0;
}

/* create all necessary inodes from the inode_start point
 * goes in with start locked
 * internal use only
 */
static int inode_create(ct_inode_frame_t* frame)
{
  ct_inode_pt c      = NULL;
  const char* cursor = frame->path;
  ino_t temp_i;
  uint32_t j;
  c = frame->inode_start;
  // ino_t start_ino = c->i_number;
  ct_dirent_pt target_dirent = NULL;
  // loop over path components
  while (1)
  {
    ct_dirent_pt cur_dirent = CT_REL2ABS(c->i_block);
    // loop over dirents
#ifdef DAX_DEBUGGING
    ct_dirent_t dir = *cur_dirent;
    // dir.d_ino = 0;
    ct_inode_t in = *c;
    // in.i_gid = 0;
#endif
    if (frame->flag & CT_INODE_FRAME_INSTALL)
    {
      j = 0;
      while (cursor[j] != '/' && cursor[j] != '\0')
      {
        j++;
      }
      if (cursor[j] == '\0')
      {
        // install inode here
        for (uint32_t i = 2;; i++)
        {
          if (i >= c->i_ndirent)
          {
            if ((i + 1) * sizeof(ct_dirent_t) > c->i_size)
            {
              // need to enlarge the data block
              inode_resize(c, (i + 1) * sizeof(ct_dirent_t));
              cache_wb(c, sizeof(ct_inode_t));
            }
            ct_time_stamp(&c->i_ctim);
            c->i_ndirent++;
            target_dirent = &cur_dirent[i];
            break;
          }
          else if (cur_dirent[i].d_ino == 0)
          {
            target_dirent = &cur_dirent[i];
            break;
          }
        }
        assert(target_dirent);
        target_dirent->d_ino = frame->current->i_number;
        strncpy(target_dirent->d_name, cursor, CT_MAX_NAME);
        target_dirent->d_type =
          (frame->current->i_mode & S_IFDIR) ? DT_DIR : DT_REG;
        cache_wb(target_dirent, sizeof(ct_dirent_t));
        inode_rt_unlock(c->i_number);
        return 0;
      }
    }
    for (uint32_t i = 2;; i++)
    {
      ct_inode_t inode_temp;
      ct_dirent_t dirent_temp;
      if ((i + 1) * sizeof(ct_dirent_t) > c->i_size)
      {
        // need to enlarge the data block
        inode_resize(c, (i + 1) * sizeof(ct_dirent_t));
        goto found_empty;
      }
      if (cur_dirent[i].d_ino == 0)
      {
      found_empty:
#ifdef CTFS_DEBUG
        inode_temp  = *c;
        dirent_temp = cur_dirent[i];
#endif
        c->i_ndirent++;
        ct_time_stamp(&c->i_ctim);
        cache_wb(c, sizeof(ct_inode_t));
        temp_i = inode_alloc();
        j      = 0;
        while (cursor[j] != '\0' && cursor[j] != '/')
        {
          cur_dirent[i].d_name[j] = cursor[j];
          j++;
          if (j > CT_MAX_NAME)
          {
            inode_dealloc(temp_i);
            return EINVAL;
          }
        }
        cur_dirent[i].d_name[j] = '\0';
        ct_time_stamp(&c->i_mtim);
        ct_time_stamp(&c->i_atim);
        cursor += j;
        while (*cursor == '/')
        {
          cursor++;
        }
        frame->parent  = c;
        c              = &ct_rt.inode_start[temp_i];
        frame->current = c;
        frame->dirent  = &cur_dirent[i];
        memcpy(c, &default_inode, sizeof(ct_inode_t));
        init_rwlock(&c->i_rwlock);
        c->i_number = temp_i;
        ct_time_stamp(&c->i_ctim);
        ct_time_stamp(&c->i_mtim);
        ct_time_stamp(&c->i_atim);
        if (*cursor == '\0')
        {
          // it's the last one. we are done.
          if (frame->i_mode & S_IFDIR)
          {
            cur_dirent[i].d_type = DT_DIR;
            inode_dir_fill(frame);
          }
          else
          {
            cur_dirent[i].d_type = DT_REG;
          }
          if (frame->flag & CT_INODE_FRAME_PARENT)
          {
            // parent requested
            if (INODE_LOCK_OFFSET(frame->parent->i_number) ==
                INODE_LOCK_OFFSET(temp_i))
            {
              frame->flag |= CT_INODE_FRAME_SAME_INODE_LOCK;
            }
            else
            {
              inode_rt_lock(temp_i);
            }
          }
          else
          {
            if (INODE_LOCK_OFFSET(frame->parent->i_number) !=
                INODE_LOCK_OFFSET(temp_i))
            {
              inode_rt_unlock(frame->parent->i_number);
              inode_rt_lock(temp_i);
            }
          }
#ifdef CTFS_DEBUG
          ct_inode_t tt = *c;
          printf("allocated at dirent #%d\n", i);
#endif
          cache_wb(c, sizeof(ct_inode_t));
          cache_wb(&cur_dirent[i], sizeof(ct_dirent_t));
          cur_dirent[i].d_ino = temp_i;
          cache_wb_one(&cur_dirent[i].d_ino);
          return 0;
        }
        else
        {
          cur_dirent[i].d_type = DT_DIR;
          inode_dir_fill(frame);

          cache_wb(c, sizeof(ct_inode_t));
          cache_wb(&cur_dirent[i], sizeof(ct_dirent_t));
          cur_dirent[i].d_ino = temp_i;
          cache_wb_one(&cur_dirent[i].d_ino);
          if (INODE_LOCK_OFFSET(frame->parent->i_number) !=
              INODE_LOCK_OFFSET(temp_i))
          {
            inode_rt_unlock(frame->parent->i_number);
            inode_rt_lock(temp_i);
          }
        }
        break;  // break loop over dirents
      }
    }
  }
}

#ifdef __x86_64__
#  define ALIGN_SIZE 8
#else
#  define ALIGN_SIZE 4
#endif
#define ALIGN_MASK (ALIGN_SIZE - 1)

uint32_t extend(uint32_t init_crc, const char* data, size_t n)
{
  uint32_t res = init_crc ^ 0xffffffff;
  size_t i;
#ifdef __x86_64__
  uint64_t* ptr_u64;
  uint64_t tmp;
#endif
  uint32_t* ptr_u32;
  uint16_t* ptr_u16;
  uint8_t* ptr_u8;

  // aligned to machine word's boundary
  for (i = 0; (i < n) && ((intptr_t)(data + i) & ALIGN_MASK); ++i)
  {
    res = _mm_crc32_u8(res, data[i]);
  }

#ifdef __x86_64__
  tmp = res;
  while (n - i >= sizeof(uint64_t))
  {
    ptr_u64 = (uint64_t*)&data[i];
    tmp     = _mm_crc32_u64(tmp, *ptr_u64);
    i += sizeof(uint64_t);
  }
  res = (uint32_t)tmp;
#endif
  while (n - i >= sizeof(uint32_t))
  {
    ptr_u32 = (uint32_t*)&data[i];
    res     = _mm_crc32_u32(res, *ptr_u32);
    i += sizeof(uint32_t);
  }
  while (n - i >= sizeof(uint16_t))
  {
    ptr_u16 = (uint16_t*)&data[i];
    res     = _mm_crc32_u16(res, *ptr_u16);
    i += sizeof(uint16_t);
  }
  while (n - i >= sizeof(uint8_t))
  {
    ptr_u8 = (uint8_t*)&data[i];
    res    = _mm_crc32_u8(res, *ptr_u8);
    i += sizeof(uint8_t);
  }

  return res ^ 0xffffffff;
}

uint32_t crc32c(const char* data, size_t n)
{
  return extend(0, data, n);
}

int inode_path2inode_fast(ct_inode_frame_t* frame)
{
  uint32_t hash = crc32c(frame->path, strlen(frame->path));
  // avoid indexing overflow
  uint32_t index =
    hash % (CT_PATH2INODE_HASH_SIZE - CT_PATH2INODE_HASH_OPEN_ADDRESS);

  // pthread_spin_lock(ct_rt.path2inode_lock);
  for (int m = 0; m < CT_PATH2INODE_HASH_OPEN_ADDRESS; m++)
  {
    // TODO: check if the inode is overflown
    if (ct_rt.path2inode[index + m] != NULL &&
        ct_rt.path2inode[index + m]->i_hash == hash)
    {
      frame->current = ct_rt.path2inode[index + m];
      // pthread_spin_unlock(ct_rt.path2inode_lock);
      return 0;
    }
  }

  // if not found, use the slow path by calling inode_path2inode()
  int ret = inode_path2inode(frame);
  for (int n = 0; n < CT_PATH2INODE_HASH_OPEN_ADDRESS; n++)
  {
    // TODO: check if the inode is overflown
    if (ct_rt.path2inode[index + n] == NULL)
    {  // success
      ct_rt.path2inode[index + n] = frame->current;
      frame->current->i_hash      = hash;
      return ret;
    }
  }
  // if the hash table is full, randomly replace one entry

  int rand_index = rand() % CT_PATH2INODE_HASH_OPEN_ADDRESS;
  ct_rt.path2inode[index + rand_index] = frame->current;
  frame->current->i_hash               = hash;
  return ret;
}

/* parse the path
 * and return the pointer
 * to its inode and its parent inode.
 * return the current with locked
 * return the parent with locked
 * if it's not NULL, otherwise unlokced
 * return NULL to parent if the current is root
 * @inode_start[in] 	starting inode. Ignored if path starts with '/'
 * @path[in]        	string of the path
 * @current[out]    	the target's inode
 * @parent[out]     	target's parent inode
 * @dirent[out]     	parent's dirent where the current locates
 * @i_mode[in]			the mode provided for last level if request for
 * create
 * @flag[inout]			flags
 * @return          0 if success, error otherwise
 */
int inode_path2inode(ct_inode_frame_t* frame)
{
  ct_inode_pt c      = NULL;  // point to the inode of current directory
  const char* cursor = frame->path;
  int ret, found;
  ino_t ino_n;

  if (cursor[0] == '/')
  {  // if it is an absolute path
    ino_n = ct_super->root_inode;
    c     = &ct_rt.inode_start[ino_n];
  }
  else
  {  // if it is a relative path and inode_start is not NULL
    assert(frame->inode_start != NULL);
    c = frame->inode_start;
  }
#ifdef CTFS_DEBUG
  ct_super_blk_t sp = *ct_super;
#endif
  while (*cursor == '/')
    cursor++;
  inode_rt_lock(c->i_number);
  ct_time_stamp(&c->i_atim);
  if (*cursor == '\0')
  {
    // request is root
    frame->current = c;
    return 0;
  }
  /* loop over each directory in the path */
  while (1)
  {
    // if ((c->i_mode & S_IFMT) != S_IFDIR)
    // {  // if it is not a directory
    //   ret = ENOTDIR;
    //   goto error;
    // }
    if ((c->i_mode & S_IXUSR) == 0)
    {  // if the user cannot access the directory
      ret = ENOEXEC;
      goto error;
    }

    uint32_t size_dirent = c->i_size / sizeof(ct_dirent_t);
    // i_size-->the size of directory file | sizeof(ct_dirent_t)-->the size of
    // a directory entry | size_dirent--> the number of entries in a directory
    // file.
    found                   = 0;
    ct_dirent_pt cur_dirent = CT_REL2ABS(c->i_block);
    /* loop over dirents in the directory */
    for (uint32_t i = 0; i < size_dirent; i++)
    {
      if (cur_dirent[i].d_ino == 0)
      {  // if the inode number is 0, it means that the directory entry is
         // empty.
        continue;
      }
      uint32_t j = 0;
#ifdef CTFS_DEBUG
      ct_dirent_t dir = cur_dirent[i];
      dir.d_ino       = 0;
#endif
      // if the inode exists, check the name
      while (1)
      {
        if (cur_dirent[i].d_name[j] == '\0')
        {
          // all matches and reach the end of current dirent's name
          if (cursor[j] == '/')
          {
            cursor += j;
            while (*cursor == '/')
            {
              cursor++;
            }
            if (*cursor == '\0')
            {  // if there is no more directory in the path
            target_reached:
              if (frame->flag & CT_INODE_FRAME_INSTALL)
              {
                // got trouble. Shouldn't exist.
                ino_t old           = cur_dirent[i].d_ino;
                cur_dirent[i].d_ino = frame->current->i_number;
                ct_rt.inode_start[old].i_nlink--;
                if (ct_rt.inode_start[old].i_level != -1)
                {
                  if (ct_rt.inode_start[old].i_block != 0)
                  {
                    pgg_deallocate(ct_rt.inode_start[old].i_level,
                                   ct_rt.inode_start[old].i_block);
                  }
                  inode_dealloc(old);
                }
                cur_dirent[i].d_type =
                  (frame->current->i_mode & S_IFDIR) ? DT_DIR : DT_REG;
                inode_rt_unlock(c->i_number);
                return 0;
              }
              if (frame->flag & CT_INODE_FRAME_PARENT)
              {
                // parent requested
                frame->parent = c;
                frame->dirent = &cur_dirent[i];
                if (INODE_LOCK_OFFSET(c->i_number) ==
                    INODE_LOCK_OFFSET(cur_dirent[i].d_ino))
                {
                  frame->flag |= CT_INODE_FRAME_SAME_INODE_LOCK;
                }
                else
                {
                  inode_rt_lock(cur_dirent[i].d_ino);
                }
              }
              else
              {
                // parent not requested
                if (INODE_LOCK_OFFSET(c->i_number) !=
                    INODE_LOCK_OFFSET(cur_dirent[i].d_ino))
                {
                  inode_rt_lock(cur_dirent[i].d_ino);
                  inode_rt_unlock(c->i_number);
                }
              }
              frame->current = &ct_rt.inode_start[cur_dirent[i].d_ino];
              return 0;
            }
            found = 1;
            if (INODE_LOCK_OFFSET(c->i_number) !=
                INODE_LOCK_OFFSET(cur_dirent[i].d_ino))
            {
              inode_rt_unlock(c->i_number);
              inode_rt_lock(cur_dirent[i].d_ino);
            }
            // the path has not reached the end, update the current inode with
            // the next dirent
            c = &ct_rt.inode_start[cur_dirent[i].d_ino];
            ct_time_stamp(&c->i_atim);
            break;  // break check the name, because the next dirent is found
          }
          if (cursor[j] == '\0')
          {
            goto target_reached;
          }
        }
        if (cursor[j] != cur_dirent[i].d_name[j])
        {
          // not match
          break;  // break check the name
        }
        j++;
      }
      if (found)
      {
        break;  // break loop over dirents in the directory
      }
    }
    if (!found)  // if the path has not reached the end and the next dirent is
                 // not found
    {
      if (frame->flag & CT_INODE_FRAME_INSTALL)
      {
        uint16_t j                 = 0;
        ct_dirent_pt target_dirent = NULL;
        while (cursor[j] != '/' && cursor[j] != '\0')
        {
          j++;
        }
        if (cursor[j] == '\0')
        {
          // install inode here
          for (uint32_t i = 2;; i++)
          {
            if (i >= c->i_ndirent)
            {
              if (i * sizeof(ct_dirent_t) > c->i_size)
              {
                // need to enlarge the data block
                inode_resize(c, i * sizeof(ct_dirent_t));
                cache_wb(c, sizeof(ct_inode_t));
              }
              ct_time_stamp(&c->i_ctim);
              c->i_ndirent++;
              target_dirent = &cur_dirent[i];
              break;
            }
            else if (cur_dirent[i].d_ino == 0)
            {
              target_dirent = &cur_dirent[i];
              break;
            }
          }
          assert(target_dirent);
          target_dirent->d_ino = frame->current->i_number;
          strncpy(target_dirent->d_name, cursor, CT_MAX_NAME);
          target_dirent->d_type =
            (frame->current->i_mode & S_IFDIR) ? DT_DIR : DT_REG;
          cache_wb(target_dirent, sizeof(ct_dirent_t));
          inode_rt_unlock(c->i_number);
          return 0;
        }
      }
      // no match dirent
      if (frame->flag & CT_INODE_FRAME_CREATE)
      {
        // create from the current inode
        frame->inode_start = c;
        frame->path        = cursor;
        return inode_create(frame);
      }
      else
      {
        // no file or dir
        inode_rt_unlock(c->i_number);
        return ENOENT;
      }
    }
  }  // end of loop over each directory component

  assert(0);
  return -1;

error:
  inode_rt_unlock(c->i_number);
  return ret;
}

void inode_set_root()
{
  ct_inode_pt c = &ct_rt.inode_start[1];
  memcpy(c, &default_inode, sizeof(ct_inode_t));
  init_rwlock(&c->i_rwlock);
  c->i_number = 1;
  c->i_block  = pgg_mkfs();  //! bug here

#ifdef CTFS_HACK
  c->i_level = PGG_LVL3;
#else
  c->i_level = PGG_LVL0;
#endif

  c->i_size = 2 * sizeof(ct_dirent_t);
  c->i_mode |= S_IFDIR;
  c->i_ndirent            = 2;
  ct_dirent_pt cur_dirent = CT_REL2ABS(c->i_block);
  cur_dirent[0].d_ino     = 1;
  strcpy(cur_dirent[0].d_name, ".");
  cur_dirent[0].d_type = DT_DIR;
  cur_dirent[1].d_ino  = 1;
  strcpy(cur_dirent[1].d_name, "..");
  cur_dirent[1].d_type = DT_DIR;
  ct_time_stamp(&c->i_ctim);
  ct_time_stamp(&c->i_mtim);
  ct_time_stamp(&c->i_atim);
  cache_wb(c, sizeof(ct_inode_t));
  cache_wb(cur_dirent, 2 * sizeof(ct_dirent_t));
}
