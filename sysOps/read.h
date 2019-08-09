#include "../sizes.h"
#include "../sysStructs/iNode.h"

#include <math.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

/*
 * Function:  read_t
 * -----Read bytes from file by it id (inode id)
 *
 *  id: id of inode associated with those blocks where we want to read
 *  offset: the number of byte from which we start reading
 *  buf: the buffer for information weread
 *  count: the number of bytes to read
 *
 * Returns: The number of bytes read
 *           returns -1 on error (if file didn't exist or it is a dir)
 */

int read_t(int id, int offset, void* buf, int count) {
  struct Inode iNode = getInode(id);
  if (iNode.id != id || iNode.numOfFiles != 0) {
    return -1;
  }

  if (offset >= iNode.size) {
    return 0;
  }

  int block = (int) floor (offset / BLOCK_SIZE);

  int fd = open (FILESYS, O_RDWR, 660);
  int readCnt;
  int totalRead = 0;
  int blockIdx = 0;

  while(count > 0) {
    if (block == 0) {
      if (iNode.direct[0] == -1) {
        printf("Error: No direct block 1\n");
	      return totalRead;
      } else {
        blockIdx = iNode.direct[0];
      }
    } else if (block == 1) {
      if (iNode.direct[1] == -1) {
        printf("Error: No direct block 2\n");
	      return totalRead;
      } else {
        blockIdx = iNode.direct[1];
      }
    } else if (block >= 2) {
      if (iNode.indirect == -1) {
        printf("Error: No indirect block\n");
        return totalRead;
      } else {
        lseek(fd, DATA_OFFSET + iNode.indirect * BLOCK_SIZE + (block) * sizeof(int), SEEK_SET);
        read(fd, (void *) &blockIdx, sizeof(int));
      }
    }

    if (offset > 0) {
      offset = offset - (block * BLOCK_SIZE);
    }

    lseek(fd, DATA_OFFSET + blockIdx * BLOCK_SIZE + offset, SEEK_SET);

    if (count < BLOCK_SIZE-offset) {
      readCnt = count;
      count -= readCnt;
    } else {
      readCnt = BLOCK_SIZE-offset;
      count -= readCnt;
    }

    read(fd, buf+totalRead, readCnt);
    totalRead += readCnt;
    if (count > 0) {
      offset = 0;
      block++;
    }
  }

  close(fd);
  return totalRead;
}
