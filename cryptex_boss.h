#ifndef CRYPTEX_BOSS_H
#define CRYPTEX_BOSS_H

#include <string>
#include <vector>
using namespace std;

struct TriviaQuestion
{
    string difficulty;
    string question;
    string answer;
    string hint;
    bool bonusLife;
};

void runCryptexBoss(string mode);

#endif
