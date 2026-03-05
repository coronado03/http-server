#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <ctype.h>
#include <netdb.h>
#include <netinet/in.h>

#define MYPORT "3490"
#define BACKLOG 10

void get_request_method(char *dest, char *path_dest, char *request,
                        int *string_index) {

  char current_letter;
  while (!isspace(current_letter)) {
    current_letter = request[*string_index];
    if (!isspace(current_letter)) {
      dest[*string_index] = current_letter;
      printf("Current Letter: %c\n", current_letter);
    }
    (*string_index)++;
  }
  while (!isspace(current_letter)) {
    current_letter = request[*string_index];
    if (!isspace(current_letter)) {
      path_dest[*string_index] = current_letter;
      printf("Current Letter: %c\n", current_letter);
    }
    (*string_index)++;
  }
}

void get_request_path(char *dest, char *request) {

  char current_letter;
  int string_index = 0;
  while (!isspace(current_letter)) {
    current_letter = request[string_index];
    if (!isspace(current_letter)) {
      dest[string_index] = current_letter;
      printf("Current Letter: %c\n", current_letter);
    }
    string_index++;
  }
}

void read_file() {
  FILE *fptr;

  fptr = fopen("index.html", "r");
  char myString[100];
  fgets(myString, 100, fptr);
  printf("%s", myString);
  fclose(fptr);
}

void request_parser(char *request, char *response, int response_len) {

  int string_index = 0;
  char request_method[5];
  char request_path[10];

  get_request_method(request_method, request_path, request, &string_index);

  printf("Req mesg beep bop: %s\n", request);
  printf("Request Method beep bop: %s\n", request_method);
  printf("Request Path  beep bop: %s\n", request_path);
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

  listen(socketfd, BACKLOG);

  addr_size = sizeof client_addr;
  new_fd = accept(socketfd, (struct sockaddr *)&client_addr, &addr_size);

  int len, bytes_sent, bytes_received;
  char *response_message = "HTTP/1.1 200 OK\r\n"
                           "Server: localhost\r\n"
                           "Content-Type: text/html\r\n"
                           "Content-Length: 44\r\n"
                           "\r\n"
                           "<html><body>This is a test</body></html>";
  char request_message[1024];

  bytes_received =
      recv(new_fd, request_message, sizeof(request_message) - 1, 0);
  len = strlen(response_message);

  request_parser(request_message, response_message, len);

  bytes_sent = send(new_fd, response_message, len, 0);
}
