/**
 * Server side program for a socket connection
 * in C.
 *
 * Ref: Socket Programming in C
 * https://youtube.com/playlist?list=PLPyaR5G9aNDvs6TtdpLcVO43_jvxp4emI&si=InjyoX3xL126qPuM
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

void error(const char *msg) {
  perror(msg);
  exit(1);
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Port no. not provided. Program terminated.");
    exit(1);
  }

  int sockfd, cli_sockfd, portno, n;
  char buffer[255];

  /*
   * sockaddr_in and sockaddr_in6 are both structures
   * where first member is a sockaddr structure.
   *
   * According to the C standard, the address of a
   * structure and its first member are the same,
   * so you can cast the pointer to sockaddr_in(6)
   * in a pointer to sockaddr.
   *
   * Related ref about sockaddr_in and similar struct
   * https://stackoverflow.com/q/18609397/5614968
   */
  struct sockaddr_in serv_addr, cli_addr;
  socklen_t cli_len;

  /*
   * socket() creates an endpoint for communication and returns a file
   * descriptor that refers to that endpoint.
   *
   * man page: https://man7.org/linux/man-pages/man2/socket.2.html
   */
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    error("Error opening socket.");
  }

  bzero((char *) &serv_addr, sizeof(serv_addr));
  portno = atoi(argv[1]);

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);

  /*
   * When a socket is created with socket(2), it exists in a name space
   * (address family) but has no address assigned to it.  bind()
   * assigns the address specified by addr to the socket referred to by
   * the file descriptor sockfd.
   *
   * man page: https://www.man7.org/linux/man-pages/man2/bind.2.html
   */
  if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    error("Binding failed.");
  }

  /*
   * NEEDSWORK:
   * We call the API to allow 5 clients to connect to the server
   * but we don't invoke clients in threads. Need to change the
   * code accordingly.
   */
  /*
   * listen() marks the socket referred to by sockfd as a passive
   * socket, that is, as a socket that will be used to accept incoming
   * connection requests using accept(2).
   *
   * man page: https://man7.org/linux/man-pages/man2/listen.2.html
   */
  listen(sockfd, 5);
  cli_len = sizeof(cli_addr);

  printf("Listening for client on port %d\n", portno);

  cli_sockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &cli_len);

  if (cli_sockfd < 0) {
    error("Error on accept");
  }
  else
  {
    printf("A client has connected to the server.\n");
  }

  while (1) {
    bzero(buffer, 255);
    n = read(cli_sockfd, buffer, 255);

    if (n < 0) {
      error("Error on reading.");
    }

    printf("Client: %s", buffer);
    printf("Your message: ");
    fgets(buffer, 255, stdin);

    n = write(cli_sockfd, buffer, strlen(buffer));
    if (n < 0) {
      error("Error on writing.");
    }

    int i = strncmp("Bye", buffer, 3);
    if (i == 0) {
      break;
    }
  }

  close(cli_sockfd);
  close(sockfd);
  return 0;
}
