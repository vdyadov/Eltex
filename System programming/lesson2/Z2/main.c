#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <malloc.h>

#define MAX_LENGTH 512
#define FIFO "/tmp/chat"

int main(int argc, char **argv){
    if (argc < 2){
        printf("Не был считан ключ!");
        exit(2);
    }
    int writefd, readfd;
    size_t len;
    char *buff = (char*)malloc((MAX_LENGTH + 1) * sizeof(char)); 
    mkfifo(FIFO, 0666);

    if (strcmp(argv[1], "-r") == 0){
        printf("Напишите какой нибудь текст: \n");
        writefd = open(FIFO, O_WRONLY);
        while (strcmp(buff, "quit") != 0){
            
            fgets(buff, MAX_LENGTH, stdin);
            // scanf("%s", buff);
            len = strlen(buff);
            write(writefd, buff, len);
        }
        close(writefd);
    }
    if (strcmp(argv[1], "-w") == 0){
        printf("Этот же текст:\n");
        readfd = open(FIFO, O_RDONLY);
        while (1){
            len = read(readfd, buff, MAX_LENGTH);
            printf("%s", buff);
            buff = (char*)malloc((MAX_LENGTH + 1) * sizeof(char));
            if (strcmp(buff, "quit") == 0){
                exit(0);
                free(buff);
            }
        }
        close(readfd);
    }
    return 0;
}
