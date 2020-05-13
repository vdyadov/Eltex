#define MAXPENDING 255
#define MAX_LEN 128
#define BUF_SIZE 128

#define MAX_WAIT_TIME 15
#define WAIT_TIME_BROADCAST_T1 5
#define WAIT_TIME_BROADCAST_T2 3

#define BroadcastPortT1 7000
#define BroadcastPortT2 7700
#define TCPportT1 7770
#define TCPportT2 7777

#define MESSAGE_T1 "Waiting a massage"
#define MESSAGE_T2 "Send a massage"

#define MAX_SIZE_QUEUE 5

#define servIP "127.0.0.1"

typedef struct mymsg
{
    int T;
    int len;
    char text[MAX_LEN + 1];
} Msg;

