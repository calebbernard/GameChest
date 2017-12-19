#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include "dataStructures.h"
#include "stringUtil.h"
#include "Modules/mabModule.h"
#include "io.h"

class TCP;

class Lobby : public Module{
    metaData * m;
    Game * game;
    User * host;
public:
    Lobby(TCP * _tcp, User * host_){
        users.push_back(host_);
        host = host_;
        addCommand("module", "Select a module", "string", "name of the module you want", StateChangeAction);
        addCommand("list", "List all available modules", "", "", NonStateChangeAction);
        tcp = _tcp;
    }

    User * next(){
        return host;
    }

    string listModules(){
        return "mab";
    }
    string instructions(){
        return "Testing";
    }

    bool validNumUsers(){
      bool output = false;
      for (int x = 0; x < m->numPlayers.size(); x++){
        if (users.size() == m->numPlayers[x]){
          output = true;
        }
      }
      return output;
    }

    // Takes a vector<int>, returns true if the collection is consecutive and ascending, otherwise returns false.
    bool consecutiveAscendingInts(vector<int> list){
        bool output = true;
        int first = list[0];
        for (int x = 0; x < list.size(); x++){
            if (list[x] != first + x){
                output = false;
            }
        }
        return output;
    }

    string listRequiredUsers(){
      string output = m->name + " ";
      if (m->numPlayers.size() == 1){
        output += "requires " + itos(m->numPlayers[0]) + " player" + conditionalPlural(".", "s.", m->numPlayers[0]);
      } else {
          if (consecutiveAscendingInts(m->numPlayers)){
              output += "is for " + itos(m->numPlayers[0]) + "-" + itos(m->numPlayers[m->numPlayers.size()-1]) + " players.";
          } else {
              output += "can be played with ";
              for (int x = 0; x < m->numPlayers.size(); x++){
                  output += itos(m->numPlayers[x]);
                  if (x < m->numPlayers.size() - 2){
                    output += ", ";
                  } else if (x < m->numPlayers.size() - 1){
                    output += " or ";
                  } else {
                    output += " players.";
                  }
              }
          }
      }
      output += "\nCurrently there " + conditionalPlural("is", "are", users.size()) + " " + itos(users.size()) + " " + conditionalPlural("player.", "players.", users.size());
      if (validNumUsers()){
          output += " Module can be launched.";
      } else {
          output += " Module not ready for launch.";
      }
      return output;
    }

    string selectModule(string module){ // Add to this with first removing the option-altering commands, then at the end if found == true, add them back
        string output = "Module not found.";
        bool found = false;
        if (module == "mab"){
            found = true;
            m = new metaMabModule();
            game = new mabModule();
            output = m->name + " module selected.\n";
            sort(m->numPlayers.begin(), m->numPlayers.begin() + m->numPlayers.size());
            output += listRequiredUsers();
        }
        if (found){
            addCommand("listOptions", "List all of the settable options for this game.", "", "", NonStateChangeAction);
            addCommand("setOption", "Set a specific option", "string string", "Option number~New value", StateChangeAction);
            addCommand("listUsers", "Lists the currently connected users, as well as remaining spots to be filled.", "", "", NonStateChangeAction);
            addCommand("connect", "Connect to a user", "", "", StateChangeAction);
            addCommand("kick", "kick a user", "int", "the user's lobby number", StateChangeAction);
            addCommand("testPlay", "test", "", "", MetaAction);
            addCommand("promote", "Promote a user to host", "int", "the user's lobby number", MetaAction);
        }
        return output;
    }

    string listOptions(){
        string output = "";
        for (int x = 0; x < m->options.size(); x++){
            output += (itos(x+1) + ". " + m->options[x].name + " - " + m->options[x].value);
            if (x < m->options.size() - 1){
                output += "\n";
            }
        }
        return output;
    }

    string setOption(string option, string value){
        string output = "Invalid option.";
        int o = strtoi(option);
        o--;
        if (o >= 0 && o < m->options.size()){
            for (int x = 0; x < m->options.size(); x++){
                if (x == o){
                    m->options[x].value = value;
                    output = m->options[x].name + " set to " + value;
                }
            }
        }
        return output;
    }

    string getUsers(){
        string output = "";
        if (users.size() == 0){
            output = "No users connected.\n";
        } else {
            for (int x = 0; x < users.size(); x++){
                output += itos(x + 1) + ". " + users[x]->name + " - " + itos(users[x]->conn) + "\n";
            }
        }
        output += listRequiredUsers();
        return output;
    }

    string loadUser(){
        string output = "Unknown error.";
        cout << "Loading user...\n";
        User * u = new User();
        u->conn = tcp->connect();
        tcp->output(u, "Welcome! What is your name?");
        u->name = tcp->input(u);
        tcp-> output(u, "Cool. Please wait for launch.");
        users.push_back(u);
        output = "User " + u->name + " loaded!";
        return output;
    }

    string kick(string user){
        string output = "Invalid user.";
        int u = strtoi(user);
        u--;
        if (u >= 0 && u < users.size()){
            tcp->output(users[u], "#");
            output = "User " + users[u]->name + " kicked.";
            delete users[u];
            users.erase(users.begin()+u);
        }
        return output;
    }

    string testPlay(){
      string output = "Unknown error.";
      if (validNumUsers()){
        game->init(*m, users, tcp);
        game->turnManager();
      } else {
        output = "Cannot start module: Invalid number of users in lobby.";
      }
      return output;
    }

    string promote(string user){
        string output = "Invalid user.";
        int u = strtoi(user);
        u--;
        if (u >= 0 && u < users.size()){
            host = users[u];
            output = users[u]->name + " promoted to host.";
        }
        return output;
    }

    string runCommand(vector<string> words, int arity){
        string output = "Error processing command...";
        if (words[0] == "?" && arity == 0){
            output = instructions();
        } else if (words[0] == "list" && arity == 0){
            output = listModules();
        } else if (words[0] == "module" && arity == 1){
            output = selectModule(words[1]);
        } else if (words[0] == "listOptions" && arity == 0){
            output = listOptions();
        } else if (words[0] == "setOption" && arity == 2){
            output = setOption(words[1], words[2]);
        } else if (words[0] == "listUsers" && arity == 0){
            output = getUsers();
        } else if (words[0] == "connect" && arity == 0){
            output = loadUser();
        } else if (words[0] == "kick" && arity == 1){
            output = kick(words[1]);
        } else if (words[0] == "testPlay" && arity == 0){
          output = testPlay();
        } else if (words[0] == "promote" && arity == 1){
          output = promote(words[1]);
        }
        return output;
    }
};
