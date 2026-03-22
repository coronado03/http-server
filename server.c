#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <ctype.h>
#include <netdb.h>
#include <netinet/in.h>

#define MYPORT "3490"
#define BACKLOG 10

void get_request_method(char *method_dest, char *request) {
  int index = 0;

  while (!isspace(request[index])) {
    method_dest[index] = request[index];
    index++;
  }

  method_dest[index] = '\0';
}

void get_request_url(char *method_dest, char *request) {
  int url_index, request_index = 0;

  do {
    request_index++;
  } while (!isspace(request[request_index]));
  request_index++;

  while (!isspace(request[request_index])) {
    method_dest[url_index] = request[request_index];
    url_index++;
    request_index++;
  }

  method_dest[url_index] = '\0';
}

void get_file_data() {
  FILE *fptr = fopen("./index.html", "r");
  if (fptr == NULL) {
    printf("TS don't work");
    return;
  }

  char myString[300];

  while (fgets(myString, 100, fptr) != NULL) {
    printf("%s", myString);
  }

  printf("HTML: %s", myString);

  fclose(fptr);
}

void request_parser(char *request, char *response, int response_len) {
  char request_method[8];
  char request_url[236];

  get_request_method(request_method, request);
  get_request_url(request_url, request);
  get_file_data();

  printf("Request Method: %s\n", request_method);
  printf("Request Path: %s\n", request_url);

  printf("%s\n", request);
};

int main() {
  struct sockaddr_storage client_addr;
  socklen_t addr_size;
  struct addrinfo hints, *res;
  int socketfd, new_fd;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  getaddrinfo(NULL, MYPORT, &hints, &res);
  socketfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  bind(socketfd, res->ai_addr, res->ai_addrlen);

  printf("Server running on port %s...\n", MYPORT);

  while (1) {
    listen(socketfd, BACKLOG);

    addr_size = sizeof client_addr;
    new_fd = accept(socketfd, (struct sockaddr *)&client_addr, &addr_size);

    int len, bytes_sent, bytes_received;
    char *response_message = "HTTP/1.1 200 OK\r\n"
                             "Server: localhost\r\n"
                             "Content-Type: text/html\r\n"
                             "Content-Length: 44\r\n"
                             "\r\n"
                             "<html><body> BEEP BOOP BLABLA</body></html>";
    char request_message[1024];

    bytes_received =
        recv(new_fd, request_message, sizeof(request_message) - 1, 0);
    len = strlen(response_message);

    request_parser(request_message, response_message, len);

    bytes_sent = send(new_fd, response_message, len, 0);
  }
}
