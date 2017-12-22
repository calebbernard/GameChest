// MultiArmedBandit
#pragma once

#include "../dataStructures.h"
#include "../stringUtil.h"

using namespace std;

class metaMabModule : public metaData{
public:
    metaMabModule(){
        name = "Multi-armed bandit";
        numPlayers.push_back(1);
        numPlayersDesc = "";
        Option o;
        o.name = "Number of machines";
        o.value = "1000";
        options.push_back(o);
        o.name = "Worst odds for a machine are 1/X";
        o.value = "500";
        options.push_back(o);
        o.name = "Number of turns";
        o.value = "10000";
        options.push_back(o);
    }
};

class mabModule : public Game{
    private:
        class mab{
            private:
                int count;
                int max;
                int turnsLeft;
                int startTurns;
                int score;
                vector<int> bandit;
            public:
            mab(int _count, int _max, int _startTurns);
            int getCount();
            int getTurns();
            int getScore();
            // For debug only
            int getChance(int x);
            int pullLever(int x);
            int getBest();
            void reset();
        };
        mab * g;
        int cheaterScore;
    public:
        void init(metaData m, vector<User*> u, TCP * _tcp);
        string reset();
        string pullLever(int lever);
        string getInfo();
        string results();
        string instructions();
        string runCommand(vector<string> words, int arity);
        metaData* defaultOptions();
        static string name();
};
