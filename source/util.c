#include "util.h"

void printPacket(struct packet p)
{
	printf("S0 = %5d\tS1 = %5d\tS2 = %5d\tR = %5d\tE  =     %d\n", p.S0, p.S1, p.S2, p.R, p.E);
}

void initGlobal(void)
{
	h0.x = 0;
	h0.y = -MOUNT_RADIUS;
	h0.z = MOUNT_HEIGHT;

	h1.x = MOUNT_RADIUS*sin(60*DTR);
	h1.y = MOUNT_RADIUS*cos(60*DTR);
	h1.z = MOUNT_HEIGHT;

	h2.x = -MOUNT_RADIUS*sin(60*DTR);
	h2.y = MOUNT_RADIUS*cos(60*DTR);
	h2.z = MOUNT_HEIGHT;

	post0.x = 0;
	post0.y = -POST_RADIUS;
	post0.z = POST_HEIGHT;

	post1.x = POST_RADIUS*sin(60*DTR);
	post1.y = POST_RADIUS*cos(60*DTR);
	post1.z = POST_HEIGHT;

	post2.x = -POST_RADIUS*sin(60*DTR);
	post2.y = POST_RADIUS*cos(60*DTR);
	post2.z = POST_HEIGHT;
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
	float ret;
	ret = sqrt(pow(two.x-one.x,2) + pow(two.y-one.y,2) + pow(two.z-one.z,2));
	return ret;
}

int readLine(FILE * fptr, struct gLine * curr)
{
	size_t lineLen = 0;
	char * line = NULL;
	int readSuccess = 0;
	char xString[5];
	char yString[5];
	float x = 0;
	float y = 0;

	readSuccess = getline(&line,&lineLen,fptr);

	if (readSuccess != -1)
	{
		sscanf(line, "%c %c %s %s %f", &(curr->moveType), &(curr->tool), xString, yString, &(curr->theta));
		//printf("xString - %s ... yString %s\n",xString,yString);
		x += (float)(xString[1]-'0')*1;
		x += (float)(xString[2]-'0')/10;
		x += (float)(xString[3]-'0')/100;
		if (xString[0] == '-')
		{
			x = -x;
		}
		else
		{
			x += (float)(xString[0]-'0')*10;
		}

		y += (float)(yString[1]-'0')*1;
		y += (float)(yString[2]-'0')/10;
		y += (float)(yString[3]-'0')/100;
		if (yString[0] == '-')
		{
			y = -y;
		}
		else
		{
			y += (float)(yString[0]-'0')*10;
		}

		curr->x = x;
		curr->y = y;
	}
	else
	{
		return -1;
	}

	printf("READ - %c %c %f %f %f\n",curr->moveType,curr->tool,x,y,curr->theta);
	return 0;

}

float * interp(float one, float two, unsigned long num)
{

	float * pointsArr;

	pointsArr = (float *)malloc(sizeof(float)*num);
	if (pointsArr == NULL)
	{
		printf("Couldn't allocate interp.\n");
		exit(0);
	}

	float increment;
	int i;


	if (fabsf(two - one) < SMALL)
	{
		for (i = 0; i < num; i++)
		{
			pointsArr[i] = one;
		}

		return pointsArr;
	}

	pointsArr[num]=two;

	increment = (two-one)/num;
	pointsArr[0] = one+increment;
	//printf("FIRST - %f\n",pointsArr[0]);
	for (i= 1;i<num-1;i++)
	{
		pointsArr[i] = pointsArr[i-1]+increment;
		//printf("%f\n",pointsArr[i]);
	}

	return pointsArr;
}

int dist2steps(float dist)
{

	//circum needs to be measures
	int steps;

	steps = dist/CIRC * 200;
	return steps;
}

int rot2steps(float rot)
{


	int steps;

	steps = rot * (200/360);

	return steps;
}

void sendPacket(struct packet p, int uart_port)
{
	unsigned char * rover;
	int i;

	rover = (unsigned char *)&p;

	for (i = 0; i < sizeof(struct packet); i++)
	{
		serialPutchar(uart_port, rover[i]);
		delay(3);
	}
}
