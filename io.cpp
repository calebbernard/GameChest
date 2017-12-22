#include "io.h"
using namespace std;

void TCP::init(int port){
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);
    bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
}
void TCP::output (User * u, string msg){
    string length = itos(msg.length());
    while (length.length() < 5){
        length = "0" + length;
    }
    string finalMsg = length + msg;
    //if (u->outputLast == true){
      //cout << "Error: writing output to this user twice before reading.\n";
      //cout << "Attempted output: " << msg << "\n";
      //return;
    //}
    write(u->conn, finalMsg.c_str(), finalMsg.length());
    u->outputLast = true;
}
string TCP::input(User * u){
    bzero(buffer, 99999);
    //if (u->outputLast == false){
      //cout << "Error: reading input from this user twice before responding.\n";
      //output(u, "#");
      //return "";
    //}
    output(u, "#Ready for input.");
    read(u->conn, buffer, 5);
    for (int x = 0; x < 5; x++){
        if (buffer[x] == '\0'){
            throw(0);
        }
        if (buffer[x] < '0' || buffer[x] > '9'){
            cout << "Error: Bad input format.\nPlease preface each ASCII message with a 5-character string specifying the length in bytes of the remaining message.\n";
            cout << "(Character " << buffer[x] << " found at position " << x << ", which is reserved for length marker)\n";
            throw(0);
        }
    }
    int msgLength = 0;
    for (int x = 0; x < 5; x++){
        msgLength *= 10;
        msgLength += buffer[x] - '0';
    }
    bzero(buffer, 5);
    if (msgLength > 0){
        read (u->conn, buffer, msgLength);
    }
    u->outputLast = false;
    return buffer;
}
int TCP::connect(){
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    int socket = accept(sockfd, (struct sockaddr*) &cli_addr, &clilen);
    bzero(buffer, 99999);
    return socket;
}
