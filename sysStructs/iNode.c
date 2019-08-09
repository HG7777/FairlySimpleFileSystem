#include "../sizes.h"
#include "../funSizeSys/readWriteSys.h"

#include "iNode.h"
#include "mapping.h"

#include <stdlib.h>

struct Inode getInode(int id) {
  struct Inode toReturn;
  readSys(INODE_OFFSET + id * sizeof(struct Inode), (void*) &toReturn, sizeof(struct Inode));

  return toReturn;
}


void createInode(int id, int parentId, int block, int type) {
  struct Inode newInode;
  newInode.id = id;
  newInode.size = 0;
  newInode.numOfBlocks = 1;
  newInode.direct[0] = block;
  newInode.direct[1] = -1;
  newInode.indirect = -1;

  if (type == 0) {
    newInode.numOfFiles = 0;
  } else {
    newInode.numOfFiles = 2;

    struct Mapping self = createMapping(".", id);
    writeSys (DATA_OFFSET + newInode.direct[0] * BLOCK_SIZE, (void*) &self, sizeof(struct Mapping));

    struct Mapping parent = createMapping("..", parentId);
    writeSys (DATA_OFFSET + newInode.direct[0] * BLOCK_SIZE + sizeof(struct Mapping), (void*) &parent, sizeof(struct Mapping));
  }

  writeSys (INODE_OFFSET + sizeof(struct Inode) * id, (void*) &newInode, sizeof(struct Inode));

  return;
}
