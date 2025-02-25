#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#define PORT 8080

int main() {
  int sockfd, client_sockfd;
  struct sockaddr_in address;
  int option_value = 1;
  socklen_t addrlen = sizeof(address);

  // Create socket to communicate with IPv4 client via TCP using IP
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("Failed to create socket");
    exit(EXIT_FAILURE);
  }

  // Attempt to attach socket to port
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option_value,
                 sizeof(option_value)) < 0) {
    perror("Failed to attach socket to port");
    exit(EXIT_FAILURE);
  }

  // Set protocol, port number, and IPv4 address in address struct
  address.sin_family = AF_INET;
  address.sin_port = htons(PORT);
  address.sin_addr.s_addr = INADDR_ANY;

  // Attempt to bind socket to port
  if (bind(sockfd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("Failed to bind socket to port");
    exit(EXIT_FAILURE);
  }

  // Attempt to listen on bound port for connection requests
  if (listen(sockfd, 3) < 0) {
    perror("Failed to listen for client connections");
    exit(EXIT_FAILURE);
  }

  // Attempt to accept an incoming client connection
  if ((client_sockfd = accept(sockfd, (struct sockaddr *)&address, &addrlen)) <
      0) {
    perror("Failed to accept client connection");
    exit(EXIT_FAILURE);
  }

  return 0;
}
