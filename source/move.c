#include "move.h"
#include "util.h"

void move(struct gLine * prev, struct gLine * curr, float prev_z, float curr_z, unsigned char E, int uart_port)
{
	struct packet current;
	int numPoints;
	float * x;
	float * y;
	float * z;
	float * theta;
	int i;

	printf("PREV\n");
	printf("x = %f, y = %f, z = %f, theta = %f\n", prev->x, prev->y, prev_z, prev->theta);
	printf("CURRENT\n");
	printf("x = %f, y = %f, z = %f, theta = %f\n", curr->x, curr->y, curr_z, curr->theta);


	numPoints = numSteps(*prev, *curr, prev_z, curr_z);

	x = (float * )malloc(numPoints*sizeof(float));
	y = (float * )malloc(numPoints*sizeof(float));
	z = (float * )malloc(numPoints*sizeof(float));
	theta = (float * )malloc(numPoints*sizeof(float));

	x = interp(prev->x,curr->x,numPoints);
	y = interp(prev->y,curr->y,numPoints);
	z = interp(prev_z,curr_z,numPoints);
	theta = interp(prev->theta,curr->theta,numPoints);

	for (i = 0; i < numPoints; i++)
	{
//		printf("x - %f\ty - %f\tz - %f\n",x[i],y[i],z[i]);
		current = calcStep(x[i], y[i], z[i], theta[i], 1);
		printPacket(current);
		sendPacket(current, uart_port);
		delay(SEND_DELAY);
	}

//	delay(2000);

	free(x); free(y); free(z); free(theta);
}

int numSteps(struct gLine prev, struct gLine curr, int prev_z, int curr_z)
{
	struct point one, two;
	float dist;
	float angle;

	one.x = prev.x;
	one.y = prev.y;
	one.z = prev_z;

	two.x = curr.x;
	two.y = curr.y;
	two.z = curr_z;

	dist = distance(one,two);

	angle = fabsf(curr.theta - prev.theta);

	if (dist*DIST_WEIGHT > angle*ANGLE_WEIGHT)
	{

		if ((int) dist*DIST_WEIGHT > 0) {
			return (int) dist*DIST_WEIGHT;
		}else {
			return 1;
		}
	}
	else
	{

		if ((int) angle*ANGLE_WEIGHT > 0) {
			return (int) angle*ANGLE_WEIGHT;
		}else {
			return 1;
		}
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

void swapTool(struct gLine * prev, struct gLine * curr, int uart_port, float z_prev)
{

	return;

	//grab tool initially
	//go to prev tool position and release tool
	//go to new tool position from old tool position and grab new tool

	struct gLine prevToolLine,newToolLine;

	if (curr->tool==CLEAR)
	{
		newToolLine.x = TOOLCLEARX;
		newToolLine.y = TOOLCLEARY;
	}
	else if (curr->tool == BLUNT)
	{
		newToolLine.x = TOOLBLUNTX;
		newToolLine.y = TOOLBLUNTY;
	}
	else if (curr->tool == FINE)
	{
		newToolLine.x = TOOLFINEX;
		newToolLine.y = TOOLFINEY;
	}
	else if (curr->tool == RAKE)
	{
		newToolLine.x = TOOLRAKEX;
		newToolLine.y = TOOLRAKEY;
	}

	if (prev->tool != NOTOOL)
	{
		if (prev->tool==CLEAR)
		{
			prevToolLine.x = TOOLCLEARX;
			prevToolLine.y = TOOLCLEARY;

		}
		else if (prev->tool == BLUNT)
		{
			prevToolLine.x = TOOLBLUNTX;
			prevToolLine.y = TOOLBLUNTY;

		}
		else if (prev->tool == FINE)
		{
			prevToolLine.x = TOOLFINEX;
			prevToolLine.y = TOOLFINEY;
		}
		else if (prev->tool == RAKE)
		{
			prevToolLine.x = TOOLRAKEX;
			prevToolLine.y = TOOLRAKEY;
		}
	}

	//go up--pass in prev for previous and current so x and y dont change
	//pass in draw height and move height to raise it
	printf("starting tool change\n");
	move(prev,prev,z_prev,MOVE_HEIGHT,1,uart_port);
	if (prev->tool != NOTOOL)
	{
		//go to previous tool position in toolbed, pass in prev for previous position
		//and prevToolLine for new x and y, keep heights the same
		printf("move to prev tool x y\n");
		move(prev,&prevToolLine,MOVE_HEIGHT,MOVE_HEIGHT,1,uart_port);
		//lower height to prepare for detachment
		printf("go down\n");
		move(&prevToolLine,&prevToolLine,MOVE_HEIGHT,DRAW_HEIGHT,1,uart_port);
		//release electromagnet
		printf("release\n");
		move(&prevToolLine,&prevToolLine,DRAW_HEIGHT,DRAW_HEIGHT,0,uart_port);
		//raise up without tool
		printf("go up\n");
		move(&prevToolLine,&prevToolLine,DRAW_HEIGHT,MOVE_HEIGHT,0,uart_port);
		//move to new tool
		printf("go to new tool\n");
		move(&prevToolLine,&newToolLine,MOVE_HEIGHT,MOVE_HEIGHT,0,uart_port);
	}
	else
	{
		printf("go to new tool\n");
		move(prev,&newToolLine,MOVE_HEIGHT,MOVE_HEIGHT,0,uart_port);
	}
	//lower tool
	printf("lower\n");
	move(&newToolLine,&newToolLine,MOVE_HEIGHT,DRAW_HEIGHT,0,uart_port);
	//turn on electromagnet
	printf("energize\n");
	move(&newToolLine,&newToolLine,DRAW_HEIGHT,DRAW_HEIGHT,1,uart_port);
	//raise tool
	printf("go up\n");
	move(&newToolLine,&newToolLine,DRAW_HEIGHT,MOVE_HEIGHT,1,uart_port);
	//go back to previous position
	printf("go back to x y\n");
	move(&newToolLine,prev,MOVE_HEIGHT,MOVE_HEIGHT,1,uart_port);
	//lower
	printf("go back to z\n");
	move(prev,prev,MOVE_HEIGHT,z_prev,1,uart_port);

	//after inital tool setup
	//go to prev.tool position and drop it
	//go to new tool position from old tool position and grab new tool
}
