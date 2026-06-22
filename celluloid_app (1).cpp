#include "celluloid_app.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cctype> 
#include <algorithm> 
#include <random>

using namespace std;

/*
 * Function: CelluloidApp (Constructor)
 * What it does: Initializes the game state, setting the player's lives and available hints based on the chosen difficulty.
 * What the inputs are: int difficulty (1 for Easy, 2 for Medium, 3 for Hard)
 * What the outputs are: None (Constructor)
 */
CelluloidApp::CelluloidApp(int difficulty) {
    // [PROJECT REQUIREMENT: Multiple Difficulty Levels]
    // The game alters its parameters (lives and hints) based on the user's difficulty choice.
    difficultyLevel = difficulty;
    currentSiteIndex = 0; 

    if (difficulty == 1) { // Easy
        lives = 4;
        hintsAllowed = 3;
    } else if (difficulty == 2) { // Medium
        lives = 3;
        hintsAllowed = 2;
    } else { // Hard
        lives = 2;
        hintsAllowed = 1;
    }
}

/*
 * Function: loadFiles
 * What it does: Reads movie data from an external text file, parses it using a delimiter, stores it into a data structure, and shuffles the deck.
 * What the inputs are: None
 * What the outputs are: Returns a boolean (true if the file loaded successfully, false if it failed)
 */
bool CelluloidApp::loadFiles() {
    // [PROJECT REQUIREMENT: File input/output]
    // Loads game data from a file rather than hardcoding it.
    ifstream file("movies.txt");
    if (!file.is_open()) {
        // Console Output (I/O)
        cout << "\033[1;31m [ERROR] Missing 'movies.txt' database file.\033[0m\n";
        return false;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue; // Skip blank lines
        
        stringstream ss(line);
        string item;
        HeritageSite site;

        // Parse the pipe-separated file correctly
        getline(ss, site.movieClue, '|');
        getline(ss, site.answer, '|');
        
        // Push remaining segments as hints
        while (getline(ss, item, '|')) {
            // [PROJECT REQUIREMENT: Dynamic memory management] 
            // The vector dynamically expands its allocated heap memory to fit new hints.
            site.hints.push_back(item);
        }
        
        siteDatabase.push_back(site);
    }
    
    file.close();
    
    if (siteDatabase.empty()) {
        cout << "\033[1;31m [ERROR] movies.txt is empty.\033[0m\n";
        return false;
    }
    
    // [PROJECT REQUIREMENT: Generation of random events]
    // Uses a random device and Mersenne Twister engine to randomly shuffle the questions.
    random_device rd;
    mt19937 g(rd());
    shuffle(siteDatabase.begin(), siteDatabase.end(), g);
    
    return true;
}

/*
 * Function: printFilmReel
 * What it does: Visually displays the player's remaining lives using ASCII art.
 * What the inputs are: int livesRemaining (the current number of lives the player has)
 * What the outputs are: None (Prints directly to standard output)
 */
void CelluloidApp::printFilmReel(int livesRemaining) {
    // [PROJECT REQUIREMENT: File input/output] -> Standard Output (Console I/O)
    cout << "\n[FILM REEL: " << livesRemaining << " FRAMES LEFT]\n";
    for(int i = 0; i < livesRemaining; i++) {
        cout << "[O] ";
    }
    cout << "\n\n";
}

/*
 * Function: playRound
 * What it does: Manages a single round of the game, including fetching a question, accepting player guesses, evaluating accuracy, and providing hints.
 * What the inputs are: None
 * What the outputs are: None (Alters internal game state and outputs to console)
 */
void CelluloidApp::playRound() {
    if (static_cast<size_t>(currentSiteIndex) >= siteDatabase.size()) {
        cout << "\n\033[1;36m[SYSTEM] All records decrypted. Reshuffling the archive...\033[0m\n";
        
        // [PROJECT REQUIREMENT: Generation of random events]
        // Reshuffles the database when the player runs out of unique questions.
        random_device rd;
        mt19937 g(rd());
        shuffle(siteDatabase.begin(), siteDatabase.end(), g);
        
        currentSiteIndex = 0; 
    }

    HeritageSite targetSite = siteDatabase[currentSiteIndex];
    currentSiteIndex++; // Move down the deck for the next round

    string hiddenAnswer = targetSite.answer;
    string guessedState = "";

    // Generate the hidden state representation (underscores for letters, spaces for spaces)
    for (char c : hiddenAnswer) {
        if (c == ' ') {
            guessedState += " ";
        } else {
            guessedState += "_";
        }
    }

    int currentLives = lives;
    int hintsUsed = 0;

    cout << "\n\033[1;33m" << targetSite.movieClue << "\033[0m\n";

    // Main gameplay loop for a single round
    while (currentLives > 0 && guessedState != hiddenAnswer) {
        printFilmReel(currentLives);

        cout << "Target: " << guessedState << "\n";
        cout << "Enter a letter to guess: ";

        // [PROJECT REQUIREMENT: File input/output] -> Standard Input (Console I/O)
        string input;
        char guess;
        while (true) {
            cin >> input;
            // Check if they typed exactly 1 character AND it is an alphabet letter
            if (input.length() == 1 && isalpha(input[0])) {
                guess = toupper(input[0]);
                break; // Valid input, break out of the checking loop
            } else {
                cout << "\033[1;31m [ERROR] Invalid input. Please enter a single letter (A-Z).\033[0m\n";
                cout << "Target: " << guessedState << "\n";
                cout << "Enter a letter to guess: ";
            }
        }

        // Check if the guessed letter exists in the hidden answer
        bool found = false;
        for (size_t i = 0; i < hiddenAnswer.length(); i++) {
            if (hiddenAnswer[i] == guess) {
                guessedState[i] = guess;
                found = true;
            }
        }

        // Handle incorrect guess logic
        if (!found) {
            currentLives--;
            cout << "\a"; // Audible alert
            cout << "\033[1;31m [FRAME BURNED] Incorrect guess.\033[0m\n";

            // Trigger hints if available based on difficulty
            if (hintsUsed < hintsAllowed &&
                static_cast<size_t>(hintsUsed) < targetSite.hints.size()) {
                cout << "\033[1;36m [HINT]: " << targetSite.hints[hintsUsed] << "\033[0m\n";
                hintsUsed++;
            }
        } else {
            cout << "\033[1;32mMatch found!\033[0m\n";
        }
    }

    // Round conclusion messaging
    if (guessedState == hiddenAnswer) {
        cout << "\n\033[1;32m [DECRYPTED] The location is: " << hiddenAnswer << "\033[0m\n";
    } else {
        cout << "\n\033[1;31m [DATA LOST] The film reel burned up. Answer: " << hiddenAnswer << "\033[0m\n";
    }
}

/*
 * Function: run
 * What it does: The main driver for the Celluloid app, loads the database and handles the play-again loop.
 * What the inputs are: None
 * What the outputs are: None
 */
void CelluloidApp::run() {
    cout << "\nLoading The Director's Archive...\n";
    if (!loadFiles()) return; // Exit if the data file cannot be loaded

    char playAgain = 'Y';
    // [PROJECT REQUIREMENT: File input/output] -> Standard I/O loop
    while (playAgain == 'Y' || playAgain == 'y') {
        playRound();
        cout << "\nLoad another reel? (Y/N): ";
        cin >> playAgain;
    }
}
