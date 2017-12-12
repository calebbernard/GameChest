#pragma once

#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include "dataStructures.h"
#include "stringUtil.h"

class Lobby : public Module{
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
    string selectModule(string m){ // Add to this with first removing the option-altering commands, then at the end if found == true, add them back
        output = "";
        bool found = false;
        if (m == "mab"){
            changeModule = new mabModule();
            metaData m = new metaMabModule();
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
        }
        return output;
    }
};