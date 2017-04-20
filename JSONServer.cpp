/* 
This code primarily comes from 
http://www.prasannatech.net/2008/07/socket-programming-tutorial.html
and
http://www.binarii.com/files/papers/c_sockets.txt
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string>
#include <iostream>
#include <strings.h>
#include <vector>
#include <pthread.h>
#include <queue>
#include <deque>
using namespace std;
struct usbFuncInput{
  deque<double>* vec;
  char* arduinoPort;
};
struct serverFuncInput{
  deque<double>* vec;
  int PORT_NUMBER;
  char* arduinoPort;
} ;
void readusb(deque<double>*, char*);
void* usbFunc(void* p){
  usbFuncInput input = *(usbFuncInput*)p;
  readusb(input.vec, input.arduinoPort);
}
void sendusb(char*, string);
bool iscelsius;//true for celsius false for fah;
bool isAlertMod = false;
double convertTemp(double input){
  double fah = input * 1.8 + 32;
  return fah;
}
double alertTemp = 0;
int start_server(int PORT_NUMBER, deque<double>* input, char* arduinoPort)
{

      // structs to represent the server and client
      struct sockaddr_in server_addr,client_addr;    
      
      int sock; // socket descriptor

      // 1. socket: creates a socket descriptor that you later use to make other system calls
      if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket");
        exit(1);
      }
      int temp;
      if (setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&temp,sizeof(int)) == -1) {
        perror("Setsockopt");
        exit(1);
      }

      // configure the server
      server_addr.sin_port = htons(PORT_NUMBER); // specify port number
      server_addr.sin_family = AF_INET;         
      server_addr.sin_addr.s_addr = INADDR_ANY; 
      bzero(&(server_addr.sin_zero),8); 
      
      // 2. bind: use the socket and associate it with the port number
      if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
        perror("Unable to bind");
        exit(1);
      }

      // 3. listen: indicates that we want to listn to the port to which we bound; second arg is number of allowed connections
      if (listen(sock, 5) == -1) {
        perror("Listen");
        exit(1);
      }
          
      // once you get here, the server is set up and about to start listening
      cout << endl << "Server configured to listen on port " << PORT_NUMBER << endl;
      fflush(stdout);
     
      while(1){
        // 4. accept: wait here until we get a connection on that port
        isAlertMod = false;
        int sin_size = sizeof(struct sockaddr_in);
        int fd = accept(sock, (struct sockaddr *)&client_addr,(socklen_t *)&sin_size);
        cout << "Server got a connection from " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << endl;
        
        // while(1){
          // buffer to read data into
          char request[1024];
          
          // 5. recv: read incoming message into buffer
          int bytes_received = recv(fd,request,1024,0);
          // null-terminate the string
          request[bytes_received] = '\0';
          cout << "Here comes the message:" << endl;
          cout << request << endl;
          if(request[5] == 'a'){
            cout << "blink" << endl;
            // sendusb(arduinoPort);
          }
          if(request[5] == 'c'){
            iscelsius = true;
            string inputMessage = "a";
            sendusb(arduinoPort, inputMessage);
          }
          if(request[5] == 'f'){
            iscelsius = false;
            string inputMessage = "b";//b for fran
            sendusb(arduinoPort, inputMessage);
          }
          if(request[5] == 's'){
            string inputMessage = "s";//stand by start request
            sendusb(arduinoPort, inputMessage);
            // isAlertMod = false;
          }
          if(request[5] == 'e'){
            string inputMessage = "e";//stand by end request
            sendusb(arduinoPort, inputMessage);
          }
          if(request[5] == 'd'){
            string inputMessage = "c";
            sendusb(arduinoPort, inputMessage);
          }
          if(request[6] == '8'){
            alertTemp = 20;
            isAlertMod = true;
          }
          if(request[6] == '9'){
            alertTemp = 22;
            isAlertMod = true;
          }
          if(request[6] == '0'){
            alertTemp = 24;
            isAlertMod = true;  
          }
          if(request[6] == '1'){
            alertTemp = 26;
            isAlertMod = true;
          }
          if(request[6] == '2'){
            alertTemp = 28;
            isAlertMod = true;
            cout << "awesdf" << endl;
          }
          if(request[6] == '3'){
            alertTemp = 30;
            isAlertMod = true;
          }
          // this is the message that we'll send back
          /* it actually looks like this:
            {
               "name": "cit595"
            }
          */

          if(isAlertMod){
            while(true){
              double curTemp = (*input)[input->size() - 1];
              if(curTemp > alertTemp){
                string alertreply = "{\n\"alert\": \"true\"\n}";
                send(fd, alertreply.c_str(), alertreply.length(), 0);
                cout << "Server sent message: " << alertreply << endl;
                close(fd);
                break;
              }
            }
            continue;
          }
          double curTemp = (*input)[input->size() - 1];
          deque<double>::iterator it;
          double minTemp = curTemp;
          double maxTemp = curTemp;
          double sumTemp = 0;
          string alertreply;
          if(isAlertMod){
            cout << "alert start" << endl;
            cout << "cur temp is : " << curTemp << endl;
            if(curTemp > alertTemp){
              alertreply = "{\n\"alert\": \"true\"\n}";
            }
          }
          for(it = (*input).begin(); it != (*input).end(); it++){
            sumTemp += (*it);
            minTemp = min(minTemp, (*it));
            maxTemp = max(maxTemp, (*it));
          }
          double aveTemp = sumTemp / input->size();
          if(!iscelsius){
            curTemp = convertTemp(curTemp);
            minTemp = convertTemp(minTemp);
            maxTemp = convertTemp(maxTemp);
            aveTemp = convertTemp(aveTemp); 
          }
          string temper = to_string(curTemp);
          string minTempStr = to_string(minTemp);
          string averageTemp = to_string(aveTemp);
          string maxTempStr = to_string(maxTemp);
          string reply = "{\n\"curTemp\": \"curTemp: " + temper + "\",\n" + "\"min\": \"minTemp: " + minTempStr + "\",\n" + "\"max\": \"maxTemp: " + maxTempStr + "\",\n" + "\"average\": \"averageTemp: " + averageTemp + "\"\n}\n";
          if(isAlertMod){
            if(curTemp > alertTemp){
              reply = alertreply;
              cout << reply << endl;
            }
          }          
          // 6. send: send the message over the socket
          // note that the second argument is a char*, and the third is the number of chars

          send(fd, reply.c_str(), reply.length(), 0);
          cout << "Server sent message: " << reply << endl;
        // }
        close(fd);
      }

      // 7. close: close the socket connection
      close(sock);
      cout << "Server closed connection" << endl;
  
      return 0;
} 
void* serverFunc(void* p){
  serverFuncInput input = *(serverFuncInput*)p;
  start_server(input.PORT_NUMBER, input.vec, input.arduinoPort);
}

int main(int argc, char *argv[])
{
  int r = 0;
  pthread_t readUSBThread, serverThread;
  deque<double>* temp = new deque<double>();
  usbFuncInput usbInput;
  usbInput.vec = temp;
  usbInput.arduinoPort = argv[1];
  serverFuncInput serverInput;
  serverInput.vec = temp;
  serverInput.PORT_NUMBER = atoi(argv[2]);
  serverInput.arduinoPort = argv[1];
  r = pthread_create(&readUSBThread, NULL, &usbFunc, &usbInput);
  if(r != 0){
    cout << "readUSBThread ERROR!" << endl;
  }
  r = pthread_create(&serverThread, NULL, &serverFunc, &serverInput);
  if(r != 0){
    cout << "serverThread ERROR!" << endl;
  }
  void* r1;
  void* r2;
  r = pthread_join(readUSBThread, &r1);
  if(r != 0){
    cout << "readUSBThread join ERROR!" << endl;
  }
  r = pthread_join(serverThread, &r2);
  if(r != 0){
    cout << "serverThread join ERROR!" << endl;
  }
  
  // string inputMessage = "b";
  // sendusb(argv[1], inputMessage);
  // cout << inputMessage << endl;
  
}

