// Description: Test the MPFS file system. Show how to use MPFS

#include <errno.h>
#include <fcntl.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{
  int fd = open("/mnt/MPFS/test", O_RDWR | O_CREAT, S_IRWXU);
  char buf[256];
  // struct stat* stat_buf = malloc(sizeof(struct stat));
  // fstat(fd, stat_buf);
  write(fd, "Here is an example of how MPFS works. Welcome to MPFS.\0", 55);
  // printf("%s\n", strerror(errno));
  // lseek(fd, 0, SEEK_SET);
  read(fd, buf, 55);
  printf("buf: %s\n", buf);
  close(fd);
}