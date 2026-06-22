#ifndef PUZZLE_H
#define PUZZLE_H

#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

enum class Direction {
    Across,
    Down
};

//rep single puzzle with its position.
struct Clue {
    int number;
    Direction direction;
    std::string answer;
    std::string clueText;
    int row;
    int col;
};

//config settings for each diff level
struct DifficultyConfig {

    std::string name;               // "Easy", "Medium", "Hard"
    std::string theme;              // Theme description
    int gridSize;                   // N for NxN grid
    int timeLimitSec;               // Time limit in seconds (0 = none)
    int maxHints;                   // Max hints allowed (0 = unlimited)
    int wordBankSize;               // Target number of words
    int basePointsPerWord;          // Points for correct guess
    int wrongGuessPenalty;          // Points deducted for wrong guess
    bool unlimitedHints;            // True if hints never run out
    bool hintPenaltyPercent;        // True = penalty is percentage
    int hintPenaltyValue;           // Penalty amount (points or percent)
    int finalMinuteBell;            // 1 = play sound at 60 seconds
    bool streakBonus;               // True if combo bonus enabled
    int streakThreshold;            // Words needed for bonus
    int streakBonusValue;           // Bonus points for streak
    int finishTimeBonusFactor;      // Points per second remaining
};

//all current data
struct GameState {
    std::vector<std::string> solutionGrid;
    std::vector<std::vector<int>> numbering;
    std::vector<Clue> clues;
    std::vector<std::string> wordBank;
    std::set<std::string> remainingWords;
    std::map<std::pair<int, Direction>, std::string> solvedByKey;
    int score = 0;
    int hintsUsed = 0;
    int currentStreak = 0;
};

//raw word and clue pair before processing 
struct RawItem {
    std::string answer;
    std::string clue;
};

// Returns difficulty configuration based on user choice
// return DifficultyConfig struct with all game settings
DifficultyConfig getConfig(int choice);

// Returns word list for selected difficulty level
// return vector of RawItem (answer + clue pairs)
std::vector<RawItem> getRawByDifficulty(int choice);

//build crossword grid by placing words.
bool buildCrossword(
    int n, const std::vector<RawItem> &items, std::vector<std::string> &solutionGrid,
    std::vector<Clue> &placedClues);
std::vector<std::vector<int>> buildNumbering(const std::vector<std::string> &grid);

#endif
