#pragma once

struct Superboulder {    /* The key information of filesystem */
  int inodeOffset;       /* The start offset of the iNode region */
  int dataOffset;        /* The start offset of the data region */
  int maxInodeNum;       /* The maximum number of iNodes */
  int maxDataBlockNum;   /* The maximum number of data blocks */
  int nextFreeInode;     /* The index of the next free iNode */
  int nextFreeBlock;     /* The index of the next free block */
  int sizeOfBlock;       /* The size per block */
};

/*
 * Function:  getSuperBoulder
 * -----Get superboulder struct from FS-file
 * Returns: superboulder
 */

struct Superboulder getSuperBoulder();
