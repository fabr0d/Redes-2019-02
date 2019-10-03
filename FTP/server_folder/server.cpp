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
#include "sockets.hpp"
#include "protocol.hpp"//

using namespace std;

void ReadWriteMsn(int ConnectSFD, int SocketCFD, vector<string>& vcommand, vector<string>& vfile) {
  int n = 0;
  char buffer[4097];
  string str = "";
  string d_str = "";
  string e_str = "";
  do {
    bzero(buffer,4096);
    n = read(ConnectSFD,buffer,sizeof(buffer));
    buffer[n]='\0';
    str = buffer;
    d_str = parserDCommands(str,vcommand);
    cout<<"From Client: "<<d_str<<endl;
    if (vcommand[0] == "0") {
      n = write(SocketCFD,vcommand[1].c_str(),vcommand[1].size());
    }
    else if (vcommand[0] == "1") {
      e_str = parserEFiles(vcommand[1],vfile);
      n = write(SocketCFD,e_str.c_str(),e_str.size());
    }
    else if (vcommand[0] == "2") {
      bzero(buffer,4096);
      n = read(SocketCFD,buffer,sizeof(buffer));
      buffer[n]='\0';
      str = buffer;
      d_str = parserDFiles(str,vfile);
      cout<<"From Client: "<<d_str<<endl;
      n = write(SocketCFD,vcommand[1].c_str(),vcommand[1].size());
    }
    else if (vcommand[0] == "3") {
      n = write(SocketCFD,d_str.c_str(),d_str.size());
    }
  } while(d_str != "CLOSE");
}

int main() {
  // Data Begin
  vector<string> vcommand(2);
  vector<string> vfile(4);
  // Data End
  // Server Begin
  int ServerPORT = 0;
  cout << "Insert LocalPORT: ";
  cin >> ServerPORT;
  int SocketSFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  createServer(SocketSFD,ServerPORT);
  int ConnectSFD = accept(SocketSFD, NULL, NULL);
  // Server End
  
  // Recibir el Puerto del Cliente
  char buffer[10];
  int n = read(ConnectSFD,buffer,9);
  buffer[n]='\0';
  string str = buffer;
  cout << "ClientPort: " << str <<endl;
  
  // Client Begin
  //string ClientIP = "";
  string ClientIP = "127.0.0.1";
  int ClientPORT = stoi(str);
  int SocketCFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  createClient(SocketCFD,ClientIP,ClientPORT);
  // Client End
  
  ReadWriteMsn(ConnectSFD,SocketCFD,vcommand,vfile);
  
  shutdown(ConnectSFD, SHUT_RDWR);  
  close(ConnectSFD);
  close(SocketSFD);
  shutdown(SocketCFD, SHUT_RDWR);
  close(SocketCFD);
  
  return 0;
}

