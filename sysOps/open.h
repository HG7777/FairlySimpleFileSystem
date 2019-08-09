#include "../sizes.h"
#include "../sysStructs/superBoulder.h"

#include <string.h>
#include <stdio.h>

int open_t(int currID, const char* path) {
  char* string = strdup(path);

  if (string != NULL) {
    char* token = strsep(&string, "/");
    while (token != NULL) {
      if (strcmp(token, "") != 0) {
        struct Inode currDir = getInode(currID);
	      int offset = DATA_OFFSET + currDir.direct[0] * BLOCK_SIZE;
	      int flag = 1;

        for (int i = 0; i < currDir.numOfFiles; i++) {
          struct Mapping mapping;
          readSys(offset + i * sizeof(struct Mapping), (void*) &mapping, sizeof(struct Mapping));

          if (strcmp(token, mapping.name) == 0) {
            currID = mapping.id;
	          flag = 0;
	          break;
          }
        }

        if (flag) {
          return -1;
	      }
      }
      token = strsep(&string, "/");
    }
    return currID;
  }
  return -1;
}
