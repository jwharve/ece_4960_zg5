#include "util.h"
#include "move.h"
#include "def.h"
#include "string.h"

int zero_step;

int main(int argc, char * argv[])
{
	struct gLine line[2];
	FILE * fptr;
	unsigned long num = 0;
	float z[2];
	int readSuccess;
	int uart_port;
	char filename[FILE_NAME_LEN];
	filename[0] = 0;


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
	uart_port = serialOpen("/dev/ttyS0",9600);

	line[0].moveType = MOVE;
	line[0].tool = NOTOOL;
	line[0].x = 0;
	line[0].y = 0;
	line[0].theta = 0;
	z[0] = DRAW_HEIGHT;

	initGlobal();

//	printPacket(calcStep(0,0,0,0,0));

//	return 0;

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
			swapTool(line+num%2,line+(num+1)%2,uart_port,z[(num)%2]);
		}

		if (line[(num+1)%2].moveType == DRAW)
		{
			z[(num+1)%2] = DRAW_HEIGHT;
		}
		else
		{
			z[(num+1)%2] = MOVE_HEIGHT;
		}

		if (fabsf(z[0] - z[1]) < SMALL)
		{
//			printf("SAME HEIGHT\n");
			move(line+num%2, line+(num+1)%2, z[num%2], z[(num+1)%2], 1, uart_port);
		}
		else
		{
//			printf("DIFFERENT HEIGHT\n");
			if (z[(num+1)%2] == MOVE_HEIGHT)
			{
				move(line+num%2, line+num%2, z[num%2], z[(num+1)%2], 1, uart_port);
				move(line+num%2, line+(num+1)%2, z[(num+1)%2], z[(num+1)%2], 1, uart_port);
			}
			else
			{
				move(line+num%2, line+(num+1)%2, z[num%2], z[num%2], 1, uart_port);
				move(line+(num+1)%2, line+(num+1)%2, z[num%2], z[(num+1)%2], 1, uart_port);
			}
		}

		num++;
	}
}
