#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */

#include "struct.h"

#define RCVBUFSIZE 32   /* Size of receive buffer */

void DieWithError(char *errorMessage);  /* Error handling function */

int main(int argc, char *argv[])
{
    struct sockaddr_in UDPServAddr;   /* Echo server address */
    char recvString[MAX_LEN + 1];     /* Buffer for echo string */
    int bytesRcvd;                    /* Bytes read in single recv() 
                                        and total bytes read */
    
    printf("Client2!\n");
    while (1)
    {
        int UDPsock, broadcastPermission;
    
        if ((UDPsock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
            DieWithError("UDP_socket() failed");

        broadcastPermission = 1;
        if (setsockopt(UDPsock, SOL_SOCKET, SO_BROADCAST, (void *) &broadcastPermission, 
            sizeof(broadcastPermission)) < 0)
            DieWithError("setsockopt() failed");

        /* Construct the server address structure */
        memset(&UDPServAddr, 0, sizeof(UDPServAddr));     /* Zero out structure */
        UDPServAddr.sin_family      = AF_INET;             /* Internet address family */
        UDPServAddr.sin_addr.s_addr = inet_addr(servIP);   /* Server IP address */
        UDPServAddr.sin_port        = htons(BroadcastPortT2);    /* Server port */

        /* Binding UDPsock */
        if (bind(UDPsock, (struct sockaddr *) &UDPServAddr, sizeof(UDPServAddr)) < 0)
            DieWithError("bind() failed");
        
        /* Received a massage from server  */
        if ((bytesRcvd = recvfrom(UDPsock, recvString, MAX_LEN, 0, NULL, 0)) < 0)
            DieWithError("recvd() failed");

        close(UDPsock);
        recvString[bytesRcvd] = '\0';

        printf("Recived: %s\n", recvString);

        if (!strncmp(recvString, MESSAGE_T2, bytesRcvd))
        {
            int TCPsock;
            struct sockaddr_in TCPServAddr;
            int recvBytes;
            Msg msg;
            
            memset(&TCPServAddr, 0, sizeof(TCPServAddr));     /* Zero out structure */
            TCPServAddr.sin_family      = AF_INET;             /* Internet address family */
            TCPServAddr.sin_addr.s_addr = htonl(INADDR_ANY);   /* Server IP address */
            TCPServAddr.sin_port        = htons(TCPportT2);    /* Server port */

            /* Create a reliable, stream socket using TCP */
            if ((TCPsock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
                DieWithError("socket() failed");

            /* Establish the connection to the echo server */
            if (connect(TCPsock, (struct sockaddr *) &TCPServAddr, sizeof(TCPServAddr)) < 0)
                DieWithError("connect() failed");

            /* Send the string to the server */
            if ((recvBytes = recv(TCPsock, &msg, sizeof(msg), 0)) != sizeof(msg))
                DieWithError("send() sent a different number of bytes than expected");

            printf("Recived a massage: %s\n", msg.text);

            close(TCPsock);

            sleep(msg.T);
        }       
    }
    exit(0);
}
