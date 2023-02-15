#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#include <winsock2.h>
#else
exit(1);
#endif
#define MAX 80
#define PORT 8080
#define sock_addr struct sockaddr
void event_handler(int sockfd) {
  char buff[MAX];
  int n;

  for (;;) {
    memset(buff, 0, sizeof(buff));
    printf("Enter data: ");
    n = 0;
    while ((buff[n++] = getchar()) != 0)
      ;

    send(sockfd, buff, sizeof(buff), 0);
    memset(buff, 0, sizeof(buff));
    recv(sockfd, buff, sizeof(buff), 0);
    printf("From server: %s", buff);
    if ((strncmp(buff, "exit", 4)) == 0) {
      printf("Client exiting...\n");
      break;
    }
  }
}

int main(void) {
  int sockfd, connfd;
  struct sockaddr_in servaddr, cli;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    printf("Socket creation failed");
    exit(1);
  } else {
    printf("Socket succesfully created");
  }

  memset(&servaddr, 0, sizeof(servaddr));

  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  servaddr.sin_port = htons(PORT);

  if (connect(sockfd, (sock_addr *)&servaddr, sizeof(servaddr)) != 0) {
    printf("Connection with server is failed!");
    exit(1);
  } else {
    printf("Conected to the server...\n");
  }

  event_handler(sockfd);

  closesocket(sockfd);
  return 0;
}
