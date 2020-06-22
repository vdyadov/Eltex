#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */

#include "struct.h"
#include "msg.pb-c.h"

int main(int argc, char *argv[])
{
    printf("Client1!\n");

    while (1)
    {    
        struct sockaddr_in UDPServAddr;   /* Echo server address */
        char recvString[MAX_LEN + 1];     /* Buffer for echo string */
        int bytesRcvd, UDPsock;           /* Bytes read in single recv() 
                                             and UDP scoket */

        if ((UDPsock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
            DieWithError("UDP_socket() failed");

        const int broadcastPermission = 1;
        if (setsockopt(UDPsock, SOL_SOCKET, SO_REUSEADDR, (void *) &broadcastPermission,
            sizeof(broadcastPermission)) < 0)
            DieWithError("REUSEADDR failed\n");
        
        if (setsockopt(UDPsock, SOL_SOCKET, SO_BROADCAST, (void *) &broadcastPermission, 
            sizeof(broadcastPermission)) < 0)
            DieWithError("setsockopt() failed");

        memset(&UDPServAddr, 0, sizeof(UDPServAddr));           /* Zero out structure */
        UDPServAddr.sin_family      = AF_INET;                  /* Internet address family */
        UDPServAddr.sin_addr.s_addr = htonl(INADDR_ANY);        /* Server IP address */
        UDPServAddr.sin_port        = htons(BroadcastPortT1);   /* Server port */

        if (bind(UDPsock, (struct sockaddr *) &UDPServAddr, sizeof(UDPServAddr)) < 0)
            DieWithError("bind() failed");

        /* Received a massage from server  */
        if ((bytesRcvd = recvfrom(UDPsock, recvString, MAX_LEN, 0, NULL, 0)) < 0)
            DieWithError("recvd() failed");

        close(UDPsock);
        recvString[bytesRcvd] = '\0';

        printf("\nRECEIVED: %s\n", recvString);

        if (!strncmp(recvString, MESSAGE_T1, bytesRcvd))
        {
            int TCPsock;
            struct sockaddr_in TCPServAddr;

            memset(&TCPServAddr, 0, sizeof(TCPServAddr));      /* Zero out structure */
            TCPServAddr.sin_family      = AF_INET;             /* Internet address family */
            TCPServAddr.sin_addr.s_addr = htonl(INADDR_ANY);   /* Server IP address */
            TCPServAddr.sin_port        = htons(TCPportT1);    /* Server port */

            if ((TCPsock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
                DieWithError("socket() failed");

            if (setsockopt(TCPsock, SOL_SOCKET, SO_REUSEADDR, (void *) &broadcastPermission,
                sizeof(broadcastPermission)) < 0)
                DieWithError("REUSEADDR failed\n");

            if (connect(TCPsock, (struct sockaddr *) &TCPServAddr, sizeof(TCPServAddr)) < 0)
                DieWithError("connect() failed");

            uint8_t *buf = NULL;
            size_t size;

            Msg msg = msgGen();  

            printf("type: %ld\n", msg.mtype);  
            printf("MESSAGE TO SEND: %s\n", msg.text);
            printf("SLEEP TIME: %d\n", msg.T);
            printf("Len: %d\n", msg.len);

            size = packProtobuf(msg, &buf);

            printf("size: %ld\n", size);

            /* Send message to the server */
            if (send(TCPsock, buf, size, 0) != size)
                DieWithError("send() failed");

            Msg msg1 = unpackProtobuf(buf);

            printf("MESSAGE TO SEND: %s\n", msg1.text);
            printf("SLEEP TIME: %d\n", msg1.T);
            printf("Len: %d\n", msg1.len);
            printf("typeM1: %ld\n", msg1.mtype);

            free(buf);
            close(TCPsock);
            sleep(msg.T);
        }       
    }
    exit(0);
}