#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */

#include "struct.h"

void DieWithError(char *errorMessage);          /* Error handling function */        
Msg msgGen();                                   /* Massage generator */

int main(int argc, char *argv[])
{
    struct sockaddr_in UDPServAddr;   
    char recvString[MAX_LEN + 1];     
    int bytesRcvd;                     
                                        

    printf("Client1!\n");

    while (1)
    {
        int UDPsock, broadcastPermission;
    
        if ((UDPsock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
            DieWithError("UDP_socket() failed");
        
        // printf("sock created!\n");

        broadcastPermission = 1;
        if (setsockopt(UDPsock, SOL_SOCKET, SO_BROADCAST, (void *) &broadcastPermission, 
            sizeof(broadcastPermission)) < 0)
            DieWithError("setsockopt() failed");

        memset(&UDPServAddr, 0, sizeof(UDPServAddr));     
        // printf("memset is ok\n");
        UDPServAddr.sin_family      = AF_INET;             
        UDPServAddr.sin_addr.s_addr = htonl(INADDR_ANY);   
        UDPServAddr.sin_port        = htons(BroadcastPortT1);    

        if (bind(UDPsock, (struct sockaddr *) &UDPServAddr, sizeof(UDPServAddr)) < 0)
            DieWithError("bind() failed");
        
        // printf("sock is binding\n");

        /* Received a massage from server  */
        if ((bytesRcvd = recvfrom(UDPsock, recvString, MAX_LEN, 0, NULL, 0)) < 0)
            DieWithError("recvd() failed");

        // printf("meassage a recived");

        close(UDPsock);
        recvString[bytesRcvd] = '\0';

        printf("Recived: %s\n", recvString);

        if (!strncmp(recvString, MESSAGE_T1, bytesRcvd))
        {
            printf("cycles is running\n");
            int TCPsock;
            struct sockaddr_in TCPServAddr;

            memset(&TCPServAddr, 0, sizeof(TCPServAddr));     
            TCPServAddr.sin_family      = AF_INET;             
            TCPServAddr.sin_addr.s_addr = inet_addr(servIP);   
            TCPServAddr.sin_port        = htons(TCPportT1);    

            if ((TCPsock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
                DieWithError("socket() failed");

            if (connect(TCPsock, (struct sockaddr *) &TCPServAddr, sizeof(TCPServAddr)) < 0)
                DieWithError("connect() failed");

            Msg msg = msgGen();      

            printf("text: %s\n", msg.text);
            printf("len: %d\n", msg.len);
            printf("Time: %d\n", msg.T);

            if (send(TCPsock, &msg, sizeof(msg), 0) != sizeof(msg))   
                DieWithError("send() failed");
            else 
                printf("message is sending\n");

            close(TCPsock);

            sleep(msg.T);
        }       
    }
    exit(0);
}
