
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <netdb.h>
#include <netinet/in.h>

#define MYPORT "3490"
#define BACKLOG 10

int main() {
  struct sockaddr_storage client_addr;
  socklen_t addr_size;
  struct addrinfo hints, *res;
  int socketfd, new_fd;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  getaddrinfo(NULL, "3490", &hints, &res);

  socketfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  bind(socketfd, res->ai_addr, res->ai_addrlen);
  listen(socketfd, 2);

  addr_size = sizeof client_addr;
  new_fd = accept(socketfd, (struct sockaddr *)&client_addr, &addr_size);

  char *msg = "Seb was here!";
  int len, bytes_sent;
  len = strlen(msg);
  bytes_sent = send(socketfd, msg, len, 0);
}
