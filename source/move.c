#include "move.h"
#include "util.h"

void move(struct point prev, struct point next, char E, int uart_port)
{
	int numPoints;
	float * x;
	float * y;
	float * z;
	float * theta;
	int i;

	printf("PREV\n");
	printf("x = %f, y = %f, z = %f, theta = %f\n", prev.x, prev.y, prev.z, prev.theta);
	printf("NEXT\n");
	printf("x = %f, y = %f, z = %f, theta = %f\n", next.x, next.y, next.z, next.theta);


	numPoints = numSteps(prev, next);

	x = (float * )malloc(numPoints*sizeof(float));
	y = (float * )malloc(numPoints*sizeof(float));
	z = (float * )malloc(numPoints*sizeof(float));
	theta = (float * )malloc(numPoints*sizeof(float));

	x = interp(prev.x,next.x,numPoints);
	y = interp(prev.y,next.y,numPoints);
	z = interp(prev.z,next.z,numPoints);
	theta = interp(prev.theta,next.theta,numPoints);

	for (i = 0; i < numPoints; i++)
	{
		current = calcStep(x[i], y[i], z[i], theta[i], E);
		printPacket(current);
		sendPacket(current, uart_port);
		delay(SEND_DELAY);
	}

	free(x); free(y); free(z); free(theta);
}

void swapTool(struct point prev, struct point next, char prevTool, char nextTool, int uart_port)
{
	return;
	//grab tool initially
	//go to prev tool position and release tool
	//go to new tool position from old tool position and grab new tool

	struct point prevTool,nextTool;
	struct point p1, p2;

	prevTool.z = TOOL_HEIGHT;
	nextTool.z = TOOL_HEIGHT;
	
	if (nextTool == CLEAR)
	{
		nextTool.x = TOOLCLEARX;
		nextTool.y = TOOLCLEARY;
	}
	else if (nextTool == BLUNT)
	{
		nextTool.x = TOOLBLUNTX;
		nextTool.y = TOOLBLUNTY;
	}
	else if (nextTool == FINE)
	{
		nextTool.x = TOOLFINEX;
		nextTool.y = TOOLFINEY;
	}
	else if (nextTool == RAKE)
	{
		nextTool.x = TOOLRAKEX;
		nextTool.y = TOOLRAKEY;
	}

	if (prevTool != NOTOOL)
	{
		if (prevTool == CLEAR)
		{
			prevTool.x = TOOLCLEARX;
			prevTool.y = TOOLCLEARY;

		}
		else if (prevTool == BLUNT)
		{
			prevTool.x = TOOLBLUNTX;
			prevTool.y = TOOLBLUNTY;

		}
		else if (prevTool == FINE)
		{
			prevTool.x = TOOLFINEX;
			prevTool.y = TOOLFINEY;
		}
		else if (prevTool == RAKE)
		{
			prevTool.x = TOOLRAKEX;
			prevTool.y = TOOLRAKEY;
		}
	}

	//go up--pass in prev for previous and current so x and y dont change
	//pass in draw height and move height to raise it
	printf("starting tool change\n");
	p1 = prev;
	p2 = prev;
	p2.z = MOVE_HEIGHT;
	move(p1,p2,1,uart_port);
	if (prev->tool != NOTOOL)
	{
		//go to previous tool position in toolbed, pass in prev for previous position
		//and prevToolLine for new x and y, keep heights the same
		printf("move to prev tool x y\n");
		p1 = prevTool;
		p1.z = MOVE_HEIGHT;
		move(p2,p1,1,uart_port);
		//lower height to prepare for detachment
		printf("go down\n");
		p2 = prevTool;
		move(p1,p2,1,uart_port);
		//release electromagnet
		printf("release\n");
		move(p2,p2,0,uart_port);
		//raise up without tool
		printf("go up\n");
		p1 = p2;
		p1.z = MOVE_HEIGHT;
		move(p2,p1,0,uart_port);
		//move to new tool
		printf("go to new tool\n");
		p2 = nextTool;
		p2.z = MOVE_HEIGHT;
		move(p1,p2,0,uart_port);
	}
	else
	{
		printf("go to new tool\n");
		p1 = nextTool;
		p1.z = MOVE_HEIGHT;
		move(p2,p1,0,uart_port);
	}
	//lower tool
	printf("lower\n");
	p1 = nextTool;
	p1.z = MOVE_HEIGHT;
	p2 = nextTool;
	move(p1,p2,0,uart_port);
	//turn on electromagnet
	printf("energize\n");
	move(p2,p2,1,uart_port);
	//raise tool
	printf("go up\n");
	move(p2,p1,1,uart_port);
	//go back to previous position
	printf("go back to x y\n");
	p2 = next;
	p2.z = MOVE_HEIGHT;
	move(p1,p2,1,uart_port);
	//lower
	p1 = next;
	move(p2,p1,1,uart_port);

	//after inital tool setup
	//go to prev.tool position and drop it
	//go to new tool position from old tool position and grab new tool
}

int numSteps(struct point prev, struct point next)
{
	float dist;
	float angle;

	dist = distance(prev,next);

	angle = fabsf(prev.theta - next.theta);

	if (dist <= 0 && angle <= 0)
	{
		return 1;
	}
	else if (dist*DIST_WEIGHT > angle*ANGLE_WEIGHT)
	{
		return (int) dist*DIST_WEIGHT;
	}
	else
	{
		return (int) angle*ANGLE_WEIGHT;
	}
}

struct packet calcStep(float x, float y, float z, float theta, char E)
{
	struct point draw;
	struct packet ret;
	draw.x = x;
	draw.y = y;
	draw.z = z;

	ret.S0 = dist2steps(distance(addP(draw,h0),post0)) - zero_step;
	ret.S1 = dist2steps(distance(addP(draw,h1),post1)) - zero_step;
	ret.S2 = dist2steps(distance(addP(draw,h2),post2)) - zero_step;
	ret.R = rot2steps(theta);
	ret.E = E;

	return ret;
}

int dist2steps(float dist)
{
	int steps;

	steps = dist/CIRC * 200;
	return steps;
}

int rot2steps(float rot)
{
	int steps;

	steps = (int)(rot * ANGLE_TO_STEPS);

	return steps;
}

float * interp(float one, float two, unsigned long num)
{
	float * pointsArr;
	float increment;
	int i;

	pointsArr = (float *)malloc(sizeof(float)*num);
	if (pointsArr == NULL)
	{
		printf("Couldn't allocate interp.\n");
		exit(0);
	}

	if (fabsf(two - one) < SMALL)
	{
		for (i = 0; i < num; i++)
		{
			pointsArr[i] = one;
		}

		return pointsArr;
	}

	pointsArr[num-1]=two;

	increment = (two-one)/num;
	pointsArr[0] = one+increment;
	for (i= 1;i<num-1;i++)
	{
		pointsArr[i] = pointsArr[i-1]+increment;
	}

	return pointsArr;
}

struct point addP(struct point one, struct point two)
{
	struct point ret;
	ret.x = one.x + two.x;
	ret.y = one.y + two.y;
	ret.z = one.z + two.z;
	return ret;
}

float distance(struct point one, struct point two)
{
	return (float)sqrt(pow(two.x-one.x,2) + pow(two.y-one.y,2) + pow(two.z-one.z,2));
}