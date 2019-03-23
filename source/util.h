#ifndef UTIL_H
#define UTIL_H
#include "def.h"

extern int zero_step;

void initGlobal(void);

int readLine(FILE * fptr, struct gLine * next);

void sendPacket(struct packet p, int uart_port);

void printPacket(struct packet p);

#endif
