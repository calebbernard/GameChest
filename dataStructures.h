#pragma once

#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include "stringUtil.h"
#include "io.h"
using namespace std;

class TCP;

class Argument{
public:
    string type; // this should become an enum
    string description;
};

enum ActionType {NonStateChangeAction, StateChangeAction, MetaAction, EndTurnAction};

class Command{
public:
    string keyword;
    string description;
    vector<Argument> argument;
    ActionType action;
    void set(string keyword_, string description_, string argTypes_, string argDescriptions_, ActionType action_);
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
    vector<int> numPlayers;
    // this is optional; set it to "" in most cases or something else if there's
        // a pattern or rule to your number of players, like 'any even number from 2-100',
        // and it would be easier to just say that than to list out every acceptable
        // number to the user.
    string numPlayersDesc;
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

class Module{
public:
    TCP * tcp;
    vector<User*> users;
    User * current;
    vector<Command> availableCommands;
    virtual string instructions()=0;
    virtual string runCommand(vector<string> words, int arity)=0;
    virtual string invalidInput(bool blankString);
    virtual string parse(string request);
    virtual void addCommand(string keyword, string description, string argTypes, string argDescriptions, ActionType action);
    virtual void removeCommand(string keyword_, int arity_);
    virtual void removeAllActiveCommands();
    virtual void turnManager();
    virtual User * next();
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
    virtual metaData* defaultOptions()=0;
};
