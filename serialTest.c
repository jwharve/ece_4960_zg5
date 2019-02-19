



//serial communication UART test

//site used for reference --> https://www.cmrr.umn.edu/~strupp/serial.html#2_5_3

#include <stdio.h>  
#include <string.h> 
#include <unistd.h> 
#include <fcntl.h>   
#include <errno.h>   
#include <termios.h>


int main(void) {


	int fd; /* File descriptor for the port */
	int n;
	//O_RDWR read+write mode
	//O_NOCTTY flag means the program doesnt want to be controlling terminal for that port
	//	this means input will not affect your process
	//O_NDELAY program doesn't care what state the DCD signal line is in - whether the 
	//	other end of the port is up and running. If you do not specify this flag, your process 
	//	will be put to sleep until the DCD signal line is the space voltage.


	fd = open("/dev/ttyf1", O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd == -1) {
   /*
    * Could not open the port.
    */

    perror("open_port: Unable to open /dev/ttyf1 - ");
	} else {
		fcntl(fd, F_SETFL, 0);

	}

	n = write(fd, "testsig", 8);
	if (n < 0) {
  		fputs("write() of 8 bytes failed!\n", stderr);
	}

  	close(fd);
}