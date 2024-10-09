#ifndef CHAT_CLIENT_H
#define CHAT_CLIENT_H

struct chat_request_model {
  char server[256];
  int port;
};

void connect_to_server(struct chat_request_model *crm);

#endif
