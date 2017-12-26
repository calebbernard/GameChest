// GoFish
#include <stdlib.h>
#include "../dataStructures.h"
#include "../stringUtil.h"
using namespace std;

class metaGoFishModule : public metaData{
public:
    metaGoFishModule(){
        name = "Go Fish";
        numPlayers.push_back(2);
        Option o;
        o.name = "Number of starting cards";
        o.value = "7";
        options.push_back(o);
    }
};

class goFish : public Game {
private:
class playingCard{
    public:
        string rank;
        string suit;
        void resolveNum(int num){
            if (num < 13){
                suit = "D";
            } else if (num < 26){
                suit = "S";
            } else if (num < 39){
                suit = "H";
            } else {
                suit = "C";
            }
            num %= 13;
            if (num == 0){
              rank = "A";
            } else if (num == 1){
              rank = "2";
            } else if (num == 2){
              rank = "3";
            } else if (num == 3){
              rank = "4";
            } else if (num == 4){
              rank = "5";
            } else if (num == 5){
              rank = "6";
            } else if (num == 6){
              rank = "7";
            } else if (num == 7){
              rank = "8";
            } else if (num == 8){
              rank = "9";
            } else if (num == 9){
              rank = "10";
            } else if (num == 10){
              rank = "J";
            } else if (num == 11){
              rank = "Q";
            } else if (num == 12){
              rank = "K";
            }
        }
        string sayName(bool extended=true){
          string output = "";
          if (extended){
              if (rank == "A"){
                output += "Ace";
              } else if (rank == "2"){
                output += "Two";
              } else if (rank == "3"){
                output += "Three";
              } else if (rank == "4"){
                output += "Four";
              } else if (rank == "5"){
                output += "Five";
              } else if (rank == "6"){
                output += "Six";
              } else if (rank == "7"){
                output += "Seven";
              } else if (rank == "8"){
                output += "Eight";
              } else if (rank == "9"){
                output += "Nine";
              } else if (rank == "10"){
                output += "Ten";
              } else if (rank == "J"){
                output += "Jack";
              } else if (rank == "Q"){
                output += "Queen";
              } else if (rank == "K"){
                output += "King";
              }
              output += " of ";
              if (suit == "D"){
                output += "Diamonds";
              } else if (suit == "S"){
                output += "Spades";
              } else if (suit == "H"){
                output += "Hearts";
              } else if (suit == "C"){
                output += "Clubs";
              }
          } else {
            output += rank;
            output += suit;
          }
          return output;
        }
    };
    class playingCardPile{
    public:
        vector<playingCard> p;
        void createDeck(){
            for (int x = 0; x < 52; x++){
              playingCard c;
              c.resolveNum(x);
              p.push_back(c);
            }
        }
        void shuffle(){
          vector<playingCard> temp;
          while (p.size() > 0){
            int choice = rand() % p.size();
            temp.push_back(p[choice]);
            p.erase(p.begin() + choice);
          }
          for (int x = 0; x < temp.size(); x++){
            p.push_back(temp[x]);
          }
        }
        playingCard draw(){
          playingCard c = p[0];
          p.erase(p.begin());
          return c;
        }
        void addToTop(playingCard c){
          p.insert(p.begin(), c);
        }
        void addToBottom(playingCard c){
          p.insert(p.begin() + p.size(), c);
        }
    };
    class Player : public User { // improvement on the current 5 card draw system I think
        public:
        playingCardPile hand;
        vector<int> books; // this should be checkable by enemies
        string turnStage; // asking, fishing, done
    };
public:
    vector<Player> p;
    Player * current;
    playingCardPile deck;
    void init(metaData m, vector<User*> u, TCP * _tcp){
        tcp = _tcp;
        users = u;
        int startingHandSize = strtoi(m.options[0].value);
        deck.createDeck();
        deck.shuffle();
        for (int x = 0; x < u.size(); x++){
          Player temp;
          temp.name = u[x]->name;
          temp.conn = u[x]->conn;
          temp.turnStage = "asking";
          for (int x = 0; x < startingHandSize; x++){
            temp.hand.addToTop(deck.draw());
          }
          p.push_back(temp);
        }
        addCommand("?", "Instructions for this room", "", "", NonStateChangeAction);
        addCommand("myHand", "Check your current hand", "", "", NonStateChangeAction);
    }
    string instructions(){
      string output = "Go Fish";
      return output;
    }
    string myHand(){
        string output = "";
        if (current->hand.p.size() == 0){
            output += "empty";
        } else {
            for (int x = 0; x < current->hand.p.size(); x++){
                output += current->hand.p[x].sayName(false) + conditionalString("", " ", x == current->hand.p.size() - 1);
            }
        }
        return output;
    }
    string ask(string player, string rank){
        string output = "";
        output += "test";
        return output;
    }
    string fish(){
        string output = "";
        playingCard c = deck.draw();
        current->hand.addToTop(c);
        output += "Drew " + c.sayName();
        return output;
    }
    string runCommand(vector<string> words, int arity){
      string output = "Error processing command...";
      if (words[0] == "?" && arity == 0){
          output = instructions();
      } else if (words[0] == "myHand" && arity == 0){
          output = myHand();
      } else if (words[0] == "ask" && arity == 2){
          output = ask(words[1], words[2]);
      } else if (words[0] == "goFish" && arity == 0){
          output = fish();
      }
      return output;
    }
    Player * nextp(){
      for (int x = 0; x < p.size(); x++){
        if (p[x].conn == current->conn){
          if (x < p.size() - 1){
            return &p[x+1];
          }
        }
      }
      return &p[0];
    }
    void prepareCommands(){
      string stage = current->turnStage;
      if (stage == "asking"){
        removeAllActiveCommands();
        string enemiesList = "";
        for (int x = 0; x < p.size(); x++){
            if (p[x].conn != current->conn){
                enemiesList += p[x].name + conditionalString("", ", ", x == p.size() - 1);
            }
        }
        string ownedRanks = "";
        vector<string> unique;
        for (int x = 0; x < current->hand.p.size(); x++){
            bool u = true;
            for (int y = 0; y < unique.size(); y++){
                if (current->hand.p[x].rank == unique[y]){
                    u = false;
                }
            }
            if (u){
                unique.push_back(current->hand.p[x].rank);
            }
        }
        for (int x = 0; x < unique.size(); x++){
            ownedRanks += unique[x] + conditionalString("", ", ", x == unique.size() - 1);
        }
        addCommand("ask", "ask a user for a rank", "string string", "name of user to ask #COLLECTION(" + enemiesList + ")~rank to ask for #COLLECTION(" + ownedRanks + ")", StateChangeAction);
      } else if (stage == "fishing"){
        removeAllActiveCommands();
        addCommand("goFish", "Draw a card from the deck", "", "", StateChangeAction);
      }
    }
    void turnManager(){
      int currentIndex = rand() % p.size();
      current = &p[currentIndex];
      bool gameFinished = false;
      while (!gameFinished){
          prepareCommands();
          string input = tcp->input(current);
          if (input == "#"){
            broadcast(current->name + " disconnected. Returning to lobby.");
            return;
          }
          tcp->output(current, parse(input));
          current = nextp();
      }
    }
    metaData* defaultOptions(){
      return new metaGoFishModule();
    }
};