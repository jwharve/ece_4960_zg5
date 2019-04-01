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

	wiringPiSetup();
	initGlobal();

	locF = fopen("current.loc","r");

	fscanf(locF,"%f %f %c",p1.x,p1.y,prevTool);
	p1.z = DRAW_HEIGHT;
	p1.theta = 0;

	uart_port = serialOpen("/dev/ttyS0",9600);

	p2.x = atof(argv[1]);
	p2.y = atof(argv[2]);
	p2.z = p1.z;
	p2.theta = atof(argv[3]);
	
	
	move(p1,p2,1,uart_port);
	
// 	struct gLine line[2];
// 	FILE * fptr;
// 	unsigned long num = 0;
// //	float z[2];
// 	int readSuccess;
// 	int uart_port;
// 	char filename[FILE_NAME_LEN];
// 	filename[0] = 0;


	fclose(locF);
	locF = fopen("current.loc","w");
	fprintf(locF,"%f %f %c\n", p2.x,p2.y,prevTool);
	fclose(locF);





}
