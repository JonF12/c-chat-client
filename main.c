#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "src/include/chat_client.h"

int main(int argc, char *argv[]){
  if (argc == 2 && strcmp(argv[1], "--help") == 0) {
    printf("Usage: %s [options] <arguments>\n", argv[0]);
    printf("\nOptions:\n");
    printf("  --s hostname     Set chat server url \n");
    printf("  --p port         Set chat server port \n");
    return 0;
  }
  if (argc < 3) {
      printf("--help to see usage.\n");
      return -1;
  }
  int ct = 1;
  int in_arg = 0;
  char argName[64];
  
  struct chat_request_model *ftpArgs = malloc(sizeof(struct chat_request_model));
  if (ftpArgs == NULL) {
      printf("Memory allocation failed\n");
      return -1;
  }

  while (ct < argc){
    if (argv[ct][0] == '-' && argv[ct][1] == '-') {
      if (strcmp(argv[ct], "--s") == 0) {
        strcpy(ftpArgs->server, argv[ct + 1]);
        ct += 2;
      } else if (strcmp(argv[ct], "--p") == 0){
        ftpArgs->port = atoi(argv[ct + 1]);
        ct += 2;
      } else { 
        printf("Unknown option: %s\n", argv[ct]);
        free(ftpArgs);
        return -1;
      }
    } else {
      printf("Invalid argument: %s\n", argv[ct]);
      free(ftpArgs); 
      return -1;
    }
  }
  printf("%s \n", ftpArgs->server);
  printf("%d \n", ftpArgs->port);

  connect_to_server(ftpArgs);
  free(ftpArgs);
  return 0;
}
