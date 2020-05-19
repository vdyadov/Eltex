#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

union semun {
 int val;                  /* значение для SETVAL */
 struct semid_ds *buf;     /* буферы для  IPC_STAT, IPC_SET */
 unsigned short *array;    /* массивы для GETALL, SETALL */
                           /* часть, особенная для Linux: */
 struct seminfo *__buf;    /* буфер для IPC_INFO */
};

const char *path = "./text/";

int main(int argc, char **argv){
    int shmid;
    int semid;
    int status;
    key_t key;
    int *shm;
    union semun arg;
    struct sembuf lok_res = {0, -1, 0}; // блокировка ресурса
    struct sembuf rel_res = {0, 1, 0}; // освобождение ресурса
    
    if ((key = ftok(".", 'S')) < 0){
        printf("Не удалось создать ключ\n");
        exit(1);
    }
    /* Создаем семафор */
    semid = semget(key, 1, 0666|IPC_CREAT);
    
    arg.val = 1;
    semctl(semid, 0, SETVAL, arg);
    /* Создаем разделяемую память */
    if((shmid = shmget(key, sizeof(int), 0666|IPC_CREAT)) < 0){
        perror("semget");
        exit(1);
    }
    /* Получаем доступ */
    if ((shm = shmat(shmid, NULL, 0)) == (int *) -1) {
        perror("shmat");
        exit(1);
    }
    
    (*shm) = 0;

    DIR *d;

    struct dirent *dir;
    d = opendir(path);
    if(d){
        while ((dir = readdir(d)) != NULL){
            char fpath[100];
            strcpy(fpath, path);
            strcat(fpath, dir->d_name);

            pid_t pid = fork();
            if (-1 == pid){
                perror("pid");
                exit(1);
            }
            if (0 == pid){
                printf("Процесс запущен\n");
                FILE *fp;
                if((fp = fopen(fpath, "r")) == NULL){
                    printf("Файл не открыт\n");
                    exit(1);
                }else
                    printf("Файл открыт\n");
                
                int ctr = 0;
                char s;
                while ((s = fgetc(fp)) != EOF){
                    if((shm = (int*)shmat(shmid,NULL, 0)) == (int*)-1){
                    perror("shmat");
                    exit(1);
                    } 

                    if ((semop(semid, &lok_res, 1)) < 0){
                        perror("semop");
                        exit(1);
                    }
                    // printf("Ресурс заблокирован\n");
                
                    (*shm)++;
                    ctr++;

                    if ((semop(semid, &rel_res, 1)) < 0){
                        perror("semop");
                        exit(1);
                    }
                    // printf("Ресурс разблокирован\n");               
                }
                printf("Количество символов в файле = %d\n", ctr);
                printf("Общее количество символов = %d\n", *shm);
                if (shmdt(shm) < 0) {
					printf("Ошибка отключения\n");
					exit(1);
				}
                fclose(fp);
                exit(2);
            }
            waitpid(pid, &status, 0); 
            if (WIFEXITED(status)){
                printf("Процесс звершен. Вернул: %d\n\n", WEXITSTATUS(status));
            }
                       
        }
        closedir(d);
    }

    if ((shm = (int *)shmat(shmid, NULL, 0)) == (int *) -1) {
		perror("shmat");
		exit(1);
	}

	printf("\nВсего символов = %d\n", *shm);
	/* Отключаем и удаляем созданные объекты IPC */
	if (shmdt(shm) < 0) {
		printf("Ошибка отключения\n");
		exit(1);
	} 
		
	if (shmctl(shmid, IPC_RMID, 0) < 0) {
		printf("Невозможно удалить область\n");
		exit(1);
	}
	
	if (semctl(semid, 0, IPC_RMID) < 0) {
		printf("Невозможно удалить семафор\n");
		exit(1);
	}
	
	return 0;
}