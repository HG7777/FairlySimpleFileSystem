#include "sizes.h"
#include "funSizeSys/readWriteSys.h"
#include "sysStructs/superBoulder.h"
#include "sysStructs/iNode.h"
#include "sysStructs/mapping.h"

int main(int argc, char *argv[]) {

  // Initializing superboulder
  struct Superboulder superboulder;
  superboulder.inodeOffset = INODE_OFFSET;
  superboulder.dataOffset = DATA_OFFSET;
  superboulder.maxInodeNum = MAX_INODE;
  superboulder.maxDataBlockNum = MAX_DATA_BLOCK;
  superboulder.sizeOfBlock = BLOCK_SIZE;
  superboulder.nextFreeInode = 1;
  superboulder.nextFreeBlock = 1;
  writeSys (SB_OFFSET, (void*) &superboulder, sizeof(struct Superboulder));

  // Initializing root iNode
  struct Inode root;
  root.id = 0;
  root.size = 0;
  root.numOfBlocks = 1;
  root.direct[0] = 0;
  root.direct[1] = -1;
  root.indirect = -1;
  root.numOfFiles = 1;
  writeSys (INODE_OFFSET, (void*) &root, sizeof(struct Inode));

  // Map root to itself
  struct Mapping rootDir = createMapping(".", 0);
  writeSys (DATA_OFFSET, (void*) &rootDir, sizeof(struct Mapping));

  return 0;
}
