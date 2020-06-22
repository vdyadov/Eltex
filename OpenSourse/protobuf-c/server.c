#include <stdio.h>          /* for printf() and fprintf() */ 
#include <stdlib.h>         /* for atoi() and exit() */
#include <string.h>         /* for memset() */ 
#include <unistd.h>         /* for close() */
#include <arpa/inet.h>      /* for sockaddr_in and inet_addr() */
#include <sys/types.h>      /* for key_t and pid_t */
#include <sys/wait.h>       /* for waitpid() */
#include <sys/socket.h>     /* for socket(), connect(), send(), and recv() */
#include <sys/msg.h>        /* for msgget(), msgctl() */

#include "msg.pb-c.h"
#include "struct.h"

struct msqid_ds q;

int status = 2;

pid_t pidB1, pidB2, pidT1, pidT2;

void BroadcastClientT1(int msqid){
    int sock;
    struct sockaddr_in servAddr;

    pidB1 = fork();

    if (pidB1 == -1)
        DieWithError("forkB1() failed");
    
    if (pidB1 == 0)
    {
        // printf("pid B1 is runnig!!!\n");
        if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
            DieWithError("socketB1() failed");

        const int broadcastPermission = 1;
        if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void *) &broadcastPermission, 
            sizeof(broadcastPermission)) < 0)
            DieWithError("setsockopt() failed");

        memset(&servAddr, 0, sizeof(servAddr));               /* Zero out structure */
        servAddr.sin_family      = AF_INET;                   /* Internet address family */
        servAddr.sin_addr.s_addr = inet_addr(servIP);         /* Server IP address */
        servAddr.sin_port        = htons(BroadcastPortT1);    /* Server port */

        while (1)
        {
            if (msgctl(msqid, IPC_STAT, &q) < 0)    /* get size queue*/
                DieWithError("msgctlT1() failed");
                
            int size = q.msg_qnum;

            if (size < MAX_SIZE_QUEUE){
                printf("Queue size B1: %d\n", size);
                
                /* send message to clientT2 */
                if (sendto(sock, MESSAGE_T1, strlen(MESSAGE_T1), 0, (struct sockaddr *)
                 &servAddr, sizeof(servAddr)) != strlen(MESSAGE_T1))
                    DieWithError("sendB1() failed");
            }
            sleep(WAIT_TIME_BROADCAST_T1);            
        }
        close(sock); 
        exit(2);
    }
}

void BroadcastClientT2(int msqid){
    int sock;
    struct sockaddr_in servAddr;

    pidB2 = fork();

    if (pidB2 == -1)
        DieWithError("forkB2() failed");
    
    if (pidB2 == 0)
    {
        // printf("pid B2 is runnig!!!\n");
        if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
            DieWithError("sockB2() failed");

        const int broadcastPermission = 1;
        if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void *) &broadcastPermission, 
            sizeof(broadcastPermission)) < 0)
            DieWithError("setsockopt() failed");
    
        memset(&servAddr, 0, sizeof(servAddr));             /* Zero out structure */
        servAddr.sin_family      = AF_INET;                 /* Internet address family */
        servAddr.sin_addr.s_addr = inet_addr(servIP);       /* Server IP address */
        servAddr.sin_port        = htons(BroadcastPortT2);  /* Server port */

        while (1)
        {
            if (msgctl(msqid, IPC_STAT, &q) < 0)    /* get size queue */
                DieWithError("msgctlT1() failed");

            int size = q.msg_qnum;

            if (size > 0){
                printf("Queue size B2: %d\n", size);

                /* send message to clientT2 */
                if(sendto(sock, MESSAGE_T2, strlen(MESSAGE_T2), 0, (struct sockaddr *)
                 &servAddr, sizeof(servAddr)) != strlen(MESSAGE_T2))
                    DieWithError("sendB2() failed");
            }
            sleep(WAIT_TIME_BROADCAST_T2);
        }
        close(sock);
        exit(status);
    }
}

void TCPHandleT1(int msqid){
    int sockServ, sockClient, bytesRecv;
    struct sockaddr_in servAddr, clientAddr;
    unsigned int clntLen;

    pidT1 = fork();

    if (pidT1 == -1)
        DieWithError("forkT1() failed");
    
    if (pidT1 == 0)
    {
        // printf("pid T1 is runnig!!!\n");
        if((sockServ = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
            DieWithError("sockServT1() failed");
    
        memset(&servAddr, 0, sizeof(servAddr));         /* Zero out structure */
        servAddr.sin_family      = AF_INET;             /* Internet address family */
        servAddr.sin_addr.s_addr = inet_addr(servIP);   /* Server IP address */
        servAddr.sin_port        = htons(TCPportT1);    /* Server port */

        if (bind(sockServ, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
            DieWithError("bindT1() failed");

        if (listen(sockServ, MAXPENDING) < 0)
            DieWithError("listenT1() failed");

        while (1)
        {
            clntLen = sizeof((clientAddr));
            if ((sockClient = accept(sockServ, (struct sockaddr *) &clientAddr, &clntLen)) < 0)
                DieWithError("aceptT1() failed");

            if (msgctl(msqid, IPC_STAT, &q) < 0)    /* get size queue */
                DieWithError("msgctlT1() failed");

            if (q.msg_qnum < MAX_SIZE_QUEUE)
            {
                char buffer[MAX_LEN];
                uint8_t *buf = NULL;
                size_t size = 0;

                /* received message from clientT1 */
                if ((bytesRecv = recv(sockClient, buffer, MAX_LEN, 0)) < 0)
                    DieWithError("recvT1() failed");

                size += bytesRecv;

                buf = (uint8_t *)malloc(sizeof(uint8_t) * size);

                for (int i = 0; i < size; i++)
                    buf[i] = buffer[i];

                while (bytesRecv > 0)
                {
                    if ((bytesRecv = recv(sockClient, buffer, MAX_LEN, 0)) < 0)
                        DieWithError("recvT1() failed");

                    buf = (uint8_t *)realloc(buf, (size + bytesRecv) * sizeof(uint8_t));

                    for (int i = 0; i < bytesRecv; i++)
                        buf[i + size] = buffer[i];
                    
                    size += bytesRecv;
                }
                
                Msg msg = unpackProtobuf(buf);

                printf("MESSAGE TO SEND: %s\n", msg.text);
                printf("SLEEP TIME: %d\n", msg.T);
                printf("Len: %d\n", msg.len);
                printf("type: %ld\n", msg.mtype);

                int len = sizeof(msg) - sizeof(long);

                /* send message to queue */
                if (msgsnd(msqid, &msg, len, 0) < 0)
                    DieWithError("msgsnd() failed");     

                free(buf);           
            }
            close(sockClient);
        }   
        close(sockServ);  
        exit(status);      
    }
}

void TCPHandleT2(int msqid){
    int sockServ, sockClient;
    struct sockaddr_in servAddr, clntAddr;
    unsigned int clntLen;

    pidT2 = fork();

    if (pidT2 == -1)
        DieWithError("forkT2() failed");

    if (pidT2 == 0)
    {
        // printf("pid T2 is runnig!!!\n");
        if ((sockServ = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
            DieWithError("sockT2() failed");

        memset(&servAddr, 0, sizeof(servAddr));         /* Zero out structure */
        servAddr.sin_family      = AF_INET;             /* Internet address family */
        servAddr.sin_addr.s_addr = inet_addr(servIP);   /* Server IP address */
        servAddr.sin_port        = htons(TCPportT2);    /* Server port */ 

        if (bind(sockServ, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
            DieWithError("bindT2() failed");
        
        if (listen(sockServ, MAXPENDING) < 0)  
            DieWithError("listenT2() failed");

        while (1)
        {
            clntLen = sizeof(clntAddr);
            if ((sockClient = accept(sockServ, (struct sockaddr *) &clntAddr, &clntLen)) < 0)
                DieWithError("acceptT2() failed");

            if (msgctl(msqid, IPC_STAT, &q) < 0)    /* get size queue */
                DieWithError("msgctlT2() failed");

            if(q.msg_qnum > 0){
                printf("cycle is running\n");
                Msg msg;
                int rc;
                int len = sizeof(msg) - sizeof(long);

                /* received message from queue */
                if ((rc = msgrcv(msqid, &msg, len, 1, IPC_NOWAIT|MSG_NOERROR)) < 0)
                    DieWithError("msgrcv() failed");
                
                printf("msgrcv returned: %d\n", rc);

                uint8_t *buf = NULL;
                size_t size;

                printf("MESSAGE TO SEND: %s\n", msg.text);
                printf("SLEEP TIME: %d\n", msg.T);
                printf("Len: %d\n", msg.len);
                printf("type: %ld\n", msg.mtype);

                size = packProtobuf(msg, &buf);

                printf("send size = %ld\n", size);

                /* send message to clientT2 */
                if (send(sockClient, buf, size, 0) != size)
                    DieWithError("sendT2() failed");

                free(buf);
            }
            close(sockClient);
        }
        close(sockServ); 
        exit(status);       
    }   
}

int main(int argc, char *argv[]){
    int msqid;
    key_t key;

    if ((key = ftok(".", 'S')) < 0)
        DieWithError("ftok() failed");
    
    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0)    
        DieWithError("msgget() failed");

    TCPHandleT1(msqid);
    
    TCPHandleT2(msqid);

    BroadcastClientT1(msqid);

    BroadcastClientT2(msqid );

    waitpid(pidT1, &status, 0);
    if (WIFEXITED(status))
        printf("process T1 is over, return: %d\n", status);

    waitpid(pidT2, &status, 0);
    if (WIFEXITED(status))
        printf("process T2 is over, return: %d\n", status);

    if (msgctl(msqid, IPC_RMID, 0) < 0)     /* Delete queue */
        DieWithError("msgctl() failed");

    waitpid(pidB1, &status, 0);
    if (WIFEXITED(status))
        printf("process B1 is over, return: %d\n", status);

    waitpid(pidB2, &status, 0);
    if (WIFEXITED(status))
        printf("process B2 is over, return: %d\n", status);    

    return 0;
}