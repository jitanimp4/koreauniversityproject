#ifndef ART_H
#define ART_H

#include <iostream>
#include <string>

// Function declarations only (no implementations)
void sleepMs(int ms);
void clearScreen();

namespace Color {
    extern const std::string RESET;
    extern const std::string RED;
    extern const std::string GREEN;
    extern const std::string YELLOW;
    extern const std::string BLUE;
    extern const std::string MAGENTA;
    extern const std::string CYAN;
    extern const std::string BOLD;
}

void printTitleScreen();
void printProgressBar(int solved, int total, int width = 25);
void celebrate();
void printSeparator();

#endif
