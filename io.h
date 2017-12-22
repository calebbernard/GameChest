#pragma once

#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iomanip>
#include <string>

#include "stringUtil.h"
#include "dataStructures.h"
using namespace std;

class User;

class TCP{
private:
    int sockfd;
    socklen_t clilen;
    char buffer[100000];
    struct sockaddr_in serv_addr, cli_addr;
public:
    void init(int port);
    void output (User * u, string msg);
    string input(User * u);
    int connect();
    string getInput(User * u);
};
