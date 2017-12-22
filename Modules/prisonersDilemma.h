// Prisoner's Dilemma
#pragma once

#include <stdlib.h>
#include "../dataStructures.h"
#include "../stringUtil.h"

using namespace std;

class metaPDModule : public metaData{
public:
    metaPDModule(){
        name = "Prisoner's Dilemma";
        numPlayersDesc = "requires an even number of players from 2-20";
        for (int x = 2; x < 20; x+=2){
          numPlayers.push_back(x);
        }
        //numPlayers.push_back(2);
        Option o;
        o.name = "Number of rounds";
        o.value = "1000";
        options.push_back(o);
    }
};

class prisonersDilemma : public Game {
public:
    void init(metaData m, vector<User*> u, TCP * _tcp){
        tcp = _tcp;
        users = u;
        addCommand("?", "Instructions for this room", "", "", NonStateChangeAction);
    }
    string instructions(){
      return "Test";
    }
    string runCommand(vector<string> words, int arity){
      string output = "Error processing command...";
      if (words[0] == "?" && arity == 0){
          output = instructions();
      }
      return output;
    }
    metaData* defaultOptions(){
      return new metaPDModule();
    }
};
