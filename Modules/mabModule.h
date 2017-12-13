#pragma once

#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include "../dataStructures.h"
#include "../stringUtil.h"

using namespace std;

class metaMabModule : public metaData{
public:
    metaMabModule(){
        name = "mab";
        numPlayers = 1;
        Option o;
        o.name = "Number of machines";
        o.value = "1000";
        options.options.push_back(o);
        o.name = "Worst odds for a machine are 1/X";
        o.value = "500";
        options.options.push_back(o);
        o.name = "Number of turns";
        o.value = "10000";
        options.options.push_back(o);
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
        mabModule();
        void init(Options o);
        string reset();
        string pullLever(int lever);
        string getInfo();
        string results();
        string instructions();
        string runCommand(vector<string> words, int arity);
        Options defaultOptions();
        static string name();
};