// libs:
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>

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
  printf("\nshutting server down...\n");
  server_running = 0;
  close(socketfd);
  exit(0);
}

void *read_from_client(void *client_socket)
{
  int communication_socket = *((int *)client_socket);
  free(client_socket);

  // read to buffer
  char buffer[MAX_MSG_LEN];
  while (server_running)
  {
    ssize_t bytes_read = read(communication_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read <= 0)
    {
      printf("client disconnected.\n");
      break;
    }
    buffer[bytes_read] = '\0'; // null-terminate buffer

    // store msg to queue

    write(communication_socket, "Message received.\n", 18);
  }
  close(communication_socket);
  return NULL;
}

void *write_to_client(void *client_socket)
{
  // create communication socket
  // write message from queue to target client
  // close comm socket
  return NULL;
}

int main()
{
  printf("starting server...\n");

  int new_socket;
  struct sockaddr_in server_addr, client_addr;
  socklen_t client_addr_len = sizeof(client_addr);
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
    perror("server socket creation failed");
    return 1;
  }

  // init server_addr structure
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;         // IPV4
  server_addr.sin_addr.s_addr = INADDR_ANY; // bind to any available network interface (basically ip address)
  const int PORT_NUM = 8080;
  server_addr.sin_port = htons(PORT_NUM); // set port number

  // bind socket to IP and port
  if (bind(socketfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
  {
    perror("bind failed");
    close(socketfd);
    return 1;
  }

  // listen for client sockets (20 max)
  const int BACKLOG_SIZE = 20;
  if (listen(socketfd, BACKLOG_SIZE) == -1)
  {
    perror("listen failed");
    close(socketfd);
    return 1;
  }

  printf("Server is listening on %i\n", PORT_NUM);

  while (server_running)
  {
    new_socket = accept(socketfd, (struct sockaddr *)&client_addr, &client_addr_len);
    if (new_socket == -1)
    {
      perror("accept failed");
      continue;
    }

    // TODO: the first thing that the client sends will be interpreted as the clients name
    // - this will need to be handled by the server in the handle_client function
    // - it will need to be implemented on ther server end (so that the client knowns that it is sending a name)
    // - ALSO should this be stored on the server end? so that all msgs can be logged with the user?
    printf("Client connected.\n");

    // if queue is not empty && client that connected is target of next msg:
    // write_to_client

    int *client_socket = malloc(sizeof(int));
    if (client_socket == NULL)
    {
      perror("mem allocation of client_socket failed");
      close(new_socket);
      continue;
    }

    *client_socket = new_socket;

    // create new thread to handle client
    pthread_t thread_id;
    if (pthread_create(&thread_id, NULL, read_from_client, (void *)client_socket) != 0)
    {
      perror("thread creation failed");
      free(client_socket);
      close(new_socket);
    }

    // detach thread to allow clean up
    pthread_detach(thread_id);
  }

  // clean up
  close(socketfd);
  return 0;
}