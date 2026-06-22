#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "celluloid_app.h"

using namespace std;

/**
 * Displays the main boot sequence for Odyssey OS.
 * Inputs: None
 * Outputs: Prints ASCII art and system messages to the console.
 */
void bootSequence() {
    cout << "\033[1;32m"; // Set text to bold green
    cout << "==========================================\n";
    cout << "        ODYSSEY OS: TERMINAL v2.4       \n";
    cout << "==========================================\n";
    cout << "[SYSTEM] Booting secure database...\n";
    cout << "[SYSTEM] Decryption protocols online.\n";
    cout << "\033[0m"; // Reset formatting
}

/**
 * Main entry point. Handles the menu loop and dynamic memory for apps.
 */
int main() {
    srand(time(0)); // Seed random number generator
    bootSequence();

    int difficulty = 2; // Default to Medium

    cout << "\nSelect Profile:\n1. Intern (Easy)\n2. Archivist (Medium)\n3. Master Curator (Hard)\n> ";
    cin >> difficulty;

    bool systemRunning = true;

    while (systemRunning) {
        cout << "\n\033[1;36m--- ODYSSEY OS MAIN MENU ---\033[0m\n";
        cout << "1. The Director's Archive (Celluloid Citadel)\n";
        cout << "2. Exit System\n";
        cout << "Select App > ";

        int choice;
        cin >> choice;

        if (choice == 1) {
            // DYNAMIC MEMORY REQUIREMENT: Allocate app on the heap
            CelluloidApp* app2 = new CelluloidApp(difficulty);
            app2->run();
            
            // DYNAMIC MEMORY REQUIREMENT: Free memory when app closes
            delete app2;
        } else if (choice == 2) {
            cout << "Logging off...\n";
            systemRunning = false;
        } else {
            cout << "\033[1;31m [ERROR] Invalid command.\033[0m\n"; // Red error text
        }
    }
    return 0;
}
