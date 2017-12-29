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
    static string ranks[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
    static string suits[] = {"D", "S", "H", "C"};
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
        // find, erase
    };
    class Player : public User { // improvement on the current 5 card draw system I think
        public:
        playingCardPile hand;
        vector<string> books; // this should be checkable by enemies
        string turnStage; // asking, fishing, done
    };
public:
    vector<Player> p;
    Player * current;
    Player * backup;
    string demandRank;
    bool subTurnDone;
    playingCardPile deck;
    void init(metaData m, vector<User*> u, TCP * _tcp){
        broadcast("Welcome to Go Fish!");
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
    string myInfo(){
        for (int x = 0; x < p.size(); x++){
          tcp->output(&p[x], current->name + " is checking their info");
        }
        string output = "";
        output += current->name + "\n";
        output += underline(current->name.length()) + "\n";
        output += "Hand: ";
        output += "";
        if (current->hand.p.size() == 0){
            output += "";
        } else {
            for (int x = 0; x < current->hand.p.size(); x++){
                output += current->hand.p[x].sayName(false) + conditionalString("", ", ", x == current->hand.p.size() - 1);
            }
        }
        output += "\nBooks: ";
        for (int x = 0; x < current->books.size(); x++){
          output += current->books[x] + conditionalString("", ", ", x == current->books.size() - 1);
        }
        output += "\n\n";
        for (int x = 0; x < p.size(); x++){
          if (p[x].conn != current->conn){
            output += p[x].name + "\n" + underline(p[x].name.length()) + "\nBooks: ";
            if (p[x].books.size() != 0){
              for (int y = 0; y < p[x].books.size(); y++){
                output += p[x].books[y] + conditionalString("", ", ", y == p[x].books.size() - 1);
              }
            }
          }
        }
        return output;
    }
    void checkBooks(){
      string ranks[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
      for (int x = 0; x < 13; x++){
        for (int y = 0; y < p.size(); y++){
          int count = 0;
          vector<int> toErase;
          for (int z = 0; z < p[y].hand.p.size(); z++){
            if (p[y].hand.p[z].rank == ranks[x]){
              count++;
              toErase.push_back(z);
            }
          }
          if (count >= 4){
            for (int zz = 0; zz < toErase.size(); zz++){
              p[y].hand.p.erase(p[y].hand.p.begin() + zz);
            }
            p[y].books.push_back(ranks[x]);
            broadcast(p[y].name + " has a book of " + ranks[x] + "!");
          }
        }
      }
    }
    string giveCards(){
      playingCardPile giving;
      vector<int> toErase;
      for (int x = 0; x < current->hand.p.size(); x++){
        if (current->hand.p[x].rank == demandRank){
          giving.addToTop(current->hand.p[x]);
          toErase.push_back(x);
        }
      }
      for (int x = 0; x < toErase.size(); x++){
        current->hand.p.erase(current->hand.p.begin() + toErase[x]);
      }
      for (int x = 0; x < giving.p.size(); x++){
        backup->hand.addToBottom(giving.p[x]);
      }
      broadcast(current->name + " gives " + backup-> name + " " + itos(giving.p.size()) + " " + giving.p[0].rank + conditionalString("s.", ".", giving.p.size() > 1));
      current->turnStage = "done";
      current = backup;
      current->turnStage = "done";
      cout << "HERE!!!\n";
      checkBooks();
      return "";
    }
    string tellToFish(){
      broadcast(current->name + " tells " + backup->name + " to go fish.");
      current->turnStage = "done";
      current = backup;
      current->turnStage = "fishing";
      return "";
    }
    string ask(string player, string rank){
        string output = "";
        if (player == current->name){
          return "You can't ask yourself for cards.";
        }
        if (rank == "j"){
            rank = "J";
        }
        if (rank == "q"){
            rank = "Q";
        }
        if (rank == "k"){
            rank = "K";
        }
        if (rank == "a"){
            rank = "A";
        }
        bool validRank = false;
        if ((strtoi(rank) >= 2 && strtoi(rank) <= 10) || rank == "J" || rank == "Q" || rank == "K" || rank == "A"){
          validRank = true;
        }
        if (!validRank){
          return "That rank not recognized.";
        }
        bool hasAtLeastOne = false;
        for (int x = 0; x < current->hand.p.size(); x++){
          if (rank == current->hand.p[x].rank){
            hasAtLeastOne = true;
          }
        }
        if (!hasAtLeastOne){
          return "You don't have that rank yourself, so you can't ask for it.";
        }
        bool validPlayer = false;
        for (int x = 0; x < p.size(); x++){
          if (player == p[x].name){
            validPlayer = true;
            backup = current;
            current = &p[x];
            
            demandRank = rank;
            string msg = backup->name + " says: Do you have any " + demandRank + "s?";
            tcp->output(current, msg);
            bool hasHuntedCards = false;
            for (x = 0; x < current->hand.p.size(); x++){
              if (current->hand.p[x].rank == demandRank){
                hasHuntedCards = true;
              }
            }
            if (hasHuntedCards){
              current->turnStage = "giving cards";
            } else {
              current->turnStage = "saying go fish";
            }
          }
        }
        if (!validPlayer){
          return "That player not found.";
        }
        return output;
    }
    string fish(){
        string output = "";
        playingCard c = deck.draw();
        current->hand.addToBottom(c);
        broadcast(current->name + " drew the " + c.sayName() + ".");
        checkBooks();
        if (c.rank == demandRank){
          broadcast("Since this is what they were looking for, they get another turn!");
          current->turnStage = "asking";
        } else {
          output = "Sorry, turn over!";
          current->turnStage = "done";
        }
        return output;
    }
    string runCommand(vector<string> words, int arity){
      string output = "Error processing command...";
      if (words[0] == "?" && arity == 0){
          output = instructions();
      } else if (words[0] == "myHand" && arity == 0){
          output = myInfo();
      } else if (words[0] == "ask" && arity == 2){
          output = ask(words[1], words[2]);
      } else if (words[0] == "goFish" && arity == 0){
          output = fish();
      } else if (words[0] == "giveCards" && arity == 0){
        output = giveCards();
      } else if (words[0] == "tellGoFish" && arity == 0){
        output = tellToFish();
      }
      return output;
    }
    Player * nextp(){
      Player * next;
      bool nextFound = false;
      for (int x = 0; x < p.size(); x++){
        if (p[x].conn == current->conn){
          if (x < p.size() - 1){
            next = &p[x+1];
            nextFound = true;
          }
        }
      }
      if (!nextFound){
        next = &p[0];
      }
      return next;
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
        addCommand("goFish", "Draw a card from the deck. If it's the card you asked for, you get another turn.", "", "", StateChangeAction);
      } else if (stage == "giving cards"){
        removeAllActiveCommands();
        addCommand("giveCards", "Give " + backup->name + " the cards they've requested.", "", "", StateChangeAction);
      } else if (stage == "saying go fish"){
        removeAllActiveCommands();
        addCommand("tellGoFish", "Tell " + backup->name + " to go fish", "", "", StateChangeAction);
      }
    }
    void turnManager(){
      int currentIndex = rand() % p.size();
      current = &p[currentIndex];
      bool gameFinished = false;
      while (!gameFinished){
          // add checkVictory method
          prepareCommands();
          string input = tcp->input(current);
          if (input == "#"){
            broadcast(current->name + " disconnected.");
            broadcast("#");
            throw(1);
          }
          tcp->output(current, parse(input));
          if (current->turnStage == "done"){
            current = nextp();
            current->turnStage = "asking";
          }
      }
    }
    metaData* defaultOptions(){
      return new metaGoFishModule();
    }
};