/**
 * Client side program for socket connection
 * in C.
 *
 * Ref: Socket Programming in C
 * https://youtube.com/playlist?list=PLPyaR5G9aNDvs6TtdpLcVO43_jvxp4emI&si=InjyoX3xL126qPuM
 */

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>

void error(const char *msg)
{
  perror(msg);
  exit(1);
}

int main(int argc, char *argv[])
{
  int sockfd, portno, n;
  struct sockaddr_in serv_addr;

  /*
   * The strcutre returned by gethostbyname.
   *
   * The gethostbyaddr() function shall return an
   * entry containing addresses of address family
   * type for the host with address addr.
   *
   * Ref: https://stackoverflow.com/a/57313156/5614968
   */
  struct hostent *server;

  char buffer[255];
  if (argc < 3)
  {
    ssize_t usage_bufsz = snprintf(NULL, 0, "usage: %s <hostname> <port>\n", argv[0]);
    char usage_msg[255];

    snprintf(usage_msg, usage_bufsz, "usage: %s <hostname> <port>\n", argv[0]);
    error(usage_msg);
  }

  portno = atoi(argv[2]);

  /*
   * socket() creates an endpoint for communication and returns a file
   * descriptor that refers to that endpoint.
   *
   * man page: https://man7.org/linux/man-pages/man2/socket.2.html
   */
  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0)
  {
    error("ERROR: opening the port");
  }

  /*
   * gethostbyname is deprecated. We need to change this to
   * use getaddrinfo.
   *
   * Ref: 
   *  https://www.man7.org/linux/man-pages/man3/gethostbyname.3.html
   *  https://www.man7.org/linux/man-pages/man3/getaddrinfo.3.html
   */
  server = gethostbyname(argv[1]);
  if (server == NULL)
  {
      error("ERROR: no such host");
  }

  memset(&serv_addr, 0, sizeof(serv_addr));
//  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;

  /*
   * Copy the server address to connect to.
   */
  printf("Server address: %s", server->h_name);
  memcpy(&serv_addr.sin_addr.s_addr, &(server->h_name), server->h_length);
  serv_addr.sin_port = htons(portno);

  if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
  {
    error("ERROR: connection failed");
  }
  else
  {
    printf("Successfully connected to the server @ %s\n", (char *) server->h_name);
  }

  while (1)
  {
    memset(&buffer, 0, sizeof(buffer));
//    bzero(buffer, 255);
    fgets(buffer, 255, stdin);
    n = write(sockfd, buffer, strlen(buffer));
    if (n == 0)
    {
      error("ERROR: failed to write to server");
    }

    memset(&buffer, 0, sizeof(buffer));
//    bzero(buffer, 255);
    n = read(sockfd, buffer, 255);
    if (n < 0)
    {
      error("ERROR: failed to read from server");
    }

    printf("Server: %s", buffer);

    int i = strncmp("Bye", buffer, 3);
    if (i == 0)
    {
      break;
    }
  }

  close(sockfd);
  return 0;
}
