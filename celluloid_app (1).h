#ifndef CELLULOID_APP_H
#define CELLULOID_APP_H

#include <string>
#include <vector>

using namespace std;

// [PROJECT REQUIREMENT: Data structures for storing data]
// This struct acts as a custom data structure to store information about each heritage site/movie.
struct HeritageSite {
    string movieClue;
    string answer;
    // [PROJECT REQUIREMENT: Dynamic memory management]
    // std::vector automatically manages dynamic memory allocation on the heap as hints are added.
    vector<string> hints; 
};

class CelluloidApp {
private:
    int lives;
    int hintsAllowed;
    int difficultyLevel;
    int currentSiteIndex; // Tracks our place in the shuffled deck
    
    // [PROJECT REQUIREMENT: Data structures for storing data]
    // A vector is used to store multiple instances of the HeritageSite struct.
    vector<HeritageSite> siteDatabase;

    bool loadFiles();
    void playRound();
    void printFilmReel(int livesRemaining);

public:
    CelluloidApp(int difficulty);
    void run();
};

#endif
