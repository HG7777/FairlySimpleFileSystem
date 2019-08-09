#include "../sizes.h"
#include "readWriteSys.h"
#include "../sysStructs/superBoulder.h"
#include "../sysStructs/iNode.h"
#include "../sysStructs/mapping.h"
#include "../sysOps/open.h"
#include "../sysOps/read.h"
#include "../sysOps/write.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void cmdLS (int dirID) {
  struct Inode currDir = getInode(dirID);
  int offset = DATA_OFFSET + currDir.direct[0] * BLOCK_SIZE;
  for (int i = 0; i < currDir.numOfFiles; i++) {
    struct Mapping mapping;
    readSys(offset + i * sizeof(struct Mapping), (void*) &mapping, sizeof(struct Mapping));

    struct Inode tmp = getInode(mapping.id);
    if (tmp.numOfFiles == 0) {
      printf("\033[22;34m %s \033[0m", mapping.name);
    } else {
      printf("\033[22;34m %s/ \033[0m", mapping.name);
    }
  }
}

int cmdCD (int dirID, char name[MAX_LENGTH_FILE_NAME]) {
  if(strcmp(name,"/")==0) {
    printf("Current Directory is Root\n");
    return 0;
  }

  if (name[0] == '/') {
    dirID = 0;
  }

  int temp = open_t(dirID, name);
  if (temp == -1) {
    printf("Can't Find Path\n");
    return dirID;
  } else{
    if (getInode(temp).numOfFiles == 0) {
      printf("This is a File, not a Directory\n");
      return dirID;
    }
    return temp;
  }
}

void cmdMKDIR(int dirID, char name[MAX_LENGTH_FILE_NAME]) {
  //Check if there already is a file with the same name
  struct Inode currDir = getInode(dirID);
  int offset = DATA_OFFSET + currDir.direct[0] * BLOCK_SIZE;
  for (int i = 0; i < currDir.numOfFiles; i++) {
    struct Mapping mapping;
    readSys(offset + (i * sizeof(struct Mapping)), (void*) &mapping, sizeof(struct Mapping));

    if(strcmp(name,mapping.name)==0) {
      printf("A Directory With This Name Exists Already\n");
      return;
    }
  }

  // Get next free iNode and block
  struct Superboulder superboulder = getSuperBoulder();
  int nextFreeInode = superboulder.nextFreeInode;
  int nextFreeBlock = superboulder.nextFreeBlock;

  // Create iNode for new directory
  createInode(nextFreeInode, dirID, nextFreeBlock, 1);

  // Add new mapping for parent directory
  struct Mapping mapping = createMapping(name, nextFreeInode);
  offset = DATA_OFFSET + currDir.direct[0] * BLOCK_SIZE;
  writeSys (offset + currDir.numOfFiles * sizeof(struct Mapping), (void*) &mapping, sizeof(struct Mapping));

  // Update number of children in parent dir
  currDir.numOfFiles = currDir.numOfFiles + 1;
  writeSys (INODE_OFFSET + dirID * sizeof(struct Inode), (void*) &currDir, sizeof(struct Inode));

  // Finally update superboulder
  superboulder.nextFreeInode = nextFreeInode + 1;
  superboulder.nextFreeBlock = nextFreeBlock + 1;
  writeSys(SB_OFFSET, (void*)& superboulder, sizeof(struct Superboulder));
}

void cmdTOUCH(int dirID, char name[MAX_LENGTH_FILE_NAME]) {
  //Check if there exist a file with the same name
  struct Inode currDir = getInode(dirID);
  int offset = DATA_OFFSET + currDir.direct[0] * BLOCK_SIZE;
  for (int i = 0; i < currDir.numOfFiles; i++) {
    struct Mapping mapping;
    readSys(offset + (i * sizeof(struct Mapping)), (void*) &mapping, sizeof(struct Mapping));

    if(strcmp(name,mapping.name)==0) {
      printf("A Directory With This Name Exists Already\n");
      return;
    }
  }

  // Get next free iNode and block
  struct Superboulder superboulder = getSuperBoulder();
  int nextFreeInode = superboulder.nextFreeInode;
  int nextFreeBlock = superboulder.nextFreeBlock;

  // Create iNode for new directory
  createInode(nextFreeInode, dirID, nextFreeBlock, 0);

  // Add new mapping for parent directory
  struct Mapping mapping = createMapping(name, nextFreeInode);
  offset = DATA_OFFSET + currDir.direct[0] * BLOCK_SIZE;
  writeSys (offset + currDir.numOfFiles * sizeof(struct Mapping), (void*)& mapping, sizeof(struct Mapping));

  // Update number of children in parent dir
  currDir.numOfFiles = currDir.numOfFiles+1;
  writeSys (INODE_OFFSET+dirID*sizeof(struct Inode), (void*)& currDir, sizeof(struct Inode));

  // Finally update superboulder
  superboulder.nextFreeInode = nextFreeInode + 1;
  superboulder.nextFreeBlock = nextFreeBlock + 1;
  writeSys(SB_OFFSET, (void*)& superboulder, sizeof(struct Superboulder));
}

void cmdCAT (int dirID, char name[MAX_LENGTH_FILE_NAME]) {
  int id = open_t (dirID, name);
  if (id == -1) {
    printf("File Not Found\n");
    return;
  }
  struct Inode inode = getInode(id);
  char *buf = malloc(inode.size);
  int code = read_t(inode.id, 0, buf, inode.size);

  if (code == -1) {
    printf("File is Directory : or : File Doesn't Exist\n");
    return;
  } else {
    printf("%s", buf);
    return;
  }
}

void cmdWRITE (int dirID, char name[MAX_LENGTH_FILE_NAME], char text[MAX_LENGTH_TEXT]) {
  int num = open_t (dirID, name);
  struct Inode inode = getInode(num);
  if (num == -1) {
    printf("File Not Found\n");
    return;
  }

  write_t(num, inode.size, text, strlen(text));
  printf("Write Successful\n");
}
