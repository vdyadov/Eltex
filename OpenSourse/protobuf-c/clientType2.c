#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */

#include "msg.pb-c.h"
#include "struct.h"

int main(int argc, char *argv[])
{
    printf("Client2!\n");
    while (1)
    {    
        struct sockaddr_in UDPServAddr;   /* Echo server address */
        char recvString[MAX_LEN + 1];     /* Buffer for echo string */
        int bytesRcvd, UDPsock;           /* Bytes read in single recv() 
                                             and UDP sock */

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
        UDPServAddr.sin_port        = htons(BroadcastPortT2);   /* Server port */

        if (bind(UDPsock, (struct sockaddr *) &UDPServAddr, sizeof(UDPServAddr)) < 0)
            DieWithError("bind() failed");
        
        /* Received a massage from server  */
        if ((bytesRcvd = recvfrom(UDPsock, recvString, MAX_LEN, 0, NULL, 0)) < 0)
            DieWithError("recvd() failed");

        close(UDPsock);
        recvString[bytesRcvd] = '\0';

        printf("\nRECEIVED: %s\n", recvString);

        if (!strncmp(recvString, MESSAGE_T2, bytesRcvd))
        {
            int TCPsock, bytesRecv;
            struct sockaddr_in TCPServAddr;
            
            memset(&TCPServAddr, 0, sizeof(TCPServAddr));      /* Zero out structure */
            TCPServAddr.sin_family      = AF_INET;             /* Internet address family */
            TCPServAddr.sin_addr.s_addr = htonl(INADDR_ANY);   /* Server IP address */
            TCPServAddr.sin_port        = htons(TCPportT2);    /* Server port */

            if ((TCPsock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
                DieWithError("socket() failed");

            if (setsockopt(TCPsock, SOL_SOCKET, SO_REUSEADDR, (void *) &broadcastPermission,
                sizeof(broadcastPermission)) < 0)
                DieWithError("REUSEADDR failed\n");

            if (connect(TCPsock, (struct sockaddr *) &TCPServAddr, sizeof(TCPServAddr)) < 0)
                DieWithError("connect() failed");

            char buffer[MAX_LEN];
            uint8_t *buf = NULL;
            size_t size = 0;

            /* Received message from the server */
            if ((bytesRecv = recv(TCPsock, buffer, MAX_LEN, 0)) < 0)
                    DieWithError("recvT1() failed");

            size += bytesRecv;

            buf = (uint8_t *)malloc(sizeof(uint8_t) * size);

            for (int i = 0; i < size; i++)
                buf[i] = buffer[i];

            while (bytesRecv > 0)
            {
                if ((bytesRecv = recv(TCPsock, buffer, MAX_LEN, 0)) < 0)
                    DieWithError("recvT1() failed");

                buf = (uint8_t *)realloc(buf, (size + bytesRecv) * sizeof(uint8_t));

                for (int i = 0; i < bytesRecv; i++)
                    buf[i + size] = buffer[i];
                    
                size += bytesRecv;
            }

            Msg msg = unpackProtobuf(buf);
            
            printf("RECEIVED A MESSAGE: %s\n", msg.text);
            printf("SLEEP TIME: %d\n", msg.T);

            free(buf);
            close(TCPsock);
            sleep(msg.T);
        }       
    }
    exit(0);
}
