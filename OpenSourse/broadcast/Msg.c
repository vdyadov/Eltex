#include <stdio.h>  /* for perror() */
#include <stdlib.h> /* for exit() */

#include "struct.h"

Msg msgGen()
{
    Msg msg;

    msg.len = rand() % (MAX_LEN - 1);

    msg.T = rand() % MAX_WAIT_TIME;

    msg.text = (char *)malloc(msg.len * sizeof(char));

    for (int i = 0; i < msg.len; i++)
        msg.text[i] = 20 + rand() % MAX_LEN;

    msg.text[msg.len] = '\0';

    return msg;
}

void DieWithError(char *errorMessage)
{
    perror(errorMessage);
    exit(1);
}
