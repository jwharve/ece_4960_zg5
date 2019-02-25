#include <wiringPi.h>
#include <wiringSerial.h>

int main(void)
{
	int uart_port;
	
	uart_port = serialOpen("/dev/ttyS0",9600);
	
	wiringPiSetup();
	
	serialPutchar(uart_port,'b');
	
	serialClose(uart_port);
}