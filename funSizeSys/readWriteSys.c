#include "readWriteSys.h"
#include "../sizes.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void readSys (int offset, void* data, int size) {
  int fd = open (FILESYS, O_RDWR, 660);
  if (fd == -1) {
    printf("File '%s' Doesn't Exist\n1- Run 'make'\n2- Run './PORTAL'\n", FILESYS);
    exit(0);
  }

  lseek(fd, offset, SEEK_SET);
  read(fd, data, size);
  close(fd);
}

void writeSys (int offset, const void* data, int size) {
  int fd = open (FILESYS, O_RDWR, 660);
  if (fd == -1) {
    printf("File '%s' Doesn't Exist\n1- Run 'make'\n2- Run './PORTAL'\n", FILESYS);
    exit(0);
  }

  lseek(fd, offset, SEEK_SET);
  write(fd, data, size);
  close(fd);
}
