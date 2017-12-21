#include "dataStructures.h"

    void Command::set(string keyword_ = "", string description_ = "No description has been entered for this command.", string argTypes_= "", string argDescriptions_ = "", ActionType action_ = MetaAction){
        keyword = keyword_;
        description = description_;
        action = action_;
        vector<string> argTypes;
        vector<string> argDescriptions;
        if (argTypes_ != ""){
            splitString(argTypes_, argTypes, " ");
        }
        if (argDescriptions_ != ""){
            splitString(argDescriptions_, argDescriptions, "~");
        }
        if (argTypes.size() != argDescriptions.size()){
            cout << "Internal error - Bad command initialization.\n";
            throw(0);
        }
        for (int x = 0; x < argTypes.size(); x++){
            Argument temp;
            temp.type = argTypes[x];
            temp.description = argDescriptions[x];
            argument.push_back(temp);
        }
    }
    string Command::describe(){
        int counter = 0;
        string output = keyword + " - " + description;
        output += "\n" + keyword + " takes " + itos(argument.size()) + " argument" + conditionalString(".", "s.", argument.size() == 1);
        for (int x = 0; x < argument.size(); x++){
            output += "\n\t" + itos(x+1) + ". " + argument[x].type + " - " + argument[x].description;
        }
        if (action == NonStateChangeAction){
          output += "\nThis action cannot change module state.";
        } else if (action == StateChangeAction){
          output += "\nThis action can change module state.";
        } else if (action == EndTurnAction){
          output += "\nThis action will end your turn.";
        } else if (action == MetaAction){
          output += "\nThis action has a meta-module effect.";
        }
        return output;
    }
    string Module::invalidInput(bool blankString = false){
        string output;
        if (blankString){
            output = "Currently avaliable commands:";
        } else {
            output = "Invalid command. Current options:";
        }
        for (int x = 0; x < availableCommands.size(); x++){
            output += "\n\t" + itos(x) + ". " + availableCommands[x].keyword + " (" + itos(availableCommands[x].argument.size()) + " argument" + conditionalString("", "s", availableCommands[x].argument.size() == 1) + ")";
        }
        output += "\nFor more instructions about a particular command on this list, type \"[command name] [# of args]?\"";
        return output;
    }
    string Module::parse(string request){
        string output = invalidInput();
        vector<string> words;
        splitString(request, words, " ");
        bool foundMatch = 0;
        if (words[0] == ""){
            return invalidInput(true);
        }
        if (words.size() == 2 && words[1].substr(words[1].length()-1, words[1].length()) == "?" && isNum(words[1].substr(0, words[1].length()-1))){
            for (int x = 0; x < availableCommands.size(); x++){
                if ((words[0] == availableCommands[x].keyword) && (strtoi(words[1].substr(0,words[1].length()-1)) == availableCommands[x].argument.size())){
                    output = availableCommands[x].describe();
                }
            }
        }
        else {
            for (int x = 0; x < availableCommands.size(); x++){
                if (words[0] == availableCommands[x].keyword){
                    if (words.size() - 1 == availableCommands[x].argument.size()){
                        output = runCommand(words, words.size() - 1);
                    }
                }
            }
        }
        return output;
    }

    void Module::removeCommand(string keyword_, int arity_){
        for (int x = 0; x < availableCommands.size(); x++){
            if (keyword_ == availableCommands[x].keyword && arity_ == availableCommands[x].argument.size()){
                availableCommands.erase(availableCommands.begin() + x);
            }
        }
    }

    void Module::addCommand(string keyword, string description, string argTypes, string argDescriptions, ActionType action){
        vector<string> args;
        if (argTypes != ""){
            splitString(argTypes, args, " ");
        }
        removeCommand(keyword, args.size());
        Command c;
        c.set(keyword, description, argTypes, argDescriptions, action);
        availableCommands.push_back(c);
    }

    Result::Result(ResultHeader r, string msg){
        header = r;
        message = msg;
    }

    string Result::get(){
        string output = "";
        if (header == Success){
            output = "Success";
        } else if (header == Failure){
            output = "Failure";
        } else if (header == Tie){
            output = "Tie";
        } else if (header == Error){
            output = "Error";
        } else {
            output = "Finish";
        }
        if (message != ""){
            output += " - ";
        }
        output += message;
        return output;
    }

    void Module::broadcast(string msg){
        for (int x = 0; x < users.size(); x++){
            if (users[x]->outputLast){
                tcp->input(users[x]);
            }
            tcp->output(users[x], "Broadcast: " + msg);
        }
    }

    void Module::turnManager(){
      broadcast("Module started");
      current = users[0];
      bool gameFinished = false;
      while (!gameFinished){
          string input = tcp->input(current);
          if (input == "#"){
            broadcast(current->name + " disconnected. Returning to lobby.");
            return;
          }
          tcp->output(current, parse(input));
          current = next();
      }
    }

    User * Module::next(){
      for (int x = 0; x < users.size(); x++){
        if (users[x]->conn == current->conn){
          if (x < users.size() - 1){
            return users[x+1];
          }
        }
      }
      return users[0];
    }

    User::User(){
        outputLast = false;
    }
