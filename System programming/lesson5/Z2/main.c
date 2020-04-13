#include <pthread.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <wait.h> 
#include <fcntl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#define PTH_MAX 3

struct {
    pthread_mutex_t mutex;
    FILE *fp;
} shared = {PTHREAD_MUTEX_INITIALIZER};

void reverse(char *s){
    int length = strlen(s) ;
    int c, i, j;

    for (i = 0, j = length - 1; i < j; i++, j--)
    {
      c = s[i];
      s[i] = s[j];
      s[j] = c;
    }

    printf("%s\n\n", s);
}

void *rev_func(void* arg){
    int pth_id = (int)arg;

    while (1){
        pthread_mutex_lock(&shared.mutex);

        char str[50];
        char *pstr = fgets(str, 50, shared.fp);

        pthread_mutex_unlock(&shared.mutex);
        if (pstr == NULL){
            printf("Конец файла\n");
            return 0;
        }
        printf("Поток № %d: ", pth_id);
        reverse(str);
    }
    pthread_exit(0);
}

int main (int argc, char **argv){
    if (argc < 2){
        printf("Вы не ввели имя файла\n");
        exit(1);
    }
    // FILE *fp;

    shared.fp = fopen(argv[1], "r");
    
    if(shared.fp == NULL){
        printf("Файл не открыт\n");
        exit(1);
    }   

    pthread_t thread[PTH_MAX];

    int result;

    for (int i = 0; i < PTH_MAX; i++){
        if ((result = pthread_create(&thread[i], NULL, rev_func, (void *) i)) != 0){
            perror("pth_create");
            return EXIT_FAILURE;
        }        
    }

    for (int i = 0; i < PTH_MAX; i++){
        if ((result = pthread_join(thread[i], NULL) != 0)){
            perror("pth_join");
            return EXIT_FAILURE;
        }        
    }
    fclose(shared.fp);

    return 0;
}