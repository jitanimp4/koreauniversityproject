#include "art.h"
#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h>
void sleepMs(int ms) {
    Sleep(ms);
}
void clearScreen() {
    system("cls");
}
#else
#include <unistd.h>
void sleepMs(int ms) {
    usleep(ms * 1000);
}
void clearScreen() {
    system("clear");
}
#endif

namespace Color {
    const std::string RESET = "\033[0m";
    const std::string RED = "\033[31m";
    const std::string GREEN = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string BLUE = "\033[34m";
    const std::string MAGENTA = "\033[35m";
    const std::string CYAN = "\033[36m";
    const std::string BOLD = "\033[1m";
}

void printTitleScreen() {
    std::cout << Color::CYAN << Color::BOLD;
    std::cout << "\n";
    std::cout << "============================================\n";
    std::cout << "         CROSSWORD PUZZLE GAME              \n";
    std::cout << "       Of The Ancient Scrolls               \n";
    std::cout << "============================================\n";
    std::cout << Color::YELLOW;
    std::cout << "\n        === A MYSTERIOUS PUZZLE AWAITS ===\n";
    std::cout << Color::RESET;
    std::cout << "\n";
}

void printProgressBar(int solved, int total, int width) {
    if (total == 0) return;
    int percent = (solved * 100) / total;
    int filled = (percent * width) / 100;
    
    std::cout << Color::CYAN << "[";
    for (int i = 0; i < width; i++) {
        if (i < filled) std::cout << "=";
        else std::cout << " ";
    }
    std::cout << "] " << percent << "% (" << solved << "/" << total << " clues)" << Color::RESET << "\n";
}

void celebrate() {
    std::cout << "\n";
    sleepMs(200);
    std::cout << Color::YELLOW << "  ✨ " << Color::MAGENTA << "★" 
              << Color::YELLOW << " ✨ " << Color::GREEN << "☆" 
              << Color::YELLOW << " ✨  " << Color::RESET << "\n";
    sleepMs(200);
    std::cout << Color::GREEN << Color::BOLD;
    std::cout << "\n  >>> CONGRATULATIONS! THE RELIC VAULT OPENS! <<<\n";
    std::cout << Color::RESET;
}

void printSeparator() {
    std::cout << Color::BLUE;
    std::cout << "\n----------------------------------------\n";
    std::cout << Color::RESET;
}
