#pragma once;

#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include "dataStructures.h"
#include "stringUtil.h"

class lobby : public Module{
public:
    lobby(){
        addCommand("module", "Select a module", "string", "name of the module you want");
        addCommand("list", "List all available modules", "", "");
    }
    string listModules(){
        return "mab";
    }
    string runCommand(vector<string> words, int arity){
        string output = "Error processing command...";
        if (words[0] == "?" && arity == 0){
            output = instructions();
        } else if (words[0] == "list" && arity == 0){
            output = listModules();
        }
        return output;
    }
};