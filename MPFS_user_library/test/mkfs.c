#include "../ctfs.h"
#include "../ctfs_runtime.h"
#include "../mpfs_main_task.h"
#include "../mpfs_parse.h"
#include <sys/ioctl.h>
#include <sys/mman.h>  //for mpk

int main(int argc, char** argv)
{
#ifdef MPFS_DEBUG
  MPFS_PRINT("Please attach PID of initial process:%d\n", getpid());
  char anything[10];
  scanf("%s", anything);
  MPFS_PRINT("Continue\n");
#endif

  int flag = 0;
  if (argc > 1)
  {
    if (atoi(argv[1]))
    {
      flag |= CTFS_MKFS_FLAG_RESET_DAX;
      MPFS_PRINT("Set to reset dax\n");
    }
  }
  int ret = ctfs_mkfs(flag);
  if (ret)
  {
    MPFS_ERROR("Failed to format!!\n");
    return -1;
  }
  MPFS_PRINT("ctFS formated!\n");
  return 0;
}
