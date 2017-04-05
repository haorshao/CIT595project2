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
#include <vector>
#include <deque>
#include <boost/algorithm/string.hpp>
using namespace std;
using namespace boost;

char info[2017];
/*
This code configures the file descriptor for use as a serial port.
*/
void configure(int fd) {
  struct  termios pts;
  tcgetattr(fd, &pts);
  cfsetospeed(&pts, 9600);   
  cfsetispeed(&pts, 9600);   
  tcsetattr(fd, TCSANOW, &pts);
}


void readusb(deque<double>* temp, char* port) {

  // if (argc < 2) {
  //   cout << "Please specify the name of the serial port (USB) device file!" << endl;
  //   exit(0);
  // }

  // get the name from the command line
  // char* file_name = argv[1];
  
  // try to open the file for reading and writing
  // char* portChar = 
  cout << "start read " << endl;
  int fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
  
  if (fd < 0) {
    perror("Could not open file");
    exit(1);
  }
  else {
    cout << "Successfully opened " << port << " for reading/writing" << endl;
  }

  configure(fd);

  /*
    Write the rest of the program below, using the read and write system calls.
  */
  int bytes_read = 0;
  int count = 0;
  string output;
  while(1){
    bytes_read = 0;
    while(bytes_read <= 0){
      bytes_read = read(fd, info, 2017);
      continue;
    }
    info[bytes_read] = '\0';
    string cur(info);
    if (cur[cur.length()-1] != '\n'){
      output += cur;
    }else if (cur.length() != 1){
      output += cur;
      // cout << output;
      // cout << "count: " << count << endl;
      count++;
      if(count >= 10){
        vector<string>* line = new vector<string>();
        split(*line, output, is_any_of(" "));
        (*temp).push_back(stod((*line)[3]));
        cout << (*line)[3] << endl;
        delete line;
      }
      output = "";
      while(temp->size() > 3600){
        temp->pop_front();
      }
      // if(count >= 15){
      //   cout << "read end" << endl;
      //   return;
      // }
    }
  }

}
