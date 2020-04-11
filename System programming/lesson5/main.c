#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

void *reverse_func(void* arg){
    int length = strlen(arg);
    char *str = (char*)malloc(length * sizeof(char));
    strcpy(str, arg); 
    int c, i, j;

    for (i = 0, j = length - 1; i < j; i++, j--){
        c = str[i];
        str[i] = str[j];
        str[j] = c;
    }

    // printf("%s\n", str);
    pthread_exit((void*)str);
}

int main(int argc, char **argv){
    int result;
    pthread_t thread[argc];
    void *status[argc];

    for (int i = 1; i < argc; i++){
        result = pthread_create(&thread[i], NULL, reverse_func, &argv[i]);

        if (result != 0){
            perror("pthread_create");
            return EXIT_FAILURE;
        }
    }

    for (int i = 1; i < argc; i++){
        result = pthread_join(thread[i], &status[i]);

        if (result != 0){
            perror("pthread_create");
            return EXIT_FAILURE;
        }else {
			printf("square[%d]=%s\n", i,  ((char*)status[i]));
		}
		// free(status[i]);
    }
    
    return EXIT_SUCCESS;
}