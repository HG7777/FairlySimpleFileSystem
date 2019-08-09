#pragma once

#define FILESYS "FSFS"
#define SB_OFFSET 512              /* The offset of superboulder region*/
#define INODE_OFFSET 4096          /* The offset of iNode region */
#define DATA_OFFSET 10485760       /* The offset of data region */
#define MAX_INODE 100              /* The maximum number of iNode */
#define MAX_DATA_BLOCK 25600       /* The maximum number of boulder */
#define BLOCK_SIZE 4096            /* The size per boulder */
#define MAX_NESTING_DIR 10         /* The nesting number of directory */
#define MAX_COMMAND_LENGTH 50      /* The maximum command length */
#define MAX_LENGTH_FILE_NAME 10    /* The maximum length of file name */
#define MAX_LENGTH_TEXT 100        /* ToDo: add description */
