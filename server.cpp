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
    TCP tcp;
    tcp.init(5555);
    cout << "Waiting for host...\n";
    int host = tcp.connect();
    cout << "Host connected.\n";
    tcp.output(host, "Hey! You're the host.");
    Module * lobby = new Lobby(& tcp);
    string command;
    while (command != "#"){
        command = tcp.input(host);
        tcp.output(host, lobby->parse(command));
    }
    
    return 0;
}