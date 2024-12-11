#include "mpfs_test.h"
#include "ctfs.h"
#include "ctfs_runtime.h"
#include "lib_dax.h"
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
extern ct_runtime_t ct_rt;

// multi-processes write to the same file
void experiment_0(char* root)
{
  char path[100];
  sprintf(path, "%s/append_test.txt", root);
  int fd = open(path, O_RDWR | O_CREAT, 0666);
  // i < 520 for 2080KB
  // i < 5200 for 20800KB
  // i < 52000 for 208MB
  for (int i = 0; i < 52000; i++)
  {
    // printf("%d-i: %d\n", getpid(), i);
    write(fd, data, 4096);
  }

  close(fd);
}

// multi-processes write to the different files
void experiment_1(char* root, char* index)
{
  char path[100];
  char buf[4096];
  sprintf(path, "%s/append_test_%d.txt", root, *index);

  // WRITE
  int fd = open(path, O_RDWR | O_CREAT, 0666);
  // i < 520 for 2080KB
  // i < 5200 for 20800KB
  // i < 52000 for 208MB
  for (int i = 0; i < 52000; i++)
  {
    write(fd, data, 4096);
  }
  close(fd);
}

// multi-processes read to the same file
void experiment_2(char* root)
{
  char path[100];
  char buf[4096];

  sprintf(path, "%s/append_test.txt", root);

  int fd = 0;

  // READ
  fd = open(path, O_RDWR | O_CREAT, 0666);
  for (int i = 0; i < 52000; i++)
  {
    int ret = read(fd, buf, 4096);
    // if (ret != 4096)
    // {
    //   printf("read error: %d\n", ret);
    // }
  }
  close(fd);
}

// multi-process read to different files
void experiment_3(char* root, char* index)
{
  static int flag = 0;
  char path[100];
  char buf[4096];
  sprintf(path, "%s/append_test_%d.txt", root, *index);

  int fd = 0;
  // i < 520 for 2080KB
  // i < 5200 for 20800KB
  // i < 52000 for 208MB

  // READ
  fd = open(path, O_RDWR | O_CREAT, 0666);
  for (int i = 0; i < 52000; i++)
  {
    int ret = read(fd, buf, 4096);
    // if (ret != 4096)
    // {
    //   printf("read error: %d\n", ret);
    // }
  }
  close(fd);
}

int main(int argc, char** argv)
{
  // printf("read/write pid: %d\n", getpid());
  // printf("select: %c\n", *argv[2]);
  switch (*argv[2])
  {
    case '0':
      // printf("Process %d write to the same file\n", getpid());
      experiment_0(argv[1]);  // multi-processes WRITE to the SAME file
      // printf("Process %d write to the same file done\n", getpid());
      break;
    case '1':
      // printf("Process %d write to the different file\n", getpid());
      experiment_1(argv[1],
                   argv[3]);  // multi-processes WRITE to the DIFFERENT files
      // printf("Process %d write to the different file done\n", getpid());
      break;
    case '2':
      // printf("Process %d read to the same file\n", getpid());
      experiment_2(argv[1]);  // multi-processes READ to the SAME file
      // printf("Process %d read to the same file done\n", getpid());
      break;
    case '3':
      // printf("Process %d read to the different file\n", getpid());
      experiment_3(argv[1],
                   argv[3]);  // multi-process READ to DIFFERENT files
      // printf("Process %d read to the different file done\n", getpid());
      break;
    default: printf("Invalid input\n"); break;
  }
  return 0;
}