#include "util.h"
#include "move.h"
#include "def.h"

int main(void)
{
	struct gLine line[2];
	FILE * fptr;
	unsigned long num = 0;
	float z[2];
	int readSuccess;
	int uart_port;

	wiringPiSetup();

	uart_port = serialOpen("/dev/ttyS0",9600);

	fptr = fopen("file.gcode","rb");

	line[0].moveType = MOVE;
	line[0].tool = NOTOOL;
	line[0].x = 0;
	line[0].y = 0;
	line[0].theta = 0;
	z[0] = 0;

	swapTool(NULL,line);

	initGlobal();

	// NOTE: num+1 is the current line
	while (!feof(fptr))
	{
		readSuccess = readLine(fptr,line + (num+1) % 2);
		if (readSuccess != 0)
		{
			printf("line didnt read correctly\n");
			exit(0);
		}

		// Check for tool swap
		if (line[num%2].tool != line[(num+1)%2].tool)
		{
			if (line[(num+1)%2].moveType != MOVE)
			{
				printf("Error in gcode file with move type.\n");
				exit(0);
			}
			swapTool(line+num%2,line+(num+1)%2);
		}

		if (line[(num+1)%2].moveType == DRAW)
		{
			z[(num+1)%2] = DRAW_HEIGHT;
		}
		else
		{
			z[(num+1)%2] = MOVE_HEIGHT;
		}

		move(line+num%2, line+(num+1)%2, z[num%2], z[(num+1)%2], 1, uart_port);

		num++;
	}

}
