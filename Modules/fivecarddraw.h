// 5CardDraw
#include <stdlib.h>
#include "../dataStructures.h"
#include "../stringUtil.h"
using namespace std;

class metaPokerModule : public metaData{
public:
    metaPokerModule(){
        name = "Five-Card Draw Poker";
        for (int x = 1; x <= 7; x++){
          numPlayers.push_back(x);
        }
        Option o;
        o.name = "Starting funds";
        o.value = "1000";
        options.push_back(o);
        o.name = "Ante amount";
        o.value = "3";
        options.push_back(o);
    }
};

class fivecarddraw : public Game{
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
    class Player{
    public:
      User * u;
      int money;
      playingCardPile hand;
      int bet;
      bool playing;
      bool completedStage;
    };
    class Table{
    public:
      playingCardPile deck;
      playingCardPile discard;
      int pot;
    };
public:
  Table t;
  vector<Player> p;
  int anteCost;
  vector<Player> active;
  vector<string> gameStages;
  int currentStage;
  Player * current;
    void init(metaData m, vector<User*> u, TCP * _tcp){
        tcp = _tcp;
        users = u;
        anteCost = strtoi(m.options[1].value);
        t.deck.createDeck();
        t.deck.shuffle();
        for (int x = 0; x < u.size(); x++){
          Player temp;
          temp.u = u[x];
          temp.money = strtoi(m.options[0].value);
          temp.playing = true;
          temp.completedStage = false;
          p.push_back(temp);
        }
        gameStages.push_back("ante");
        gameStages.push_back("first draw");
        gameStages.push_back("first bet");
        gameStages.push_back("second draw");
        gameStages.push_back("second bet");
        gameStages.push_back("showdown");
        currentStage = 0;
        addCommand("?", "Instructions for this room", "", "", NonStateChangeAction);
        addCommand("myStatus", "Get your info at a glance", "", "", NonStateChangeAction);
    }
    string instructions(){
      string output = "Five card draw poker";
      return output;
    }
    string myStatus(){
      string output = current->u->name + "\n" + underline(current->u->name.length()) + "\n";
      output += "Current game state: " + gameStages[currentStage] + "\n";
      output += "Money: ";
      output += itos(current->money);
      output += "\nHand: ";
      if (current->hand.p.size() == 0){
        output += "empty";
      } else {
        for (int x = 0; x < current->hand.p.size(); x++){
          output += current->hand.p[x].sayName(false) + conditionalString("", " ", x == current->hand.p.size() - 1);
        }
      }
      return output;
    }
    string ante(){
      string output = "Error in ante logic.";
      if (current->money >= anteCost){
        output = "Successfully ante'd.";
        current->bet += anteCost;
        current->money -= anteCost;
        current->completedStage = true;
      } else {
        output = "Not enough money to meet ante - game forfeited.";
        t.pot += current->money;
        current->money = 0;
        current->playing = false;
      }
      return output;
    }
    string draw(){
      string output = "Error in card draw logic.";
      int numToDraw = 5 - current->hand.p.size();
      output = "Drew ";
      for (int y = 0; y < numToDraw; y++){
        playingCard c = t.deck.draw();
        output += conditionalString(", ", "", y != 0 && y != numToDraw - 1) + c.sayName() + conditionalString("", ", and ", y != numToDraw - 2);
        current->hand.addToTop(c);
      }
      output += ".";
      current->completedStage = true;
      return output;
    }
    string runCommand(vector<string> words, int arity){
      string output = "Error processing command...";
      if (words[0] == "?" && arity == 0){
          output = instructions();
      } else if (words[0] == "myStatus" && arity == 0){
          output = myStatus();
      } else if (words[0] == "ante" && arity == 0){
          output = ante();
      } else if (words[0] == "draw" && arity == 0){
        output = draw();
      }
      return output;
    }
    Player * nextp(){
      for (int x = 0; x < active.size(); x++){
        if (active[x].u->conn == current->u->conn){
          if (x < active.size() - 1){
            return &active[x+1];
          }
        }
      }
      return &active[0];
    }
    void prepareCommands(){
      string stage = gameStages[currentStage];
      if (stage == "ante"){
        removeAllActiveCommands();
        addCommand("ante", "pay the $" + itos(anteCost) + " ante to play. If you cannot pay, your money will go in the pot and you will forfeit.", "", "", StateChangeAction);
      } else if (stage == "first draw"){
        removeAllActiveCommands();
        addCommand("draw", "draw your cards", "", "", StateChangeAction);
      }
    }
    void advanceStage(){
      bool finished = true;
      for (int x = 0; x < active.size(); x++){
        if (!active[x].completedStage){
          finished = false;
        }
      }
      if (finished){
        currentStage++;
        currentStage %= gameStages.size();
        for (int x = 0; x < active.size(); x++){
          active[x].completedStage = false;
        }
      }
    }
    void refreshActiveGroup(){
      vector<Player> temp;
      for (int x = 0; x < active.size(); x++){
        if (active[x].playing){
          temp.push_back(active[x]);
        }
      }
      active = temp;
    }
    bool checkFinished(){ // should require more than one playing. last one playing is victor
      bool finished = true;
      for (int x = 0; x < p.size(); x++){
        if (p[x].playing){
          finished = false;
        }
      }
      return finished;
    }
    void turnManager(){
      for (int x = 0; x < p.size(); x++){
        if (p[x].playing){
          active.push_back(p[x]);
        }
      }
      int currentIndex = rand() % active.size();
      current = &active[currentIndex];
      bool gameFinished = false;
      while (!gameFinished){
          refreshActiveGroup();
          advanceStage();
          prepareCommands();
          string input = tcp->input(current->u);
          if (input == "#"){
            broadcast(current->u->name + " disconnected. Returning to lobby.");
            return;
          }
          tcp->output(current->u, parse(input));
          current = nextp();
          gameFinished = checkFinished();
      }
    }
    metaData* defaultOptions(){
      return new metaPokerModule();
    }
};
