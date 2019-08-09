#pragma once

struct Inode {        /* The structure of iNode, each file has only one iNode */
  int id;             /* The iNode number */
  int size;           /* The size of file */
  int numOfBlocks;    /* The total numbers of data blocks */
  int direct[2];      /* Two direct data block pointers */
  int indirect;       /* One indirect data block pointer */
  int numOfFiles;     /* Amount of files in directory */
};


/*
 * Function:  getInode
 *-----Get Inode by it's id
 *
 *     id: Inode id to return
 * Returns: Inode
 */
struct Inode getInode(int id);

void createInode(int id, int parentId, int block, int type);
