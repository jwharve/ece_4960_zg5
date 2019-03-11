#ifndef DEF_H
#define DEF_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <wiringPi.h>
#include <wiringSerial.h>

#define MOVE '1' /* move type */
#define DRAW '2' /* move type */
#define CLEAR '1' /* tool type */
#define BLUNT '2' /* tool type */
#define FINE '3' /* tool type */
#define RAKE '4' /* tool type */
#define NOTOOL '5' /* id for no tool */

#define DRAW_HEIGHT 5 /* height in inches for drawing */
#define MOVE_HEIGHT 6 /* height in inches for moving */

#define DIST_WEIGHT 4 /* scale factor for 1 inch distance to number of steps */
#define ANGLE_WEIGHT 1 /* scale factor for 1 degree rotation to number of steps */

// Tool Locations
#define TOOLCLEARX 1
#define TOOLCLEARY 1
#define TOOLBLUNTX 2
#define TOOLBLUNTY 2
#define TOOLFINEX 3
#define TOOLFINEY 3
#define TOOLRAKEX 4
#define TOOLRAKEY 4

#define MOUNT_RADIUS 0.5 /* Radius of holes on mount */
#define MOUNT_HEIGHT 2 /* Height from tool tip to mount holes on mount */
#define POST_HEIGHT 18 /* Height of hole on post */
#define POST_RADIUS 18.33333333 /* Radius of holes on posts */

#define CIRC 0.375 /* circumference of the spool */

#define DTR 0.01745329251   /* degrees to radians (pi/180) */

#define SMALL 0.00001

#define SEND_DELAY 750

struct gLine/* packed */
{
	char moveType;
	char tool;
	float x;
	float y;
	float theta;
};

struct point
{
	float x;
	float y;
	float z;
} h0, h1, h2, post0, post1, post2;

struct packet
{
	int S0;
	int S1;
	int S2;
	int R;
	char E;
} __attribute__((packed));

#endif
