#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <string>
#include <iostream>
using namespace std;
// void configure(int fd) {
//   struct  termios pts;
//   tcgetattr(fd, &pts);
//   cfsetospeed(&pts, 9600);   
//   cfsetispeed(&pts, 9600);   
//   tcsetattr(fd, TCSANOW, &pts);
// }
void configure(int);
void sendusb(char* port){
	char info[2] = {'a', 'b'}; 
	cout << "start send " << endl;
	int fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY);

	if (fd < 0) {
	perror("Could not open file");
	exit(1);
	}
	else {
	cout << "Successfully opened " << port << " for reading/writing" << endl;
	}

	configure(fd);
	write(fd, info, 2);
}