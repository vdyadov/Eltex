#define MAXPENDING 128
#define MAX_LEN 1024

#define MAX_WAIT_TIME 15
#define WAIT_TIME_BROADCAST_T1 10
#define WAIT_TIME_BROADCAST_T2 7

#define BroadcastPortT1 5000
#define BroadcastPortT2 5500
#define TCPportT1 5550
#define TCPportT2 5555

#define MESSAGE_T1 "Waiting a massage"
#define MESSAGE_T2 "Send a massage"

#define MAX_SIZE_QUEUE 5

#define servIP "127.0.0.1"

typedef struct mymsg
{
    int T;
    int len;
    char *text;
} Msg;

typedef struct node
{
    struct node *next;
    Msg unit;
} Node;

typedef struct queue
{
    Node *head;
    Node *tail;
    int size;
} Queue;

