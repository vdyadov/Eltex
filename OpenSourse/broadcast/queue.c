#include <stdio.h>
#include <stdlib.h>

#include "struct.h"

void qPush(Queue *q, Msg msg){
    Node *node = (Node *)malloc(sizeof(Node));

    node -> unit = msg;
    node -> next = NULL;

    if (q->head && q->tail)         /* if messages are in the queue */
    {
        q->tail->next = node;
        q->tail = node;
        
        q->size++;
    }else{                          /* if there are no a messages in the queue */
        q->head = q->tail = node;
        q->size = 1;
    }
}

void qPop(Queue *q, Msg *msg){
    if (q->head){                   /* if messages are in the queue */
        q->size--;
        Node *node = q->head;
        q->head = q->head->next;
        (*msg) =  node->unit;

        free(node);
    }else
        return;
}

void qClear(Queue *q){
    while (q->head != NULL)
    {
        Node * node = q->head;
        q->head = q->head->next;
        free(node);
    }
}