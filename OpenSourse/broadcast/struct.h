#define MAXPENDING 15
#define MAX_LEN 128

#define MAX_WAIT_TIME 15
#define WAIT_TIME_BROADCAST_T1 5
#define WAIT_TIME_BROADCAST_T2 3

#define BroadcastPortT1 5000
#define BroadcastPortT2 5500
#define TCPportT1 5550
#define TCPportT2 5555

#define MESSAGE_T1 "Waiting a massage"
#define MESSAGE_T2 "Have a massage"

#define MAX_SIZE_QUEUE 5

#define servIP "127.0.0.1"

typedef struct mymsgbuf
{
    long mtype;
    int len;
    int T;
    char text[MAX_LEN + 1];
} Msg;

void DieWithError(char *errorMessage);

Msg msgGen();