#include "mabModule.h"
#include "../stringUtil.h"

using namespace std;

            mabModule::mab::mab(int _count, int _max, int _startTurns){
                score = 0;
                count = _count;
                max = _max;
                startTurns = _startTurns;
                turnsLeft = startTurns;
                for (int x = 0; x < count; x++){
                    bandit.push_back(rand() % (max - 1) + 1);
                }
            }

            int mabModule::mab::getCount(){
                return count;
            }

            int mabModule::mab::getTurns(){
                return turnsLeft;
            }

            int mabModule::mab::getScore(){
                return score;
            }

            // For debug only
            int mabModule::mab::getChance(int x){
                return bandit[x];
            }

            int mabModule::mab::pullLever(int x){
                if (x >= 0 && x <= count && turnsLeft > 0){
                    turnsLeft--;
                    int pull = rand() % bandit[x];
                    if (pull == 0){
                        score++;
                        return 1;
                    }
                    return 0;
                }
                return -1;
            }

            int mabModule::mab::getBest(){
                int champ = 0;
                for (int x = 0; x < bandit.size(); x++){
                    if (bandit[x] < bandit[champ]){
                        champ = x;
                    }
                }
                return champ;
            }

            void mabModule::mab::reset(){
                turnsLeft = startTurns;
                score = 0;
            }

            mabModule::mabModule(){
                return;
            }

        void mabModule::init(metaData m, vector<User*> u, TCP * _tcp){
            tcp = _tcp;
            users = u;
            addCommand("?", "Instructions for this room", "", "", NonStateChangeAction);
            addCommand("pullLever", "Pull a certain lever", "int", "Lever you want to pull", StateChangeAction);
            addCommand("getInfo", "Get the number of machines and number of turns remaining", "", "", NonStateChangeAction);
            g = new mab(strtoi(m.options[0].value), strtoi(m.options[1].value), strtoi(m.options[2].value));
            int champ = g->getBest();
            while (g->getTurns() > 0){
                g->pullLever(champ);
            }
            cheaterScore = g->getScore();
            g->reset();
        }

        string mabModule::pullLever(int lever){
            string output = "";
            lever--;
            if (lever < 0 || lever >= g->getCount()){
                output = "Please pull a lever from 1 to " + itos((g->getCount())) + ".";
            } else {
                int result = g->pullLever(lever);
                if (result == 1){
                    output = "Success";
                } else {
                    output = "Failure";
                }
            }
            if (g->getTurns() < 1){
                addCommand("result", "Check your results", "", "", NonStateChangeAction);
                removeCommand("pullLever", 1);
                removeCommand("getInfo", 0);
            }
            return output  + " - Pulls left: " + itos(g->getTurns());
        }

        string mabModule::getInfo(){
            string output = "";
            output += "Number of machines: " + itos(g->getCount()) + "\n";
            output += "Current score: " + itos(g->getScore()) + "\n";
            output += "Turns remaining: " + itos(g->getTurns()) + "\n";
            output += "Score to beat: " + itos(cheaterScore/2);
            return output;
        }

        string mabModule::results(){
            string output = "";
            if (g->getScore() >= cheaterScore/2){
                output = "You win!";
            } else {
                output = "You lose.";
            }
            return output;
        }

        string mabModule::instructions(){
            string output = "";
            output = "There are a number of slot machines here. Each one, on pulling its lever, returns a success or a failure value."
                    "\nEach one returns these values according to randomized odds unique to each machine. For instance,"
                    "\nOne machine might have 1 in 3 odds, another might have 1 in 20, etc."
                    "\nYou have a finite amount of lever pulls, and must maximize your payout."
                    "\nAn omniscient player with foreknowledge of the machine with the best odds will play first."
                    "\nYou have to attain at least half of this player's score to succeed.";
            return output;
        }

        string mabModule::runCommand(vector<string> words, int arity){
            string output = "Error processing command...";
            if (words[0] == "?" && arity == 0){
                output = instructions();
            } else if (words[0] == "pullLever" && arity == 1){
                cout << strtoi(words[1]) << "\n";
                output = pullLever(strtoi(words[1]));
            } else if (words[0] == "getInfo" && arity == 0){
                output = getInfo();
            } else if (words[0] == "result" && arity == 0){
              output = results();
            }
            return output;
        }

        metaData mabModule::defaultOptions(){
            metaMabModule m;
            return m;
        }

        string mabModule::name(){
            return "mab";
        }
