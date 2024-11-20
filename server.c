// libs:
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <string.h>

#define MAX_NAME_LEN 50
#define MAX_MSG_LEN 256
typedef struct Message
{
  char from[MAX_NAME_LEN];   // Sender's name
  char to[MAX_NAME_LEN];     // Recipient's name
  char content[MAX_MSG_LEN]; // Message content
} Message;

volatile int server_running = 1;
int socketfd;

void handle_signal_interrupt()
{
  printf("shutting server down...\n");
  server_running = 0;
  close(socketfd);
  exit(0);
}

void connect_client(const char *CLIENT_ID)
{
  printf("client connecting...\n");
  // handle connecting client...fork()?
  printf("client %s connected.\n", CLIENT_ID);
}

int main()
{
  printf("starting server...\n");

  struct sockaddr_in server_addr;
  // create queue to store msgs

  // Set up signal handler for SIGINT (Ctrl+C)
  if (signal(SIGINT, handle_signal_interrupt) == SIG_ERR)
  {
    perror("signal");
    return 1;
  }

  // create server (listener) socket
  socketfd = socket(AF_INET, SOCK_STREAM, 0);
  if (socketfd == -1)
  {
    perror("socket");
    return 1;
  }

  // init server_addr structure
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;         // IPV4
  server_addr.sin_addr.s_addr = INADDR_ANY; // bind to any available network interface (basically ip address)
  server_addr.sin_port = htons(8080);       // port number

  // bind socket to IP and port
  if (bind(socketfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
  {
    perror("bind");
    close(socketfd);
    return 1;
  }

  // listen for client sockets (20 max)
  const int BACKLOG_SIZE = 20;
  if (listen(socketfd, BACKLOG_SIZE) == -1)
  {
    perror("listen");
    close(socketfd);
    return 1;
  }

  while (server_running)
  {

    // *client connects:*
    // while (a client is connected):
    // if (next msg target is client that connected):
    // write() msg to target client
    // else:
    // write() "enter msg:"
    // msg = read()
    // append msg to queue
  }

  // clean up
  // close()... and other socket clean up
  return 0;
}