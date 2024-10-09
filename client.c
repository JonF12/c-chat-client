#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define BUFFER_SIZE 4096
void error (const char *str){
  perror(str);
  exit(1);
}

int main (int argc, char *argv[]){
  int sockfd, portno, n;
  struct sockaddr_in s_address;
  struct hostent *server;

  char buffer[BUFFER_SIZE];
  if (argc < 3) {
    printf("usage: hostname port, %s \n", argv[0]);
    exit(0);
  }
  portno = atoi(argv[2]);
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0){
    error("error opening socket");
  }

  server = gethostbyname(argv[1]);
  if (server == NULL){
    error("no such host \n");
  }

  bzero((char *) &s_address, sizeof(s_address));
  s_address.sin_family = AF_INET;
  bcopy((char *) server->h_addr, (char *) &s_address.sin_addr.s_addr, server->h_length);
  s_address.sin_port = htons(portno);

  if (connect(sockfd, (struct sockaddr *) &s_address, sizeof(s_address))< 0){
    error("Failed to connect \n");
  }

  fd_set readfds;  
  int maxfd = sockfd > STDIN_FILENO ? sockfd : STDIN_FILENO; 

  while (1) {
    FD_ZERO(&readfds);
    FD_SET(sockfd, &readfds);
    FD_SET(STDIN_FILENO, &readfds);
    int activity = select(maxfd + 1, &readfds, NULL, NULL, NULL);
    if (activity < 0) {
      error("error on select");
    }

    if (FD_ISSET(sockfd, &readfds)) {
      memset(buffer, 0, BUFFER_SIZE);  
      n = read(sockfd, buffer, BUFFER_SIZE - 1);
      if (n < 0) {
        error("error on read");
      }
      printf("Server: %s\n", buffer);

      if (strncmp("/exit", buffer, 5) == 0) {
        printf("Client disconnected\n");
        break;
      }
    }
    if (FD_ISSET(STDIN_FILENO, &readfds)) {
      memset(buffer, 0, BUFFER_SIZE); 
      if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
        error("error reading input from stdin");
        break;
      }

      n = write(sockfd, buffer, strlen(buffer));
      if (n < 0) {
        error("error on write");
      }

      if (strncmp("/exit", buffer, 5) == 0) {
        printf("Server exiting\n");
        break;
      }
    }
  }

}
