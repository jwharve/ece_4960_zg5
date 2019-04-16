#include "util.h"
#include "move.h"
#include "def.h"
#include "string.h"
#include <signal.h>

int zero_step;
char state = RUN;


void handle_SIGINT (int unused)
{
	state = EXIT;
}


int main(int argc, char * argv[])
{
	signal(SIGINT, handle_SIGINT);

	struct gLine line[2];
	FILE * fptr;
	FILE * locF;
	unsigned long num = 0;
	int uart_port;
	char filename[FILE_NAME_LEN];
	filename[0] = 0;
	struct point p1, p2, p1u, p2u;

	char * locLine = NULL;
	size_t sizeLine;
	float prevZ;

	wiringPiSetup();
	initGlobal();

	if (argc == 1)
	{
		fptr = fopen("file.gcode","rb");
	}
	else
	{
		strcat(filename,"./gcode/");
		strcat(filename,argv[1]);
		fptr = fopen(filename,"rb");
	}
	if (fptr == NULL)
	{
		printf("Failed to open gcode file.\n");
		return 0;
	}
	uart_port = serialOpen("/dev/ttyS0",9600);

	locF = fopen("current.loc","r");

	getline(&locLine,&sizeLine,locF);

	// x y z theta tool
	sscanf(locLine,"%f %f %f %f %c",&(line[0].x),&(line[0].y),&prevZ,&(line[0].theta),&(line[0].tool));
	line[0].moveType = MOVE;
	fclose(locF);
	free(locLine);

	if (prevZ != DRAW_HEIGHT)
	{
		p1.x = line[0].x;
		p1.y = line[0].y;
		p1.z = prevZ;
		p1.theta = line[0].theta;
		p2 = p1;
		p2.z = DRAW_HEIGHT;
		move(p1,p2,1,uart_port);
	}



	// ZEROING PROCESS SHOULD END UP AT DRAW_HEIGHT

	// NOTE: num+1 is the next line
	while (1)
	{
		if(readLine(fptr,line + (num+1) % 2) != 0)
		{
			locF = fopen("current.loc","w");
			fprintf(locF,"%f %f %f %f %c",(line[num%2].x),(line[num%2].y),DRAW_HEIGHT,(line[(num%2)].theta),(line[(num%2)].tool));
			fclose(locF);
			printf("done\n");
			exit(0);
		}

		p1.x = (line+num%2)->x;
		p1.y = (line+num%2)->y;
		p1.z = DRAW_HEIGHT;
		p1.theta = (line+num%2)->theta;

		if (line[num%2].tool == FINE)
		{
			p1.z = p1.z-FINE_DROP;
		}

		p2.x = (line+(num+1)%2)->x;
		p2.y = (line+(num+1)%2)->y;
		p2.z = DRAW_HEIGHT;
		p2.theta = (line+(num+1)%2)->theta;

		// Check for tool swap
		if (line[num%2].tool != line[(num+1)%2].tool)
		{
			if (line[(num+1)%2].moveType != MOVE)
			{
				printf("Error in gcode file with move type.\n");
				exit(0);
			}

			swapTool(p1,p2,(line+num%2)->tool,(line+(num+1)%2)->tool,uart_port);
			num++;
			continue;
		}

		if (line[(num+1)%2].moveType == DRAW)
		{

			if (line[(num+1)%2].tool == FINE)
			{
				p2.z = p2.z - FINE_DROP;
			}
			// DRAW
			move(p1, p2, 1, uart_port);
		}
		else
		{
			// MOVE
			p1u = p1;
			p1u.z = MOVE_HEIGHT;
			p2u = p2;
			p2u.z = MOVE_HEIGHT;
			move(p1, p1u, 1, uart_port);
			move(p1u, p2u, 1, uart_port);
			move(p2u, p2, 1, uart_port);
		}

		num++;
	}
}
