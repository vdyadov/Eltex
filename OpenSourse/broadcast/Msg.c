#include <stdio.h>  /* for perror() */
#include <stdlib.h> /* for exit() */
#include <time.h>   /* time for srand */

#include "struct.h"

Msg msgGen()
{
    Msg msg;

    msg.mtype = 1;

    srand(time(NULL));

    msg.len = rand() % (MAX_LEN);

    msg.T = rand() % MAX_WAIT_TIME;
    
    /* Create text a message */
    for (int i = 0; i < msg.len; i++)
        msg.text[i] = 65 + rand() % 60;

    msg.text[msg.len] = '\0';

    return msg;
}

void DieWithError(char *errorMessage)  /* Error hadler */
{
    perror(errorMessage);
    exit(1);
}
