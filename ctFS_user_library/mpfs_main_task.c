#include "mpfs_main_task.h"
#include "ctfs.h"
#include "ctfs_config.h"
#include "ctfs_runtime.h"
#include "lib_dax.h"
#include <errno.h>
#include <malloc.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>

extern ct_runtime_t ct_rt;

// void sighandler(int signum)
// {
//   printf("Signal:%d\n", signum);
//   perror("Signal");
//   exit(1);
// }

// MPFS inited by this function
static __attribute__((constructor(110))) void init_method(void)
{
  // signal(SIGBUS, sighandler);
  int fd = open("main_task_info", O_CREAT | O_RDWR, 0666);
  write(fd, "main task\n", 10);
  write(fd, "ctfs_init_for_main_task\n", 24);
  close(fd);
  ctfs_init_for_main_task(0);
}

int main()
{
  // the main task cannot exit
  int fd = open("main_task_info", O_CREAT | O_RDWR | O_APPEND, 0666);
  // write current time to main_task_info
  char time[100];
  time_t t;
  sprintf(time, "%s", ctime(&t));
  write(fd, time, strlen(time));
  close(fd);
  while (1)
  {
    pause();
  }

  return 0;
}