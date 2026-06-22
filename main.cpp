#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <limits>

#include "cryptex_boss.h"
#include "celluloid_app.h"
#include "word_pyramid.h"
#include "game.h"
#include "forgers_table.h"

using namespace std;

// Color definitions
const string RESET   = "\033[0m";
const string RED     = "\033[31m";
const string GREEN   = "\033[32m";
const string YELLOW  = "\033[33m";
const string BLUE    = "\033[34m";
const string MAGENTA = "\033[35m";
const string CYAN    = "\033[36m";
const string WHITE   = "\033[37m";
const string BOLD    = "\033[1m";
const string DIM     = "\033[2m";

// Forward declaration
int runWordleApp();

string toLowerMain(string text)
{
    for (size_t i = 0; i < text.length(); i++)
        text[i] = tolower((unsigned char)text[i]);
    return text;
}

bool isCryptexUnlocked(bool game1Done, bool game2Done, bool game3Done, bool game4Done, bool game5Done)
{
    return game1Done && game2Done && game3Done && game4Done && game5Done;
}

void pauseMenu()
{
    cout << "\n" << DIM << "Press ENTER to return to menu..." << RESET;
    string temp;
    getline(cin, temp);
}

void printBanner()
{
    cout << CYAN << BOLD;
    cout << R"(
   ╔═══════════════════════════════════════════════════════════════╗
   ║                                                               ║
   ║                                                               ║
   ║                                                               ║
   ║                      O D Y S S E Y   O S                      ║
   ║                                                               ║
   ║                                                               ║
   ║                                                               ║
   ╚═══════════════════════════════════════════════════════════════╝
   )" << '\n';
    cout << RESET;
}

void printStatusBar(int completed, int total)
{
    int percent = (completed * 100) / total;
    int barWidth = 30;
    int filled = (percent * barWidth) / 100;
    
    cout << "\n   " << CYAN << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << RESET << endl;
    cout << "   " << BOLD << "PROGRESS: " << RESET;
    cout << CYAN << "[";
    for (int i = 0; i < barWidth; i++)
    {
        if (i < filled)
            cout << "█";
        else
            cout << "░";
    }
    cout << "] " << percent << "% (" << completed << "/" << total << " apps completed)" << RESET << endl;
    cout << "   " << CYAN << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << RESET << endl;
}

int main()
{
    srand(time(0));
    
    bool wordPyramidDone = false;
    bool directorsArchiveDone = false;
    bool timeSlipDone = false;
    bool crosswordDone = false;
    bool forgersTableDone = false;
    
    while (true)
    {
        // Clear screen for clean look
        cout << "\033[2J\033[1;1H";
        
        printBanner();
        
        int completedCount = 0;
        if (wordPyramidDone) completedCount++;
        if (directorsArchiveDone) completedCount++;
        if (timeSlipDone) completedCount++;
        if (crosswordDone) completedCount++;
        if (forgersTableDone) completedCount++;
        
        printStatusBar(completedCount, 5);
        
        cout << "\n";
        cout << "   " << BOLD << "┌─────────────────────────────────────────────────────┐" << RESET << endl;
        cout << "   " << BOLD << "│                    MAIN MENU                         " << RESET << endl;
        cout << "   " << BOLD << "├─────────────────────────────────────────────────────┤" << RESET << endl;
        cout << "   │" << endl;
        
        // App 1
        cout << "   │   " << CYAN << "[1]" << RESET << " Word Pyramids of Giza";
        if (wordPyramidDone) cout << " " << GREEN << "✓ COMPLETED" << RESET;
        cout << endl;
        
        // App 2
        cout << "   │   " << CYAN << "[2]" << RESET << " The Director's Archive";
        if (directorsArchiveDone) cout << " " << GREEN << "✓ COMPLETED" << RESET;
        cout << endl;
        
        // App 3
        cout << "   │   " << CYAN << "[3]" << RESET << " The Time-Slip Syndicate";
        if (timeSlipDone) cout << " " << GREEN << "✓ COMPLETED" << RESET;
        cout << endl;
        
        // App 4
        cout << "   │   " << CYAN << "[4]" << RESET << " Crossword of Ancient Scrolls";
        if (crosswordDone) cout << " " << GREEN << "✓ COMPLETED" << RESET;
        cout << endl;
        
        // App 5
        cout << "   │   " << CYAN << "[5]" << RESET << " The Forger's Table";
        if (forgersTableDone) cout << " " << GREEN << "✓ COMPLETED" << RESET;
        cout << endl;
        
        cout << "   │" << endl;
        cout << "   " << BOLD << "├─────────────────────────────────────────────────────┤" << RESET << endl;
        
        // Cryptex (App 6)
        if (isCryptexUnlocked(wordPyramidDone, directorsArchiveDone, timeSlipDone, crosswordDone, forgersTableDone))
        {
            cout << "   │   " << MAGENTA << BOLD << "[6]" << RESET << MAGENTA << " ★ The Curator's Cryptex ★" << RESET;
            cout << " " << GREEN << "UNLOCKED!" << RESET << endl;
        }
        else
        {
            cout << "   │   " << DIM << "[6] The Curator's Cryptex (LOCKED)" << RESET << endl;
        }
        
        cout << "   │" << endl;
        cout << "   │   " << RED << "[0]" << RESET << " Exit Odyssey OS" << endl;
        cout << "   │" << endl;
        cout << "   " << BOLD << "└─────────────────────────────────────────────────────┘" << RESET << endl;
        
        cout << "\n   " << BOLD << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << RESET << endl;
        cout << "   " << YELLOW << "⚡ Select an option (0-6): " << RESET;
        
        int choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        cout << "\n";
        
        if (choice == 0)
        {
            cout << "   " << RED << "👋 Logging off Odyssey OS..." << RESET << endl;
            cout << "   " << CYAN << "Thank you for using the Odyssey App!" << RESET << endl;
            break;
        }
        else if (choice == 1)
        {
            cout << "   " << CYAN << "🎮 Launching Word Pyramids of Giza..." << RESET << endl;
            runWordPyramidGame();
            wordPyramidDone = true;
            pauseMenu();
        }
        else if (choice == 2)
        {
            cout << "   " << CYAN << "🎮 Launching The Director's Archive..." << RESET << endl;
            int difficulty;
            cout << "   " << YELLOW << "Select difficulty: 1 Easy, 2 Medium, 3 Hard: " << RESET;
            cin >> difficulty;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            CelluloidApp app(difficulty);
            app.run();
            directorsArchiveDone = true;
            pauseMenu();
        }
        else if (choice == 3)
        {
            cout << "   " << CYAN << "🎮 Launching The Time-Slip Syndicate..." << RESET << endl;
            runWordleApp();
            timeSlipDone = true;
            pauseMenu();
        }
        else if (choice == 4)
        {
            cout << "   " << CYAN << "🎮 Launching Crossword of Ancient Scrolls..." << RESET << endl;
            CrosswordGame game("save_data.txt");
            game.run();
            crosswordDone = true;
            pauseMenu();
        }
        else if (choice == 5)
        {
            cout << "   " << CYAN << "🕵️ Launching The Forger's Table..." << RESET << endl;
            runForgersTable();
            forgersTableDone = true;
            pauseMenu();
        }
        else if (choice == 6)
        {
            if (!isCryptexUnlocked(wordPyramidDone, directorsArchiveDone, timeSlipDone, crosswordDone, forgersTableDone))
            {
                cout << "   " << RED << "🔒 CRYPTEX LOCKED!" << RESET << endl;
                cout << "   " << YELLOW << "   Complete all 5 apps to unlock the Cryptex:" << RESET << endl;
                if (!wordPyramidDone) cout << "      ❌ Word Pyramids of Giza" << endl;
                if (!directorsArchiveDone) cout << "      ❌ The Director's Archive" << endl;
                if (!timeSlipDone) cout << "      ❌ The Time-Slip Syndicate" << endl;
                if (!crosswordDone) cout << "      ❌ Crossword of Ancient Scrolls" << endl;
                if (!forgersTableDone) cout << "      ❌ The Forger's Table" << endl;
                pauseMenu();
                continue;
            }
            
            cout << "   " << MAGENTA << "🔓 Accessing The Curator's Cryptex..." << RESET << endl;
            string mode;
            cout << "   " << YELLOW << "Select Cryptex mode (easy / medium / hard): " << RESET;
            getline(cin, mode);
            
            runCryptexBoss(toLowerMain(mode));
            pauseMenu();
        }
        else
        {
            cout << "   " << RED << "❌ Invalid choice. Please enter 0-6." << RESET << endl;
            pauseMenu();
        }
    }
    
    return 0;
}
