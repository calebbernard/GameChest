#pragma once

#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include "../dataStructures.h"
#include "../stringUtil.h"

using namespace std;

class riddle : public Module {
  class pile{
  public:
    int number;
    vector<bool> cards;
    int getUps(){
      int count = 0;
        for (int x = 0; x < cards.size(); x++){
          if (cards[x] == true){
            count++;
          }
        }
      return count;
    }
    void flip(int card){
      cards[card] = !cards[card];
    }
  };
public:
  vector <vector <pile> > worlds;

  string list(){
    string output = "";
    for (int x = 0; x < worlds.size(); x++){
      int ups = 0;
      output += "World " + itos(x) + ": ";
      for (int y = 0; y < worlds[x].size(); y++){
        int getUps = worlds[x][y].getUps();
        ups += getUps;
        output += (itos(worlds[x][y].number) + ". " + itos(getUps) + "/" + itos(worlds[x][y].cards.size()) + " ");
      }
      output += " - Total ups: " + itos(ups);
      output += "\n";
    }
    return output;
  }

  string reset(){
    if (worlds.size() > 0){
      worlds.erase(worlds.begin(), worlds.begin() + worlds.size());
    }
    int numWorlds = 10;
    for (int x = 0; x < numWorlds; x++){
      pile p;
      p.number = 0;
      for (int y = 0; y < 52; y++){
        p.cards.push_back(false);
      }
      while (p.getUps() != 13){
        p.flip(rand() % 52);
      }
      vector<pile> temp;
      temp.push_back(p);
      worlds.push_back(temp);
    }
    return list();
  }

  riddle(){
    reset();

    addCommand("flip", "flip a pile", "int", "number of pile to flip", StateChangeAction);
    addCommand("move", "move cards from one pile to another", "int int int", "number of from pile~number of to pile~number of cards to move", StateChangeAction);
    addCommand("list", "lists number of face-ups in each pile per deck", "", "", NonStateChangeAction);
    addCommand("reset", "Resets the worlds", "", "", StateChangeAction);
  }

  string instructions(){
    return "test";
  }

  string flip(string p){
    return "flipTest";
  }

  string move(string p1, string p2, string num){
    string output = "Error";
    int from = strtoi(p1);
    int to = -1;
    int n = strtoi(num);
    if (from >= worlds[0].size()){
      return output;
    }
    for (int x = 0; x < worlds[0].size(); x++){
      if (worlds[0][x].number == strtoi(p2)){
        to = strtoi(p2);
      }
    }
    if (to == -1){
      for (int x = 0; x < worlds.size(); x++){
        pile p;
        p.number = strtoi(p2);
        to = p.number;
        worlds[x].push_back(p);
      }
    }
    for (int x = 0; x < worlds.size(); x++){
      for (int z = 0; z < worlds[x].size(); z++){
        if (worlds[x][z].number == to){
          for (int y = 0; y < n; y++){
            worlds[x][to].cards.push_back(worlds[x][from].cards[y]);
          }
        }
      }
      worlds[x][from].cards.erase(worlds[x][from].cards.begin(), worlds[x][from].cards.begin() + n);
    }
    output = list();
    return output;
  }



  string flipPile(string p){
    string output = "";
    for (int x = 0; x < worlds.size(); x++){
      for (int y = 0; y < 52; y++){
        worlds[x][strtoi(p)].flip(y);
      }
    }
    output = list();
    return output;
  }

  string runCommand(vector<string> words, int arity){
      string output = "Error processing command...";
      if (words[0] == "flip" && arity == 1){
          output = flipPile(words[1]);
      } else if (words[0] == "move" && arity == 3){
          output = move(words[1], words[2], words[3]);
      } else if (words[0] == "list" && arity == 0){
          output = list();
      } else if (words[0] == "reset" && arity == 0){
        return reset();
      }
      return output;
  }
};
