#include <wiringPi.h>
#include <wiringSerial.h>
#include <stdlib.h>
#include <stdio.h>


int main(int argc, char * argv[])
{
	int uart_port;
	int i;
	unsigned int num;
	char * rover;

	uart_port = serialOpen("/dev/ttyS0",9600);

	wiringPiSetup();

	num = (unsigned int)atoi(argv[1]);
	printf("pos = %d\n",num);
	rover = (char *)&num;

	for (i = 0; i < 4; i++)
	{
		serialPutchar(uart_port,rover[i]);
		printf("%x ",rover[i]);
	}
	printf("\n");

	serialClose(uart_port);
}
