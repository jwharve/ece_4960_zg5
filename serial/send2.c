#include <wiringPi.h>
#include <wiringSerial.h>
#include <stdlib.h>
#include <stdio.h>

struct packet
{
	int S0;
	int S1;
	int S2;
	int R;
	char E;
} __attribute__((packed));

int main(int argc, char * argv[])
{
	int uart_port;
	int i;
	struct packet test;
	char * rover;

	uart_port = serialOpen("/dev/ttyS0",9600);

	wiringPiSetup();

	if (argc != 1)
	{
		test.S0 = (int)atoi(argv[1]);
		test.S1 = (int)atoi(argv[2]);
		test.S2 = (int)atoi(argv[3]);
		test.R = (int)atoi(argv[4]);
		if (argv[5][0] == '0')
		{
			test.E = 0;
		}
		else
		{
			test.E = 1;
		}
	}
	else
	{
		test.S0 = 0;
		test.S1 = 0;
		test.S2 = 0;
		test.R = 0;
		test.E = 0;
	}

	printf("S0 = %d\n",test.S0);
	printf("S1 = %d\n",test.S1);
	printf("S2 = %d\n",test.S2);
	printf("R = %d\n",test.R);
	printf("E = %d\n",test.E);

	rover = (char *)&test;

	for (i = 0; i < sizeof(struct packet); i++)
	{
		serialPutchar(uart_port,rover[i]);
		printf("%x ",rover[i]);
	}
	printf("\n");

	serialClose(uart_port);
}
