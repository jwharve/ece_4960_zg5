#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	

	size_t lineLen = 0;
	char *line = NULL;
	FILE *fp;
	int readSuccess = 0;
	int command,tool,xpos,ypos,ang;
	fp = fopen(argv[1], "r");

	if (fp == NULL) {

		printf("didnt open file correctly\n");

	}

	while ((readSuccess = getline(&line,&lineLen,fp)) != -1) {
	

		sscanf(line, "%d %d %d %d %d", &command, &tool, &xpos, &ypos, &ang);
		printf("command is: %d\n",command);
		printf("tool is: %d\n",tool);
		printf("xpos is: %d\n",xpos);
		printf("ypos is: %d\n",ypos);
		printf("ang is: %d\n",ang);
		printf("\n\n");
	}

	return 0;

}