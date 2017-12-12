#include <string>
#include <iostream>
#include <unistd.h>
#include <sstream>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <cstdlib>
#include <cstring>
#include <stdio.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include "io.h"
#include "stringUtil.h"
#include "dataStructures.h"
#include "lobby.h"
using namespace std;

void l(){
    cout << "Server started. Connect on port 5555.\n";
    TCP tcp;
    tcp.init(5555);
    cout << "Waiting for 2 connections...\n";
    int conn1 = tcp.connect();
    cout << "Connected 1/2\n";
    tcp.output(conn1, "Connected 1/2");
    int conn2 = tcp.connect();
    tcp.output(conn2, "Connected 2/2");
    cout << "Connected 2/2\n";
    string t1 = tcp.input(conn1);
    string t2 = tcp.input(conn2);
    tcp.output(conn2, t1);
    tcp.output(conn1, t2);
}

int main(){
    Module * lobby = new Lobby();
    string command;
    while (true){
        cout << "> ";
        cin >> command;
        if (command == "#"){
            return 0;
        } else {
            cout << lobby->parse(command) << '\n';
        }
    }
    
    return 0;
}