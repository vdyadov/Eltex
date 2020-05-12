#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/msg.h>

#include "struct.h"

struct msqid_ds q;
// Msg msg;

int status = 2;

pid_t pidB1, pidB2, pidT1, pidT2;

void DieWithError(char *errorMessage);

void BroadcastClientT1(){
    int sock, broadcastPermission;
    struct sockaddr_in servAddr;

    pid_t pidB1 = fork();

    if (pidB1 == -1)
        DieWithError("forkB1() failed:");
    
    if (pidB1 == 0)
    {
        printf("pid B1 is runnig!!!\n");
        if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
            DieWithError("socketB1() failed:");

        broadcastPermission = 1;
        if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void *) &broadcastPermission, 
            sizeof(broadcastPermission)) < 0)
            DieWithError("setsockopt() failed");

        memset(&servAddr, 0, sizeof(servAddr));
        servAddr.sin_family = AF_INET;
        servAddr.sin_addr.s_addr = inet_addr(servIP);
        servAddr.sin_port = htons(BroadcastPortT1);

        while (1)
        {
            int size = q.msg_qnum;

            printf("Queue size b1: %d\n", size);

            if (size < MAX_SIZE_QUEUE)
                if (sendto(sock, MESSAGE_T1, strlen(MESSAGE_T1), 0, (struct sockaddr *)
                 &servAddr, sizeof(servAddr)) != strlen(MESSAGE_T1))
                    DieWithError("sendB1() failed: ");

            sleep(WAIT_TIME_BROADCAST_T1);            
        }
        close(sock); 
        exit(2);
    }
}

void BroadcastClientT2(){
    int sock, broadcastPermission;
    struct sockaddr_in servAddr;

    pid_t pidB2 = fork();

    if (pidB2 == -1)
        DieWithError("forkB2() failed:");
    
    if (pidB2 == 0)
    {
        printf("pid B2 is runnig!!!\n");
        if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
            DieWithError("sockB2() failed:");

        broadcastPermission = 1;
        if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void *) &broadcastPermission, 
            sizeof(broadcastPermission)) < 0)
            DieWithError("setsockopt() failed");
    
        memset(&servAddr, 0, sizeof(servAddr));
        servAddr.sin_family = AF_INET;
        servAddr.sin_addr.s_addr = inet_addr(servIP);
        servAddr.sin_port = htons(BroadcastPortT2);

        while (1)
        {
            int size = q.msg_qnum;

            printf("Queue size b2: %d\n", size);

            if (size > 0)
                if(sendto(sock, MESSAGE_T2, strlen(MESSAGE_T2), 0, (struct sockaddr *)
                 &servAddr, sizeof(servAddr)) != strlen(MESSAGE_T2))
                    DieWithError("sendB2() failed:");
            
            sleep(WAIT_TIME_BROADCAST_T2);
        }
        close(sock);
        exit(status);
    }
    
    
}

void TCPHandleT1(int msqid){
    int sockServ, sockClient;
    struct sockaddr_in servAddr, clientAddr;
    int bytesRecv;
    unsigned int clntLen;

    pid_t pidT1 = fork();

    if (pidT1 == -1)
        DieWithError("forkT1() failed:");
    
    if (pidT1 == 0)
    {
        printf("pid T1 is runnig!!!\n");
        if((sockServ = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
            DieWithError("sockServT1() failed:");
    
        memset(&servAddr, 0, sizeof(servAddr));
        servAddr.sin_family = AF_INET;
        servAddr.sin_addr.s_addr = inet_addr(servIP);
        servAddr.sin_port = htons(TCPportT1);

        if (bind(sockServ, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
            DieWithError("bindT1() failed:");

        if (listen(sockServ, MAXPENDING) < 0)
            DieWithError("listenT1() failed:");

        while (1)
        {
            clntLen = sizeof((clientAddr));
            if ((sockClient = accept(sockServ, (struct sockaddr *) &clientAddr, &clntLen)) < 0)
                DieWithError("aceptT1() failed:");
            
            if (q.msg_qnum < MAX_SIZE_QUEUE)
            {
                Msg msg;

                if ((bytesRecv = recv(sockClient, &msg, sizeof(Msg), 0)) < 0)
                    DieWithError("recvT1() failed:");

                int len = sizeof(msg) - sizeof(long);
                int snd;

                if ((snd = msgsnd(msqid, &msg, len, 0)) < 0)
                    DieWithError("msgsnd() failed\n");
                
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

    pid_t pidT2 = fork();

    if (pidT2 == -1)
        DieWithError("forkT2() failed:");

    if (pidT2 == 0){
        printf("pid T2 is runnig!!!\n");
        if ((sockServ = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
            DieWithError("sockT2() failed");

        memset(&servAddr, 0, sizeof(servAddr));
        servAddr.sin_family = AF_INET;
        servAddr.sin_addr.s_addr = inet_addr(servIP);
        servAddr.sin_port = htons(TCPportT2);

        if (bind(sockServ, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
            DieWithError("bindT2() failed:");
        
        if (listen(sockServ, MAXPENDING) < 0)  
            DieWithError("listenT2() failed");

        while (1)
        {
            clntLen = sizeof(clntAddr);
            if ((sockClient = accept(sockServ, (struct sockaddr *) &clntAddr, &clntLen)) < 0)
                DieWithError("acceptT2() failed");

            if(q.msg_qnum > 0){
                Msg msg;

                int len = sizeof(msg) - sizeof(long);
                int rc;

                if ((rc = msgrcv(msqid, &msg, len, 0, 0)) < 0)
                    DieWithError("msgrcv() failed");

                if (send(sockClient, &msg, sizeof(msg), 0) < 0)
                    DieWithError("recvT2() failed");
            }
            close(sockClient);
        }
        close(sockServ); 
        exit(status);       
    }
    
}

int main(int argc, char *argv[]){
    int key, msqid;

    if ((key = ftok(".", 'S')) < 0)
        DieWithError("ftok() failed");
    
    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0)
        DieWithError("msgget() failed");
    
    BroadcastClientT1();

    BroadcastClientT2();

    TCPHandleT1(msqid);
    
    TCPHandleT2(msqid);

    waitpid(pidB1, &status, 0);
    if (WIFEXITED(status))
        printf("process is over, return: %d\n", status);

    waitpid(pidB2, &status, 0);
    if (WIFEXITED(status))
        printf("process is over, return: %d\n", status);

    waitpid(pidT1, &status, 0);
    if (WIFEXITED(status))
        printf("process is over, return: %d\n", status);

    waitpid(pidT2, &status, 0);
    if (WIFEXITED(status))
        printf("process is over, return: %d\n", status);

    if (msgctl(msqid, IPC_RMID, NULL) < 0)
        DieWithError("msgctl() failed");
    

    return 0;
}
