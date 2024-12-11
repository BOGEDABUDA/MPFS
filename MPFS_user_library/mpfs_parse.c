#include "mpfs_parse.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

char mpfs_mount_point[] = MPFS_DEFAULT_MOUNT_POINT;
__thread char resolved_path[4096];  // for realpath to stroe the resolved path

const char* mpfs_parse_path(const char* path)
{
  // realpath(path, resolved_path);
  // Check if the mpfs_mount_point exists at the beginning of the path
  const char* p = path;
  const char* q = mpfs_mount_point;
  while (*q)
  {
    if (*p != *q)
    {
      return NULL;
    }
    p++;
    q++;
  }
  return p;
}