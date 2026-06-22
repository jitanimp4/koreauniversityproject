// TimeSlipSyndicate.cpp
// The Time-Slip Syndicate - An Architectural Wordle Game
// COMP2113 Project

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cctype>

using namespace std;

enum Difficulty { EASY, MEDIUM, HARD };

// Word banks - ALL WORDS MUST BE EXACTLY 5 LETTERS
const vector<string> EASY_WORDS = {
    "RUINS", "DOMES", "WALLS", "TOWER", "STONE", 
    "BRICK", "GATES", "FORTS", "ABBEY", "MANOR"
};

const vector<string> MEDIUM_WORDS = {
    "TOMBS", "AZTEC", "STOAS", "PYLON", "OBELI",
    "MAYAN", "INCAS", "FORUM", "PIERS", "VAULT"
};

const vector<string> HARD_WORDS = {
    "STUPA", "STELA", "LINTL", "OGIVE", "TRABE",
    "DORIC", "IONIC", "CORIN", "CARYA", "ROSET"
};

class TimeSlipSyndicate {
private:
    string targetWord;
    vector<vector<char>> grid;
    vector<vector<int>> colors; // 0=empty, 1=green, 2=yellow, 3=gray
    int currentRow;
    bool gameWon;
    bool gameActive;
    Difficulty difficulty;
    int timeLimit;
    vector<string> wordBank;
    int timeRemaining;
    time_t startTime;
    
public:
    TimeSlipSyndicate(Difficulty diff) : 
        currentRow(0), 
        gameWon(false), 
        gameActive(true),
        difficulty(diff) {
        
        switch(difficulty) {
            case EASY:
                timeLimit = 30;
                wordBank = EASY_WORDS;
                break;
            case MEDIUM:
                timeLimit = 20;
                wordBank = MEDIUM_WORDS;
                break;
            case HARD:
                timeLimit = 10;
                wordBank = HARD_WORDS;
                break;
        }
        
        srand(time(nullptr));
        targetWord = wordBank[rand() % wordBank.size()];
        
        // Initialize grid (6 rows x 5 columns)
        grid.resize(6, vector<char>(5, ' '));
        colors.resize(6, vector<int>(5, 0));
        
        startTime = time(nullptr);
        timeRemaining = timeLimit;
    }
    
    void displayInstructions() {
        cout << "\n========================================\n";
        cout << "   TIME-SLIP SYNDICATE - INSTRUCTIONS\n";
        cout << "========================================\n\n";
        
        cout << "OBJECTIVE: Guess the 5-letter historical/architectural word in 6 tries!\n\n";
        
        cout << "COLOR MEANING:\n";
        cout << "  GREEN  - Letter is CORRECT and in RIGHT position\n";
        cout << "  YELLOW - Letter is CORRECT but in WRONG position\n";
        cout << "  GRAY   - Letter is NOT in the word\n\n";
        
        cout << "CONTROLS:\n";
        cout << "  • Type a 5-letter word\n";
        cout << "  • Press ENTER to submit your guess\n";
        cout << "  • Type 'exit' or 'quit' to exit the game\n\n";
        
        cout << "DIFFICULTY LEVELS:\n";
        cout << "  • EASY   - 30 seconds per guess, hints available\n";
        cout << "  • MEDIUM - 20 seconds per guess, standard challenge\n";
        cout << "  • HARD   - 10 seconds per guess, no hints\n\n";
        
        cout << "EXAMPLE:\n";
        cout << "  If target word is 'RUINS' and you guess 'HOUSE':\n";
        cout << "  H O U S E\n";
        cout << "  GRAY GRAY YELLOW YELLOW GRAY\n";
        cout << "  This means U and S are in the word, but in wrong positions!\n\n";
        
        cout << "TIP: Start with words containing common vowels (A, E, I, O, U)\n";
        cout << "\n========================================\n";
    }
    
    void updateTimer() {
        time_t now = time(nullptr);
        int elapsed = difftime(now, startTime);
        timeRemaining = timeLimit - elapsed;
        if (timeRemaining < 0) timeRemaining = 0;
    }
    
    void drawGameUI() {
        cout << "\n========== THE TIME-SLIP SYNDICATE ==========\n";
        
        string diffStr;
        switch(difficulty) {
            case EASY: diffStr = "EASY"; break;
            case MEDIUM: diffStr = "MEDIUM"; break;
            case HARD: diffStr = "HARD"; break;
        }
        
        updateTimer();
        cout << "\n[" << diffStr << " MODE]  [TIME: " << timeRemaining << "s]  [ATTEMPT: " << currentRow + 1 << "/6]\n";
        cout << "Type 'exit' to quit\n";
        
        cout << "\n---------------------------------------------\n\n";
        
        // Draw the grid
        for (int row = 0; row < 6; ++row) {
            cout << "  ";
            for (int col = 0; col < 5; ++col) {
                char displayChar = (grid[row][col] == ' ') ? '_' : grid[row][col];
                
                // Display with indicators
                if (colors[row][col] == 1) {
                    cout << "[" << displayChar << "] ";
                } else if (colors[row][col] == 2) {
                    cout << "(" << displayChar << ") ";
                } else if (colors[row][col] == 3) {
                    cout << "{" << displayChar << "} ";
                } else {
                    cout << " " << displayChar << "  ";
                }
            }
            cout << "\n";
        }
        
        cout << "\n---------------------------------------------\n";
        cout << "\nEnter your 5-letter guess: ";
    }
    
    void showResult(const string& guess) {
        cout << "\nResult: ";
        for (int i = 0; i < 5; ++i) {
            if (guess[i] == targetWord[i]) {
                cout << "GREEN ";
            } else {
                bool found = false;
                for (int j = 0; j < 5; ++j) {
                    if (guess[i] == targetWord[j] && guess[j] != targetWord[j]) {
                        found = true;
                        break;
                    }
                }
                if (found) {
                    cout << "YELLOW ";
                } else {
                    cout << "GRAY ";
                }
            }
        }
        cout << "\n\n";
    }
    
    bool evaluateGuess(const string& guess) {
        if (guess.length() != 5) {
            cout << "\nERROR: Need exactly 5 letters!\n";
            return false;
        }
        
        // Check if all characters are letters
        for (char c : guess) {
            if (!isalpha(c)) {
                cout << "\nERROR: Only letters A-Z are allowed!\n";
                return false;
            }
        }
        
        string upperGuess = guess;
        for (char& c : upperGuess) {
            c = toupper(c);
        }
        
        // Store the guess in grid
        for (int i = 0; i < 5; ++i) {
            grid[currentRow][i] = upperGuess[i];
        }
        
        showResult(upperGuess);
        
        // Check win
        if (upperGuess == targetWord) {
            for (int i = 0; i < 5; ++i) {
                colors[currentRow][i] = 1; // Green
            }
            gameWon = true;
            gameActive = false;
            return true;
        }
        
        // Count letters in target
        map<char, int> targetCount;
        for (char c : targetWord) {
            targetCount[c]++;
        }
        
        // Mark correct positions first
        for (int i = 0; i < 5; ++i) {
            if (upperGuess[i] == targetWord[i]) {
                colors[currentRow][i] = 1; // Green
                targetCount[upperGuess[i]]--;
            }
        }
        
        // Mark misplaced letters
        for (int i = 0; i < 5; ++i) {
            if (colors[currentRow][i] != 1) {
                if (targetCount[upperGuess[i]] > 0) {
                    colors[currentRow][i] = 2; // Yellow
                    targetCount[upperGuess[i]]--;
                } else {
                    colors[currentRow][i] = 3; // Gray
                }
            }
        }
        
        return false;
    }
    
    void showHint() {
        if (difficulty == EASY && currentRow == 3 && !gameWon) {
            cout << "\n*** HINT: The word contains letters: ";
            for (char c : targetWord) {
                cout << c << " ";
            }
            cout << "***\n";
        }
    }
    
    void run() {
        displayInstructions();
        cout << "\nPress ENTER to start the game...";
        cin.get();
        
        while (gameActive && !gameWon && currentRow < 6) {
            drawGameUI();
            showHint();
            
            // Check timer
            updateTimer();
            if (timeRemaining <= 0) {
                cout << "\n*** TIME PARADOX! Guess expired! ***\n";
                currentRow++;
                if (currentRow < 6) {
                    startTime = time(nullptr);
                    timeRemaining = timeLimit;
                } else {
                    gameActive = false;
                    break;
                }
                continue;
            }
            
            // Get guess
            string guess;
            cin >> guess;
            
            // Check for exit
            if (guess == "exit" || guess == "quit" || guess == "EXIT" || guess == "QUIT") {
                cout << "\nExiting game...\n";
                gameActive = false;
                break;
            }
            
            if (evaluateGuess(guess)) {
                break;
            }
            
            currentRow++;
            startTime = time(nullptr);
            timeRemaining = timeLimit;
            
            if (currentRow >= 6) {
                gameActive = false;
                break;
            }
        }
        
        // Game over screen
        cout << "\n=============================================\n";
        if (gameWon) {
            cout << "  ★ TEMPORAL ARCHIVE COMPLETE! ★\n";
            cout << "  You successfully restored the timeline!\n";
        } else if (!gameActive && !gameWon) {
            cout << "  ✗ TEMPORAL ARCHIVE FAILED ✗\n";
            cout << "  The historical fragment was: " << targetWord << "\n";
        } else {
            cout << "  Game exited.\n";
        }
        cout << "=============================================\n";
        
        cout << "\nPress ENTER to continue...";
        cin.ignore();
        cin.get();
    }
};

int runWordleApp() {
    while (true) {
        // Clear screen (simple way)
        for (int i = 0; i < 50; i++) cout << endl;
        
        cout << "=============================================\n";
        cout << "      THE TIME-SLIP SYNDICATE\n";
        cout << "      Architectural Wordle Game\n";
        cout << "=============================================\n";
        
        cout << "\n          MAIN MENU\n";
        cout << "\n    1. EASY     - 30s per guess, with hints";
        cout << "\n    2. MEDIUM   - 20s per guess, standard challenge";
        cout << "\n    3. HARD     - 10s per guess, expert mode";
        cout << "\n    4. INSTRUCTIONS";
        cout << "\n    5. EXIT     - Quit the game\n";
        
        cout << "\n    Enter your choice (1-5): ";
        
        int choice;
        cin >> choice;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        
        if (choice == 5) {
            cout << "\nThank you for playing The Time-Slip Syndicate!\n";
            return 0;
        }
        
        if (choice == 4) {
            TimeSlipSyndicate temp(EASY);
            temp.displayInstructions();
            cout << "\nPress ENTER to return to menu...";
            cin.ignore();
            cin.get();
            continue;
        }
        
        Difficulty diff;
        switch(choice) {
            case 1: diff = EASY; break;
            case 2: diff = MEDIUM; break;
            case 3: diff = HARD; break;
            default: 
                cout << "\nInvalid choice! Please enter 1-5.\n";
                cout << "Press ENTER to continue...";
                cin.ignore();
                cin.get();
                continue;
        }
        
        TimeSlipSyndicate game(diff);
        game.run();
    }
    
    return 0;
}
