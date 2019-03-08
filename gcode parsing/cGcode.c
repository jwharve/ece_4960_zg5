#include <stdio.h>
#include <stdlib.h>


	


int readLine(FILE *fptr, struct *gLine) 
{

	
	
	size_t lineLen = 0;
	char *line = NULL;
	int readSuccess = 0;

	readSuccess = getline(&line,&lineLen,fptr);

	if (readSuccess != -1) 
	{
		sscanf(line, "%c %c %d %d %d", gLine->moveType, gLine->tool, gLine->x, gLine->y, gLine->theta); 
	}
	else 
	{
		return -1;
	}


	return 0;

}