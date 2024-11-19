// libs:

#define MAX_NAME_LEN 50
#define MAX_MSG_LEN 256
typedef struct Message
{
  char from[MAX_NAME_LEN];   // Sender's name
  char to[MAX_NAME_LEN];     // Recipient's name
  char content[MAX_MSG_LEN]; // Message content
} Message;

volatile int server_running = 1;

void handle_signal_interrupt(sig)
{
  printf("shutting server down...\n");
  server_running = 0;
  // close socket and what not
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
  // socket()
  // bind()
  // accept()

  // create queue to store msgs

  // *wait for a client to connect*

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