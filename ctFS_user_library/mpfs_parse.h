#ifndef __MPFS_PARSE_H__
#define __MPFS_PARSE_H__

#define MPFS_DEFAULT_MOUNT_POINT "/mnt/MPFS"
const char* mpfs_parse_path(const char* path);

#define MPFSINFO_NONE "\033[m"
#define MPFSINFO_RED "\033[0;32;31m"
#define MPFSINFO_GREEN "\033[0;32;32m"
#define MPFSINFO_BLUE "\033[0;32;34m"
#define MPFSINFO_YELLOW "\033[1;33m"

#define likely(x) __builtin_expect((x), 1)
#define unlikely(x) __builtin_expect((x), 0)

// #define MPFS_DEBUG

#ifdef MPFS_DEBUG
#  define MPFS_PRINT(format, ...)                                              \
    printf(MPFSINFO_YELLOW format MPFSINFO_NONE, ##__VA_ARGS__)
#  define MPFS_ERROR(format, ...)                                              \
    printf(MPFSINFO_RED format MPFSINFO_NONE, ##__VA_ARGS__)
#else
#  define MPFS_PRINT(format, ...)
#  define MPFS_ERROR(format, ...)
#endif

#endif