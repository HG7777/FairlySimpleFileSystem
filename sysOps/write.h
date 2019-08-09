#include "../sizes.h"
#include "../sysStructs/superBoulder.h"

int write_t(int id, int offset, void *buf, int count);

int write_t(int id, int offset, void *buf, int count) {
  struct Inode inode = getInode(id);

  if (inode.id != id || inode.numOfFiles != 0) {
    printf("File is Directory : Or : File Doesn't Exist\n");
    return -1;
  }

  if (offset >= BLOCK_SIZE * inode.numOfBlocks) {
    printf("Offset is At or Past End of File\n");
    return 0;
  }

  int blk;
  for (int i = 0; i < MAX_DATA_BLOCK; i++) {
    if (offset < BLOCK_SIZE*(i+1)) {
      blk = i;
      if (blk > inode.numOfBlocks) {
        printf("Block Index Is > inode.numOfBlocks\n");
        return 0;
      }
      break;
    }
  }

  int fd = open(FILESYS, O_RDWR, 660);
  int totalWrite = 0;
  int blockIdx = 0;
  int blockCount = 0;
  while(count > 0) {
    if(blk == 0) {
      if (inode.direct[0] == -1) {
        printf("Error: No direct block 1\n");
        return totalWrite;
      } else {
        blockIdx = inode.direct[0];
      }
    } else if(blk == 1) {
      if (inode.direct[1] == -1) {
        printf("Error: No direct block 2\n");
        return totalWrite;
      } else {
        blockIdx = inode.direct[1];
      }
    } else if(blk >= 2) {
      printf("%d\n", inode.direct[1]);
      if (inode.indirect == -1) {
        printf("Error: No indirect block\n");
        return totalWrite;
      }
      else {
        lseek(fd, DATA_OFFSET + inode.indirect * BLOCK_SIZE + (blk) * sizeof(int), SEEK_SET);
        read(fd, (void*) &blockIdx, sizeof(int));
      }
    }
    if (offset > 0) {
      offset = offset - (blk * BLOCK_SIZE);
    }

    int writeCount = 0;
    if (count < BLOCK_SIZE-offset) {
      writeCount = count;
      count -= writeCount;
    } else {
      writeCount = BLOCK_SIZE-offset;
      count -= writeCount;
    }

    lseek(fd, DATA_OFFSET + blockIdx * BLOCK_SIZE + offset, SEEK_SET);
    write(fd, buf + totalWrite, writeCount);
    totalWrite += writeCount;

    if (count > 0) {
      offset = 0;
      blk++;
      blockCount++;

      if (blk == 1 && getInode(id).direct[1] == -1) {
        struct Superboulder superboulder = getSuperBoulder();
        int nextFreeBlock = superboulder.nextFreeBlock;

        if (nextFreeBlock == MAX_DATA_BLOCK) {
          printf("Reached MAX_DATA_BLOCK : ABORT :\n");
          return -1;
        }

        inode.numOfBlocks = 2;
        inode.direct[1] = nextFreeBlock; //update
        lseek(fd, INODE_OFFSET + inode.id * sizeof(struct Inode), SEEK_SET);
        write(fd, &inode, sizeof(struct Inode));

        superboulder.nextFreeBlock = nextFreeBlock + 1; //update here
        lseek(fd, SB_OFFSET, SEEK_SET);
        write(fd, &superboulder, sizeof(struct Superboulder));

      } else if (blk == 2 && getInode(id).indirect == -1) {
        struct Superboulder superboulder = getSuperBoulder();
        int nextFreeBlock = superboulder.nextFreeBlock;

        if (nextFreeBlock == MAX_DATA_BLOCK) {
          printf("Reached MAX_DATA_BLOCK : ABORT :\n");
          return -1;
	      }

        inode.indirect = nextFreeBlock; //update
        lseek(fd, INODE_OFFSET + inode.id * sizeof(struct Inode), SEEK_SET);
        write(fd, &inode, sizeof(struct Inode));

        superboulder.nextFreeBlock = nextFreeBlock + 1; //update here
        lseek(fd, SB_OFFSET, SEEK_SET);
        write(fd, &superboulder, sizeof(struct Superboulder));

        nextFreeBlock = nextFreeBlock + 1;
        inode.numOfBlocks = inode.numOfBlocks + 1;
        lseek(fd, INODE_OFFSET + inode.id * sizeof(struct Inode), SEEK_SET);
        write(fd, &inode, sizeof(struct Inode));

        lseek(fd, DATA_OFFSET + inode.indirect * BLOCK_SIZE + (inode.numOfBlocks) * sizeof(int), SEEK_SET);
        write(fd, &nextFreeBlock, sizeof(int));

        superboulder.nextFreeBlock = nextFreeBlock + 14;
        lseek(fd, SB_OFFSET, SEEK_SET);
        write(fd, &superboulder, sizeof(struct Superboulder));
      } else if (blk > 2) {
        struct Superboulder superboulder = getSuperBoulder();
        int nextFreeBlock = superboulder.nextFreeBlock;

        if (nextFreeBlock == MAX_DATA_BLOCK) {
          printf("Reached MAX_DATA_BLOCK : ABORT :\n");
          return -1;
	      }

        inode.numOfBlocks = inode.numOfBlocks + 1;
        lseek(fd, INODE_OFFSET + inode.id * sizeof(struct Inode), SEEK_SET);
        write(fd, &inode, sizeof(struct Inode));

        lseek(fd, DATA_OFFSET + inode.indirect * BLOCK_SIZE + (inode.numOfBlocks) * sizeof(int), SEEK_SET);
        write(fd, &nextFreeBlock, sizeof(int));

        superboulder.nextFreeBlock = nextFreeBlock + 14;
        lseek(fd, SB_OFFSET, SEEK_SET);
        write(fd, &superboulder, sizeof(struct Superboulder));
      }
    }
  }

  inode.size = inode.size + totalWrite;
  lseek(fd, INODE_OFFSET + inode.id * sizeof(struct Inode), SEEK_SET);
  write(fd, &inode, sizeof(struct Inode));

  close(fd);
  return totalWrite;
}

int createIndirectBlk(struct Inode inode) {
  struct Superboulder superboulder = getSuperBoulder();
  int nextFreeBlock = superboulder.nextFreeBlock;

  if (nextFreeBlock == MAX_DATA_BLOCK) {
    printf("Reached MAX_DATA_BLOCK : ABORT :\n");
    return -1;
  }

  inode.indirect = nextFreeBlock; //update

  int fd = open (FILESYS, O_RDWR, 660);
  lseek(fd, INODE_OFFSET + inode.id*sizeof(struct Inode), SEEK_SET);
  write(fd, &inode, sizeof(struct Inode));

  superboulder.nextFreeBlock = nextFreeBlock+1; //update here
  lseek(fd, SB_OFFSET, SEEK_SET);
  write(fd, &superboulder, sizeof(struct Superboulder));

  close(fd);
  return nextFreeBlock;
}
