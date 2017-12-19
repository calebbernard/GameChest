#pragma once

#include <string>
#include <vector>
#include "stringUtil.h"
#include "io.h"
using namespace std;

class TCP;

class Argument{
public:
    string type; // this should become an enum
    string description;
};

class Command{
public:
    string keyword;
    string description;
    vector<Argument> argument;
    void set(string keyword_, string description_, string argTypes_, string argDescriptions_);
    string describe();
};

class Option{
    public:
    string name;
    string value;
};

class metaData{
    public:
    string name;
    int maxPlayers;
    vector<Option> options;
    //vector<string> roles;
};

class User{
    public:
    User();
    string name;
    int conn;
    bool outputLast;
};

enum ActionType {};

class Module{
public:
    TCP * tcp;
    vector<User*> users;
    vector<Command> availableCommands;
    virtual string instructions()=0;
    virtual string runCommand(vector<string> words, int arity)=0;
    virtual string invalidInput(bool blankString);
    virtual string parse(string request);
    virtual void addCommand(string keyword, string description, string argTypes, string argDescriptions, );
    virtual void removeCommand(string keyword_, int arity_);
    virtual void turnManager();
    virtual User * next(User * current);
    virtual void broadcast(string msg);
};

enum ResultHeader {Success, Failure, Tie, Error, Other};

class Result{
    public:
    ResultHeader header;
    string message;
    Result(ResultHeader r, string msg);
    string get();
};

class Game : public Module{
public:
    virtual void init(metaData m, vector<User*> u, TCP * _tcp)=0;
    virtual metaData defaultOptions()=0;
};
