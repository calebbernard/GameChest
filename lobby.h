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
            output = "Module selected";
            m = new metaMabModule();
            for (int x = 0; x < m->options.options.size(); x++){
                cout << m->options.options[x].name << " - " << m->options.options[x].value << '\n';
            }
            game = new mabModule();
        }
        if (found){
            addCommand("listOptions", "List all of the settable options for this game.", "", "");
            addCommand("setOption", "Set a specific option", "string string", "Name of option~New value");
        }
        return output;
    }

    string listOptions(){
        for (int x = 0; x < m->options.options.size(); x++){
            
        }
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
        }
        return output;
    }
};