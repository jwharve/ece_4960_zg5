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

#define DRAW_HEIGHT 2.5 /* height in inches for drawing */
#define MOVE_HEIGHT 5. /* height in inches for moving */
#define TOOL_HEIGHT 1.5 /* height in inches for other tools */
#define CLEAR_TOOL_HEIGHT 1.6 /* height in inches for clear tool */
#define FINE_HEIGHT 1.8
#define RAKE_BLUNT_HEIGHT 1.5


#define DIST_WEIGHT 20. /* scale factor for 1 inch distance to number of steps */
#define ANGLE_WEIGHT 1.5 /* scale factor for 1 degree rotation to number of steps */

// Tool Locations
#define TOOLCLEARX 0.
#define TOOLCLEARY 10.5
#define TOOLBLUNTX -1.5
#define TOOLBLUNTY -9.5
#define TOOLFINEX 0.0
#define TOOLFINEY -12.25
#define TOOLRAKEX 1.25
#define TOOLRAKEY -9.25


#define MOUNT_RADIUS 0.5 /* Radius of holes on mount */
//#define MOUNT_HEIGHT (1.52+2.79+0.25) /* Height from tool tip to mount holes on mount */
#define MOUNT_HEIGHT 4.7
#define POST_HEIGHT 15 /* Height of hole on post */
#define POST_RADIUS (18.33333333-0.8) /* Radius of holes on posts */

#define CIRC 1.3 /* circumference of the spool */

#define DTR 0.01745329251   /* degrees to radians (pi/180) */

#define SMALL 0.00001

#define SEND_DELAY 75

#define FILE_NAME_LEN 100

#define ANGLE_TO_STEPS ((float)4800.0/360.)

#define FINE_DROP 0.2

#define RUN 0
#define EXIT 1

#define SLOPE_X (-0.2/6)
#define SLOPE_Y (0.25/6)
#define SLOPE_Y_ABS (0.1/6)

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
	float theta;
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
