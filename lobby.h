#pragma once

#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include "dataStructures.h"
#include "stringUtil.h"
#include "Modules/mabModule.h"

class Lobby : public Module{
    metaData * m;
    Game * game;
    vector<User> users;
public:
    Lobby(){
        addCommand("module", "Select a module", "string", "name of the module you want");
        addCommand("list", "List all available modules", "", "");
    }
    string listModules(){
        return "mab";
    }
    string instructions(){
        return "Testing";
    }
    string selectModule(string module){ // Add to this with first removing the option-altering commands, then at the end if found == true, add them back
        string output = "Module not found.";
        bool found = false;
        if (module == "mab"){
            found = true;
            m = new metaMabModule();
            game = new mabModule();
            output = m->name + " module selected.\nMaximum players: " + itos(m->maxPlayers);
        }
        if (found){
            addCommand("listOptions", "List all of the settable options for this game.", "", "");
            addCommand("setOption", "Set a specific option", "string string", "Option number~New value");
            addCommand("listUsers", "Lists the currently connected users, as well as remaining spots to be filled.", "", "");
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
                output += users[x].name;
                if (x < users.size() - 1){
                    output += "\n";
                }
            }
        }
        if (users.size() < m->maxPlayers){
            output += "Still waiting on " + itos(m->maxPlayers - users.size()) + conditionalPlural(" user.", " users.", (m->maxPlayers - users.size()));
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
        }
        return output;
    }
};