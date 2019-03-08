#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "util.h"
#include "move.h"

int main(void)
{
	struct gLine line[2];
	FILE * fptr;
	unsigned long num = 0;
	float z[2];
	int readSuccess;
	
	fptr = fopen("file.gcode","rb");
	
	readSuccess = readLine(fptr,line);
	if (readSuccess != 0) 
	{
		printf("first line didnt read correctly\n");
		exit(0);
	}

	swapTool(NULL,line);
	
	initGlobal();
	
	while (!feof(fptr))
	{
		readSuccess = readLine(fptr,line + num % 2);
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
		
		move(line+num%2, line+(num+1)%2, z[num%2], z[(num+1)%2], 1);
		
		num++;
	}

}