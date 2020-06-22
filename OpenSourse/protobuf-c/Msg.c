#include <stdio.h>  /* for perror() */
#include <stdlib.h> /* for exit() */
#include <time.h>   /* time for srand */
#include <string.h>

#include "msg.pb-c.h"
#include "struct.h"

Msg msgGen()
{
    Msg msg;

    srand(time(NULL));

    msg.mtype = 1;

    msg.len = rand() % MAX_LEN;

    msg.T = rand() % MAX_WAIT_TIME;
    
    /* Create text a message */
    for (int i = 0; i < msg.len; i++)
        msg.text[i] = 65 + rand() % 60;

    msg.text[msg.len] = '\0';

    return msg;
}

size_t packProtobuf(Msg msg, uint8_t **buf)
{
    TCPMessage TCPmsg = TCPMESSAGE__INIT;

    int msgSize = strlen(msg.text);

    TCPmsg.text = (char *)malloc(sizeof(char) * (msgSize + 1));
    strcpy(TCPmsg.text, msg.text);
    TCPmsg.len = msg.len;
    TCPmsg.t = msg.T;
    TCPmsg.type = msg.mtype;


    size_t len = tcpmessage__get_packed_size(&TCPmsg);
    (*buf) = (uint8_t *)malloc(len * sizeof(uint8_t));
    tcpmessage__pack(&TCPmsg, *buf);

    return len;
}

Msg unpackProtobuf(uint8_t * buf) 
{
    printf("func runing\n");
    TCPMessage * tcpmsg;
	// printf("TCPMessage create\n");
	size_t msg_len = strlen((char *)buf);
    // printf("strlen is ok\n");
    // buf = (uint8_t *)malloc(msg_len * sizeof(uint8_t));
	
	tcpmsg = tcpmessage__unpack(NULL, msg_len, buf);

    printf("message unpack\n");
	
    Msg msg /* = {"", 1, 0, 0} */;
    // printf("msg created\n");
	if (tcpmsg == NULL)
		return msg;
	strcpy(msg.text, tcpmsg->text);
    msg.mtype = tcpmsg->type;
	msg.len = tcpmsg->len;
	msg.T = tcpmsg->t;
	printf("3-3\n");
	tcpmessage__free_unpacked(tcpmsg, NULL);
	printf("3-4\n");
	return msg;
}

void DieWithError(char *errorMessage)  /* Error handler */
{
    perror(errorMessage);
    exit(1);
}

// TCPMessage msgGen()
// {
//     TCPMessage msg;

//     msg.type = 1;

//     srand(time(NULL));

//     msg.len = rand() % MAX_LEN;

//     msg.t = rand() % MAX_WAIT_TIME;

//     msg.text = (char *)malloc(sizeof(char) * (msg.len + 1));
    
//     /* Create text a message */
//     for (int i = 0; i< msg.len; i++)
//         msg.text[i] = 65 +  rand() % 60;

//     msg.text[msg.len] = '\0';

//     return msg;
// }

// size_t packProtobuf(TCPMessage msg, uint8_t **buf)
// {
//     // TCPMessage TCPmsg = TCPMESSAGE__INIT;

//     // int msgSize = strlen(msg.text);

//     size_t len;

//     msg.text = (char *)realloc(msg.text, sizeof(char) * (msg.len + 1));
//     // strcpy(TCPmsg.text, msg.text); 
//     // TCPmsg.type = msg.type;
//     // TCPmsg.len = msg.len;
//     // TCPmsg.t = msg.t;

//     printf("msg text: %s", msg.text);
//     printf("msg.len: %d", msg.len);
//     printf("msg.type: %d", msg.type);
//     printf("msg.T: %d", msg.t);

//     (*buf) = (uint8_t *)malloc(msg.len * sizeof(uint8_t));
//     len = tcpmessage__get_packed_size(&msg);
//     tcpmessage__pack(&msg, *buf);

//     return len;
// size_t packProtobuf(TCPMessage msg, uint8_t **buf)
// {
//     // TCPMessage TCPmsg = TCPMESSAGE__INIT;

//     // int msgSize = strlen(msg.text);

//     size_t len;

//     msg.text = (char *)realloc(msg.text, sizeof(char) * (msg.len + 1));
//     // strcpy(TCPmsg.text, msg.text); 
//     // TCPmsg.type = msg.type;
//     // TCPmsg.len = msg.len;
//     // TCPmsg.t = msg.t;

//     printf("msg text: %s", msg.text);
//     printf("msg.len: %d", msg.len);
//     printf("msg.type: %d", msg.type);
//     printf("msg.T: %d", msg.t);

//     (*buf) = (uint8_t *)malloc(msg.len * sizeof(uint8_t));
//     len = tcpmessage__get_packed_size(&msg);
//     tcpmessage__pack(&msg, *buf);

//     return len;
// }