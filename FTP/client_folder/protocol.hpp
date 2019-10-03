#include <string>
#include <vector>
#include "functions.hpp"

using namespace std;

// Codifica Comandos
// command[0] = "1"
// command[1] = "file.txt"
string parserECommands(string input, vector<string> &vcommand) {
  // input = "GETfile.txt"
  string output = "";
  string command = input.substr(0,3);
  if (command == "GET") {
    output = "1" + input.substr(3);
    vcommand[0] = "1";
    vcommand[1] = input.substr(3);
  }
  else if (command == "PUT") {
    output = "2" + input.substr(3);
    vcommand[0] = "2";
    vcommand[1] = input.substr(3);
  }
  else {
    command = input.substr(0,5);
    if (command == "CLOSE") {
      output = "3";
      vcommand[0] = "3";
      vcommand[1] = "CLOSE";
    }
    else {
      output = input;
      vcommand[0] = "0";
      vcommand[1] = "PORT";
    }
  }
  return output;
}

// Decodifica Comandos
// command[0] = "1"
// command[1] = "file.txt"
string parserDCommands(string input, vector<string> &vcommand) {
  // input = "1file.txt"
  string output = "";
  string command = input.substr(0,1);
  if (command == "0") {   // No sera necesario
    output = input.substr(1);
    vcommand[0] = "0";
    vcommand[1] = "PORT";
  }
  else if (command == "1") {
    output = "GET" + input.substr(1);
    vcommand[0] = "1";
    vcommand[1] = input.substr(1);
  }
  else if (command == "2") {
    output = "PUT" + input.substr(1);
    vcommand[0] = "2";
    vcommand[1] = input.substr(1);
  }
  else if (command == "3") {
    output = "CLOSE";
    vcommand[0] = "3";
    vcommand[1] = "CLOSE";
  }
  return output;
}
 
// Codifica Archivos
// vfile[0] = "08"
// vfile[1] = "file.txt"
// vfile[2] = "004"
// vfile[3] = "hola"
string parserEFiles(string input, vector<string> &vfile) {
  // input = "file.txt"
  string output = "";
  string fname_size = zeroPadding(to_string(input.size()),2);
  string file_name = input;
  string file_data = FileToString(input);
  string fdata_size = zeroPadding(to_string(file_data.size()),3);
  output = fname_size + file_name + fdata_size + file_data;
  vfile[0] = fname_size;
  vfile[1] = file_name;
  vfile[2] = fdata_size;
  vfile[3] = file_data;
  return output;
}
 
// Decodifica Archivos
// vfile[0] = "08"
// vfile[1] = "file.txt"
// vfile[2] = "004"
// vfile[3] = "hola"
string parserDFiles(string input, vector<string> &vfile) {
  // input = "08file.txt004hola"
  string output = "";
  string fname_size = input.substr(0,2);
  int s0 = stoi(fname_size);
  string file_name = input.substr(2,s0);
  int a0 = 2 + s0;
  string fdata_size = input.substr(a0,3);
  int a1 = 3 + a0;
  int s2 = stoi(fdata_size);
  string file_data = input.substr(a1,s2);
  StringToFile(file_name,file_data);
  output = file_name;
  vfile[0] = fname_size;
  vfile[1] = file_name;
  vfile[2] = fdata_size;
  vfile[3] = file_data;
  return output;
}

