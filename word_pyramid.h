#ifndef WORD_PYRAMID_H
#define WORD_PYRAMID_H

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <random>
#include <cmath>

using namespace std;

struct PuzzleState {
  vector<string> targetWords;
  vector<bool> correctlyGuessed;
  vector<string> guessedWords;

  vector<char> originalRealLetters;
  vector<char> currentRealLetters;

  int dummyCount;
  int triangleRows;
  int hintsRemaining;
};

class WordPyramidGame {
public:
  WordPyramidGame();
  ~WordPyramidGame();

  bool initialize(int difficultyChoice);
  bool runGameLoop();
  void clearPuzzle();

private:
  vector<vector<string> > easyPuzzles;
  vector<vector<string> > mediumPuzzles;
  vector<vector<string> > hardPuzzles;
  PuzzleState* activePuzzle;
  int currentDifficulty;
  mt19937 rng;

  bool loadWordBank(const string& fileName, vector<string>& words) const;
  void buildPuzzleSets();
  vector<vector<string> > generatePuzzlesFromWordBank(
      const vector<string>& words, int desiredPuzzles);

  bool validatePuzzleConstraints(const vector<string>& words) const;
  vector<char> buildLetterPool(const vector<string>& words) const;
  int computeTriangleRows(int letterCount) const;

  void displayGameState() const;
  void printPyramid() const;
  void printGuessedWords() const;
  int countWordsLeft() const;

  string normalizeInput(const string& input) const;
  bool canConstructWord(const string& word) const;
  bool isAlphabeticWord(const string& word) const;

  void processGuess(const string& word);
  void removeWordLettersFromPool(const string& word);
  int findTargetWordIndex(const string& word) const;
  bool allTargetWordsGuessed() const;

  void processHint();
  void logGameResult(bool won) const;
};

void runWordPyramidGame();

#endif
