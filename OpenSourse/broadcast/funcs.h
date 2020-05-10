#include "struct.h"

void DieWithError(char *errorMessage);

void qPush(Queue *q, Msg msg);

void qPop(Queue *q, Msg *msg);

void qClear(Queue *q);

void BroadcastClientT1();

void BroadcastClientT2();

void TCPHandleT1();

void TCPHandleT2();