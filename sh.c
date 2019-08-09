#include "sizes.h"
#include "funSizeSys/cmd.h"
#include <string.h>
#include <stdlib.h>

static size_t maxLength = MAX_COMMAND_LENGTH;

void guide() {
  printf("<---------------------------------------------------------------------------------------->\n");
  printf(": : : : : : : : : : : : : : : : : :  [COMMAND BELT]  : : : : : : : : : : : : : : : : : : :\n");
  printf("| ls                   -  Show files/directories in current directory\n");
  printf("| cd    [path]         -  Change current directory to the one specified\n");
  printf("| mkdir [name]         -  Make empty directory (with specified name) in current directory\n");
  printf("| touch [name]         -  Make empty file (with specified name) in current directory\n");
  printf("| cat   [name]         -  Read contents from file specified\n");
  printf("| write [name] [text]  -  Write [text] to the end of the file specified\n");
  printf("| help                 -  Command Info\n");
  printf("| exit                 -  Leave Portal\n");
  printf("<---------------------------------------------------------------------------------------->\n");
  printf("\n<$> ");
}

int main() {
  guide();

  int dirID = 0;

  while(1) {
    char cmd[10];

    char* str = NULL;
    int line = getline(&str, &maxLength, stdin);

    if (line == -1) {
      printf("getline() ERROR\n");
      return -1;
    } else if (str[0] == '\n') {
      cmd[0] = '\0';
    } else {
      str[line - 1] = '\0';
      char* tok = strtok(str, " ");
      strcpy(cmd, tok);
    }

    /* Command Received : Moving Forward*/
    if (strcmp(cmd, "ls") == 0) {
      cmdLS(dirID);
      printf("\n<$> ");
    } else if(strcmp(cmd, "cd") == 0){
      char name[MAX_LENGTH_FILE_NAME];
      char* tok = strtok(NULL, " ");

      strcpy(name, tok);
      if (tok == NULL) {
	printf("Parameter ERROR\n");
	break;
      }

      dirID = cmdCD(dirID, name);
      printf("\n<$> ");
    } else if(strcmp(cmd, "mkdir") == 0){
      char name[MAX_LENGTH_FILE_NAME];
      char* tok = strtok(NULL, " ");

      strcpy(name, tok);
      if (tok == NULL) {
	printf("Parameter ERROR\n");
	break;
      }

      cmdMKDIR(dirID, name);
      printf("\n<$> ");
    } else if(strcmp(cmd, "touch") == 0){
      char name[MAX_LENGTH_FILE_NAME];
      char* tok = strtok(NULL, " ");

      strcpy(name, tok);
      if (tok == NULL) {
	printf("Parameter ERROR\n");
	break;
      }

      cmdTOUCH(dirID, name);
      printf("\n<$> ");
    } else if(strcmp(cmd, "cat") == 0) {
      char name[MAX_LENGTH_FILE_NAME];
      char* tok = strtok(NULL, " ");

      strcpy(name, tok);
      if (tok == NULL) {
	printf("Parameter ERROR\n");
	break;
      }

      cmdCAT(dirID, name);
      printf("\n<$> ");
    } else if(strcmp(cmd, "write") == 0) {
      char* tok = strtok(NULL, " ");
      char name[MAX_LENGTH_FILE_NAME];

      strcpy(name, tok);
      if (tok == NULL) {
	printf("Parameter ERROR\n");
	break;
      }

      char text[MAX_LENGTH_FILE_NAME];
      tok = strtok(NULL, " ");
      if (tok==NULL) {
	printf("Parameter ERROR\n");
	break;
      }
      strcpy(text, tok);

      cmdWRITE(dirID, name, text);
      printf("\n<$> ");
    } else if(strcmp(cmd, "help") == 0) {
      guide();
    } else if(strcmp(cmd, "exit") == 0) {
      printf("Leaving Portal . . .\n");
      exit(0);
      return 0;
    } else{
      printf("Unknown cmd : %s\n$ ", cmd);
    }
  }
}
