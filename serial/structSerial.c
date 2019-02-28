#include <wiringPi.h>
#include <wiringSerial.h>
#include <stdlib.h>
#include <stdio.h>

struct __attribute__((__packed__)) {


	int string0;
	int string1;
	int string2;

	int rotation;
	unsigned char mag;
} packet;


int main(int argc, char * argv[])
{
	int uart_port;
	int i;
	unsigned int num;
	unsigned char * rover;

	uart_port = serialOpen("/dev/ttyS0",9600);

	wiringPiSetup();


	rover = (unsigned char *)&packet;

	for (i = 0; i < sizeof(packet); i++)
	{
		serialPutchar(uart_port,rover[i]);
		printf("%x ",rover[i]);
	}
	printf("\n");

	serialClose(uart_port);
}
