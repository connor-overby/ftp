#include <arpa/inet.h>
#include <fcntl.h>
#include <libgen.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#define PORT 8080

int main(int argc, char *argv[]) {
  int status, sockfd;
  struct sockaddr_in address;
  int target_file;
  unsigned char byte;

  // Attempt to create socket
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("Could not create socket");
    return -1;
  }

  address.sin_family = AF_INET;
  address.sin_port = htons(PORT);
  if (inet_pton(AF_INET, argv[1], &address.sin_addr) <= 0) {
    perror("Invalid IPv4 Address");
    return -1;
  }

  if ((status = connect(sockfd, (struct sockaddr *)&address, sizeof(address))) <
      0) {
    perror("Connection to server failed");
    return -1;
  }

  char *stripped_file_name = basename(argv[2]);
  uint file_name_size = sizeof stripped_file_name;
  if (write(sockfd, &file_name_size, 4) == -1) {
    perror("Failed to send file_name_size");
    return -1;
  } else {
    printf("%s is %d bytes\n", stripped_file_name, file_name_size);
  }

  if (write(sockfd, stripped_file_name, file_name_size) == -1) {
    perror("Failed to send file name");
    return -1;
  } else {
    printf("Sent file name: %s\n", stripped_file_name);
  }

  target_file = open(argv[2], O_RDONLY);
  while (read(target_file, &byte, 1) > 0) {
    if (write(sockfd, &byte, 1) == -1) {
      perror("Failed to write to socket");
      return -1;
    }
  }

  close(target_file);
  close(sockfd);
  return 0;
}
