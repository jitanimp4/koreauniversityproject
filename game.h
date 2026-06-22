#ifndef GAME_H
#define GAME_H

#include "puzzle.h"

#include <chrono>
#include <memory>
#include <string>

struct SaveMeta {
    int difficultyChoice = 1;
    int elapsedSeconds = 0;
    bool hardBellPlayed = false;
};

class CrosswordGame {
public:

    // Constructor - initializes the crossword game
    // savePath file path for saving/loading game progress
    explicit CrosswordGame(const std::string &savePath);

    // Main game loop - runs the crossword experience
    void run();

private:

    // Attempts to load a previously saved game
    // return true if save file exists and user wants to load it
    bool tryLoadGame();

    // Starts a brand new game with user-selected difficulty
    // return true if new game started successfully
    bool startNewGame();

    // Main gameplay loop (input and move processing)
    void gameLoop();

    // Handles the 'solve' command - lets user guess a word
    void processSolve();

    // Handles the 'hint' command - reveals random letter
    void processHint();

    // Applies end-game bonuses/penalties (time bonus, hint penalties)
    void applyFinalScoring();

    // Calculates seconds left before time runs out
    // return seconds remaining, or large number if no time limit
    int secondsRemaining() const;

    // Checks if all crossword clues have been solved
    // return true if no words left in remainingWords set
    bool isSolved() const;

    // Finds a clue by its number and direction
    // number clue number (e.g., 1, 2, 3...)
    // d Across or Down direction
    // return pointer to Clue if found, nullptr otherwise
    const Clue *findClue(int number, Direction d) const;

    // Picks a random unsolved clue and reveals one random letter
    // return true if a hint was revealed, false if all solved
    bool revealOneLetter();

    // saves current game to file
    void saveCurrentGame() const;


    std::string savePath_;
    int difficultyChoice_;
    DifficultyConfig cfg_;
    std::unique_ptr<GameState> state_;
    std::chrono::steady_clock::time_point startTime_;
    int loadedElapsedSeconds_;
    bool hardBellPlayed_;
};

#endif
