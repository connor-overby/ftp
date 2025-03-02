#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

int main() {
  int socket_file_descriptor, client_socket_file_descriptor,
      incoming_file_descriptor;
  struct sockaddr_in address;
  int option_value = 1;
  socklen_t addrlen = sizeof(address);
  unsigned char byte;

  // Create socket to communicate with IPv4 client via TCP using IP
  if ((socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("Failed to create socket");
    return -1;
  } else {
    printf("Socket created\n");
  }

  // Attempt to attach socket to port
  if (setsockopt(socket_file_descriptor, SOL_SOCKET, SO_REUSEADDR,
                 &option_value, sizeof(option_value)) == -1) {
    perror("Failed to attach socket to port");
    return -1;
  } else {
    printf("Socket attached to port %d\n", PORT);
  }

  // Set protocol, port number, and IPv4 address in address struct
  address.sin_family = AF_INET;
  address.sin_port = htons(PORT);
  address.sin_addr.s_addr = INADDR_ANY;

  // Attempt to bind socket to port
  if (bind(socket_file_descriptor, (struct sockaddr *)&address,
           sizeof(address)) == -1) {
    perror("Failed to bind socket to port");
    return -1;
  } else {
    printf("Socket bound to port\n");
  }

  // Attempt to listen on bound port for connection requests
  if (listen(socket_file_descriptor, 1) == -1) {
    perror("Failed to listen for client connections");
    return -1;
  } else {
    printf("Listening for client connecions...\n");
  }

  // Attempt to accept an incoming client connection
  if ((client_socket_file_descriptor =
           accept(socket_file_descriptor, (struct sockaddr *)&address,
                  &addrlen)) == -1) {
    perror("Failed to accept client connection");
    return -1;
  } else {
    printf("Client connected\n");
  }

  if ((incoming_file_descriptor =
           open("received.txt", O_CREAT | O_WRONLY,
                S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == -1) {
    perror("Failed to open or create file");
    return -1;
  } else {
    printf("File created or opened\n");
  }

  while (read(client_socket_file_descriptor, &byte, 1) > 0) {
    if (write(incoming_file_descriptor, &byte, 1) <= 0) {
      perror("Failed to read from socket");
      return -1;
    } else {
      printf("Writing to file\n");
    }
  }

  close(incoming_file_descriptor);
  close(client_socket_file_descriptor);
  close(socket_file_descriptor);

  return 0;
}
