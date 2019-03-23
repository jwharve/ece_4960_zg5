#ifndef MOVE_H
#define MOVE_H

#include "def.h"

void move(struct point prev, struct point next, char E, int uart_port);

void swapTool(struct point prev, struct point next, char prevTool, char nextTool, int uart_port);

int numSteps(struct point prev, struct point next);

struct packet calcStep(float x, float y, float z, float theta, char E);

int rot2steps(float rot);

int dist2steps(float dist);

float * interp(float one, float two, unsigned long num);

struct point addP(struct point one, struct point two);

float distance(struct point one, struct point two);

#endif
