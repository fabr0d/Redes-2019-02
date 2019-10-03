// Cuba Lajo Rubén Adrián
// Flores Pari Fabrizio Rodrigo

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <thread>
#include "sockets.hpp"
#include "protocol.hpp"

using namespace std;

void writeMsn(int SocketCFD, int ConnectSFD, vector<string>& vcommand, vector<string>& vfile) {
  int n = 0;
  char buffer[4097];
  string str = "";
  string e_str = "";
  do {
    bzero(buffer,4096);
    cin.getline(buffer,4096);
    str = buffer;
    e_str = parserECommands(str,vcommand);
    if (vcommand[0] == "0") {
      n = write(SocketCFD,e_str.c_str(),e_str.size());
    }
    else if (vcommand[0] == "1") {
      n = write(SocketCFD,e_str.c_str(),e_str.size());
    }
    else if (vcommand[0] == "2") {
      n = write(SocketCFD,e_str.c_str(),e_str.size());
      e_str = parserEFiles(vcommand[1],vfile);
      n = write(ConnectSFD,e_str.c_str(),e_str.size());
    }
    else if (vcommand[0] == "3") {
      n = write(SocketCFD,e_str.c_str(),e_str.size());
    }
  } while(str != "CLOSE");
}

void readMsn(int ConnectSFD, int SocketCFD, vector<string>& vcommand, vector<string>& vfile) {
  int n = 0;
  char buffer[4097];
  string str = "";
  string d_str = "";
  do {
    bzero(buffer,4096);
    n = read(ConnectSFD,buffer,sizeof(buffer));
    buffer[n]='\0';
    str = buffer;
    if (vcommand[0] == "0") {
      d_str = str;
    }
    else if (vcommand[0] == "1") {
      d_str = parserDFiles(str,vfile);
    }
    else if (vcommand[0] == "2") {
      d_str = str;
    }
    else if (vcommand[0] == "3") {
      d_str = str;
    }
    cout<<"From Server: "<<d_str<<endl;
  } while(str != "CLOSE");
}

int main() {
  // Data Begin
  vector<string> vcommand(2);
  vector<string> vfile(4);
  // Data End
  // Client Begin
  //string ServerIP = "";
  //cout << "Insert IP: ";
  //cin >> ServerIP;
  string ServerIP = "127.0.0.1";
  int ServerPORT = 0;
  cout << "Insert ServerPORT: ";
  cin >> ServerPORT;
  int SocketCFD = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
  createClient(SocketCFD,ServerIP,ServerPORT);
  // Client End
  
  // Enviar el Puerto al Server
  int ClientPORT = 0;
  cout << "Insert LocalPORT: ";
  cin >> ClientPORT;
  string str = to_string(ClientPORT);
  int n = write(SocketCFD,str.c_str(),str.size());	
  
  // Server Begin
  int SocketSFD = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
  createServer(SocketSFD,ClientPORT);
  int ConnectSFD = accept(SocketSFD, NULL, NULL);
  //Server End
  
  thread t1(writeMsn,SocketCFD,ConnectSFD,ref(vcommand),ref(vfile));
  thread t2(readMsn,ConnectSFD,SocketCFD,ref(vcommand),ref(vfile));
  t1.join();
  t2.join();
  
  shutdown(SocketCFD, SHUT_RDWR);
  close(SocketCFD);
  shutdown(ConnectSFD, SHUT_RDWR);
  close(ConnectSFD);
  close(SocketSFD);
  
  return 0;
}

