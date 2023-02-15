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

void event_handler(int connfd) {
  char buff[MAX];
  int n;

  for (;;) {
    memset(buff, 0, MAX);

    recv(connfd, buff, sizeof(buff), 0);
    printf("From client %s\t To client: ", buff);
    memset(buff, 0, MAX);
    n = 0;

    while ((buff[n++] = getchar()) != '\n')
      ;
    send(connfd, buff, sizeof(buff), 0);

    if (strncmp("exit", buff, 4) == 0) {
      printf("Server exit...");
      break;
    }
  }
}

int main(void) {
  int sockfd, connfd, len;
  struct sockaddr_in servaddr, cli;

  // create sock
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    printf("socket ceation failed");
    return 1;
  } else {
    printf("Socket succesfuly created..\n");
  }

  memset(&servaddr, 0, sizeof(servaddr));

  // assign IP and PORT
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(PORT);

  // binding socket
  if ((bind(sockfd, (sock_addr *)&servaddr, sizeof(servaddr))) != 0) {
    printf("Socket bind failed...\n");
    return 1;
  } else {
    printf("Socket succesfuly binded.");
  }

  // now listen activity in socket port.
  if ((listen(sockfd, 5)) != 0) {
    printf("Listen failed...\n");
    return 1;
  } else {
    printf("Server listening...\n");
  }

  len = sizeof(cli);

  // accept data packet from client and verify it.
  connfd = accept(sockfd, (sock_addr *)&cli, &len);

  if (connfd < 0) {
    printf("Server doesn't accept data packet!");
  }
  // run event_handler for chatting between client and server
  event_handler(connfd);

  // close socket for exiting program.
  closesocket(sockfd);

  return 0;
}
