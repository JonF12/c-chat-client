#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "../include/chat_client.h"
#define REQ_LIMIT 10
#define BUFFER_SIZE 4096

void error (const char *str){
  perror(str);
  exit(1);
}

void connect_to_server(struct chat_request_model *crm){
  int sockfd, newsockfd, portno, n;
  char buffer[BUFFER_SIZE];
  struct sockaddr_in s_address, c_address;
  socklen_t c_len, s_len;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0){
    error("error creating socket \n");
    return;
  }
  bzero((char *)&s_address, sizeof(s_address));
  s_address.sin_family = AF_INET;
  s_address.sin_addr.s_addr = INADDR_ANY;
  s_address.sin_port = htons(crm->port);

  if (bind(sockfd, (struct sockaddr *) &s_address, sizeof(s_address)) < 0){
    error("failed to bind \n");
  }

  listen(sockfd, REQ_LIMIT);
  c_len = sizeof(c_address);
  newsockfd = accept(sockfd, (struct sockaddr *) &c_address, &c_len);

  if (newsockfd < 0){
    error("failed to accept \n");
  }

  fd_set readfds;  
  int maxfd = newsockfd > STDIN_FILENO ? newsockfd : STDIN_FILENO;  

  while (1) {
    FD_ZERO(&readfds);  
    FD_SET(newsockfd, &readfds);  
    FD_SET(STDIN_FILENO, &readfds);  

    int activity = select(maxfd + 1, &readfds, NULL, NULL, NULL);
    if (activity < 0) {
      error("error on select");
    }

    if (FD_ISSET(newsockfd, &readfds)) {
      memset(buffer, 0, BUFFER_SIZE);
      n = read(newsockfd, buffer, BUFFER_SIZE - 1);
      if (n < 0) {
        error("error on read");
      }
      printf("Client: %s\n", buffer);

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

      n = write(newsockfd, buffer, strlen(buffer));
      if (n < 0) {
        error("error on write");
      }

      if (strncmp("/exit", buffer, 5) == 0) {
        printf("Server exiting\n");
        break;
      }
    }
  }
  shutdown(newsockfd, SHUT_RDWR);
  close(newsockfd);
  close(sockfd);
}
