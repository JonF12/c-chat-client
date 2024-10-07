#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


void connect_to_server(){
  int sockfd, newsockfd, portno, n;
  char buffer[256];
  struct sockaddr_in s_address, c_address;
  socklen_t c_len, s_len;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1){
    printf("error creating socket \n");
    return;
  }
  bzero((char *)&s_address, sizeof(s_address));
  s_address.sin_family = AF_INET;
  s_address.sin_addr.s_addr = INADDR_ANY;
  s_address.sin_port = htons(portno);

  if (bind(sockfd, (struct sockaddr *) &s_address, sizeof(s_address))){

  } else {

  }



  printf("In Module1\n");
}

void send_file(){

}
