#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

int main(int argc, char *argv[]) {
  int status, sockfd;
  struct sockaddr_in address;
  int option_value = 1;
  socklen_t addrlen = sizeof(address);
  FILE *fptr;
  int bufferSize;

  // Attempt to create socket
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("Could not create socket");
    exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  address.sin_port = htons(PORT);
  if (inet_pton(AF_INET, argv[1], &address.sin_addr) <= 0) {
    perror("Invalid IPv4 Address");
    exit(EXIT_FAILURE);
  }

  if ((status = connect(sockfd, (struct sockaddr *)&address, sizeof(address))) <
      0) {
    perror("Connection to server failed");
    return -1;
  }

  write(sockfd, "Client", sizeof("Client"));

  close(sockfd);
  return 0;
}
