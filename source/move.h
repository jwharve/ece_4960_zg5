#ifndef MOVE_H
#define MOVE_H

#include "def.h"

int dist2steps(float dist);

int rot2steps(float rot);

struct packet calcStep(float x, float y, float z, float theta, char E);

void swapTool(struct gLine * prev, struct gLine * curr, int uart_port, float z_curr);

void move(struct gLine * prev, struct gLine * curr, float prev_z, float curr_z, unsigned char E, int uart_port);

int numSteps(struct gLine prev, struct gLine curr);

#endif
