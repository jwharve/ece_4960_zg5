#include "util.h"
#include "move.h"
#include "def.h"
#include "string.h"

int zero_step;

int main(int argc, char * argv[])
{
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
	
	initGlobal();


	// ZEROING PROCESS SHOULD END UP AT DRAW_HEIGHT

	// NOTE: num+1 is the next line
	while (!feof(fptr))
	{
		if(readLine(fptr,line + (num+1) % 2) != 0)
		{
			locF = fopen("current.loc","w");
			fprintf(locF,"%f %f %f %f %c",(line[0].x),(line[0].y),DRAW_HEIGHT,(line[(num%2)].theta),(line[(num%2)].tool));
			fclose(locF);
			printf("done\n");
			exit(0);
		}
		
		p1.x = (line+num%2)->x;
		p1.y = (line+num%2)->y;
		p1.z = DRAW_HEIGHT;
		p1.theta = (line+num%2)->theta;
		
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
		}

		if (line[(num+1)%2].moveType == DRAW)
		{
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
