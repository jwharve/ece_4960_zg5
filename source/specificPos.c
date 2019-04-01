#include "util.h"
#include "move.h"
#include "def.h"
#include "string.h"

int zero_step;

int main(int argc, char * argv[])
{

	struct point p1, p2;
	struct packet zero = calcStep(0, 0, 0, 0, 0);
	FILE * locF;
	char prevTool;

	float nextX, nextY;

	char * locLine = NULL;
	size_t sizeLine;
	
	// Initialization
	wiringPiSetup();
	initGlobal();
	uart_port = serialOpen("/dev/ttyS0",9600);
	
	// Initalize P1
	locF = fopen("current.loc","r");
	getline(&locLine,&sizeLine,locF);
	sscanf(locLine,"%f %f %f %f %c",&(p1.x),&(p1.y),&(p1.z),&(p1.theta),&(p1.tool));
	
	fclose(locF);
	free(locLine);

	// Initialize P2
	p2.x = atof(argv[1]);
	p2.y = atof(argv[2]);
	p2.z = atof(argv[3])

	p2.theta = atof(argv[4]);
	emag = atof(argv[5])
	
	// Move
	move(p1,p2,emag,uart_port);

	// Write out position
	locF = fopen("current.loc","w");
	fprintf(locF,"%f %f %c\n", p2.x,p2.y,prevTool);
	fclose(locF);
}
