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

int main(){
    TCP tcp;
    tcp.init(5555);
    srand(time(0));
    cout << "Waiting for host...\n";
    int conn = tcp.connect();
    cout << "Host connected.\n";
    User * host = new User();
    host->conn = conn;
    tcp.output(host, "Hey! You're the host. What's your name?");
    host->name = tcp.input(host);
    Module * lobby = new Lobby(&tcp, host);
    try{
        lobby->turnManager();
    } catch(int e){
        cout << "Error encountered. Server closing.\n";
    }
    
    return 0;
}
