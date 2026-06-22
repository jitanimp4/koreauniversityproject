#ifndef CELLULOID_APP_H
#define CELLULOID_APP_H

#include <string>
#include <vector>

using namespace std;

struct HeritageSite {
    string movieClue;
    string answer;
    vector<string> hints;
};

class CelluloidApp {
private:
    int lives;
    int hintsAllowed;
    int difficultyLevel;
    int currentSiteIndex; // NEW: Tracks our place in the shuffled deck
    vector<HeritageSite> siteDatabase;

    bool loadFiles();
    void playRound();
    void printFilmReel(int livesRemaining);

public:
    CelluloidApp(int difficulty);
    void run();
};

#endif
