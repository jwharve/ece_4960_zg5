#include "util.h"

void initGlobal(void)
{
	h0.x = 0;
	h0.y = -MOUNT_RADIUS;
	h0.z = MOUNT_HEIGHT;

	h1.x = -MOUNT_RADIUS*sin(60*DTR);
	h1.y = MOUNT_RADIUS*cos(60*DTR);
	h1.z = MOUNT_HEIGHT;

	h2.x = MOUNT_RADIUS*sin(60*DTR);
	h2.y = MOUNT_RADIUS*cos(60*DTR);
	h2.z = MOUNT_HEIGHT;

	post0.x = 0;
	post0.y = -POST_RADIUS;
	post0.z = POST_HEIGHT;

	post1.x = -POST_RADIUS*sin(60*DTR);
	post1.y = POST_RADIUS*cos(60*DTR);
	post1.z = POST_HEIGHT;

	post2.x = POST_RADIUS*sin(60*DTR);
	post2.y = POST_RADIUS*cos(60*DTR);
	post2.z = POST_HEIGHT;

	struct point zero;
	zero.x = 0;
	zero.y = 0;
	zero.z = DRAW_HEIGHT;

	zero_step = dist2steps(distance(addP(zero,h0),post0));
}

int readLine(FILE * fptr, struct gLine * next)
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
		sscanf(line, "%c %c %s %s %f", &(next->moveType), &(next->tool), xString, yString, &(next->theta));
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

		next->x = x;
		next->y = y;
	}
	else
	{
		return -1;
	}

	printf("READ - %c %c %f %f %f\n",next->moveType,next->tool,x,y,next->theta);
	return 0;

}

void sendPacket(struct packet p, int uart_port)
{
	unsigned char * rover;
	int i;

	rover = (unsigned char *)&p;

	for (i = 0; i < sizeof(struct packet); i++)
	{
		serialPutchar(uart_port, rover[i]);
	}
}

void printPacket(struct packet p)
{
	printf("S0 = %5d\tS1 = %5d\tS2 = %5d\tR = %5d\tE  =     %d\n", p.S0, p.S1, p.S2, p.R, p.E);
}
