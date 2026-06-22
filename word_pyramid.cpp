#include "word_pyramid.h"

using namespace std;

WordPyramidGame::WordPyramidGame() : activePuzzle(NULL), currentDifficulty(0) {
  std::random_device rd;
  rng.seed(rd());
}


WordPyramidGame::~WordPyramidGame() {
  if (activePuzzle != NULL) {
    delete activePuzzle;
    activePuzzle = NULL;
  }
}

bool WordPyramidGame::loadWordBank(const std::string& fileName, std::vector<std::string>& words) const {
  std::ifstream inFile(fileName.c_str());
  if (!inFile.is_open()) {
    return false;
  }

  words.clear();
  std::string token;
  while (inFile >> token) {
    std::string cleaned;
    for (size_t i = 0; i < token.size(); ++i) {
      if (std::isalpha(static_cast<unsigned char>(token[i]))) {
        cleaned.push_back(static_cast<char>(std::toupper(static_cast<unsigned char>(token[i]))));
      }
    }
    if (!cleaned.empty()) {
      words.push_back(cleaned);
    }
  }
  return !words.empty();
}

//  Checks whether a puzzle candidate satisfies assignment constraints.
//  Input parameters: words: candidate puzzle word list.
//  Return value: true if words count in [4,7] and total chars in [40,60], else false.

bool WordPyramidGame::validatePuzzleConstraints(const std::vector<std::string>& words) const {
  if (words.size() < 4 || words.size() > 6) {
    return false;
  }

  int totalChars = 0;
  for (size_t i = 0; i < words.size(); ++i) {
    totalChars += static_cast<int>(words[i].size());
  }
  return totalChars >= 25 && totalChars <= 55;
}

//  Generates random puzzle sets from a loaded word bank while satisfying constraints.
//  Input parameters: words: source word bank for one difficulty. desiredPuzzles: number of valid puzzles to attempt to produce.
//  Return value:vector of puzzle word sets.

std::vector<std::vector<std::string> > WordPyramidGame::generatePuzzlesFromWordBank(
    const std::vector<std::string>& words, int desiredPuzzles) {
  std::vector<std::vector<std::string> > puzzles;
  if (words.size() < 4) {
    return puzzles;
  }

  std::uniform_int_distribution<int> sizeDist(4, 7);
  int attempts = 0;
  const int maxAttempts = 2000;

  while (static_cast<int>(puzzles.size()) < desiredPuzzles && attempts < maxAttempts) {
    ++attempts;
    int selectedSize = sizeDist(rng);
    if (selectedSize > static_cast<int>(words.size())) {
      selectedSize = static_cast<int>(words.size());
    }

    std::vector<int> indices(words.size(), 0);
    for (int i = 0; i < selectedSize; ++i) {
      indices[i] = 1;
    }
    std::shuffle(indices.begin(), indices.end(), rng);

    std::vector<std::string> candidate;
    for (size_t i = 0; i < indices.size(); ++i) {
      if (indices[i] == 1) {
        candidate.push_back(words[i]);
      }
    }

    if (!validatePuzzleConstraints(candidate)) {
      continue;
    }

    std::vector<std::string> sortedCandidate = candidate;
    std::sort(sortedCandidate.begin(), sortedCandidate.end());
    bool duplicate = false;
    for (size_t i = 0; i < puzzles.size(); ++i) {
      std::vector<std::string> existing = puzzles[i];
      std::sort(existing.begin(), existing.end());
      if (existing == sortedCandidate) {
        duplicate = true;
        break;
      }
    }
    if (!duplicate) {
      puzzles.push_back(candidate);
    }
  }

  return puzzles;
}


 // Creates fallback built-in puzzle sets and optionally replaces them from file word banks.
 // Input parameters: none.
 // Return value: none.
 
void WordPyramidGame::buildPuzzleSets() {
  easyPuzzles.clear();
  mediumPuzzles.clear();
  hardPuzzles.clear();

  easyPuzzles.push_back(std::vector<std::string>());
  easyPuzzles.back().push_back("ANUBIS");
  easyPuzzles.back().push_back("OSIRIS");
  easyPuzzles.back().push_back("PHARAOH");
  easyPuzzles.back().push_back("SARCOPHAGUS");
  easyPuzzles.back().push_back("UNDERWORLD");

  easyPuzzles.push_back(std::vector<std::string>());
  easyPuzzles.back().push_back("BASTET");
  easyPuzzles.back().push_back("HORUS");
  easyPuzzles.back().push_back("NEPHTHYS");
  easyPuzzles.back().push_back("PYRAMIDS");
  easyPuzzles.back().push_back("HIEROGLYPHS");

  easyPuzzles.push_back(std::vector<std::string>());
  easyPuzzles.back().push_back("SEKHMET");
  easyPuzzles.back().push_back("AMMIT");
  easyPuzzles.back().push_back("THOTH");
  easyPuzzles.back().push_back("AFTERLIFE");
  easyPuzzles.back().push_back("SCARAB");
  easyPuzzles.back().push_back("EMBALMING");

  mediumPuzzles.push_back(std::vector<std::string>());
  mediumPuzzles.back().push_back("ALEXANDRIA");
  mediumPuzzles.back().push_back("CARTHAGE");
  mediumPuzzles.back().push_back("OBELISK");
  mediumPuzzles.back().push_back("COLOSSEUM");
  mediumPuzzles.back().push_back("AQUEDUCT");

  mediumPuzzles.push_back(std::vector<std::string>());
  mediumPuzzles.back().push_back("BYZANTINE");
  mediumPuzzles.back().push_back("PARTHENON");
  mediumPuzzles.back().push_back("MESOPOTAMIA");
  mediumPuzzles.back().push_back("CITADEL");
  mediumPuzzles.back().push_back("EMPIRE");

  mediumPuzzles.push_back(std::vector<std::string>());
  mediumPuzzles.back().push_back("PERSIA");
  mediumPuzzles.back().push_back("BABYLON");
  mediumPuzzles.back().push_back("ACROPOLIS");
  mediumPuzzles.back().push_back("LEGIONARY");
  mediumPuzzles.back().push_back("MONOLITH");
  mediumPuzzles.back().push_back("TRIBUTE");

  hardPuzzles.push_back(std::vector<std::string>());
  hardPuzzles.back().push_back("ALEXANDER");
  hardPuzzles.back().push_back("NEFERTITI");
  hardPuzzles.back().push_back("GILGAMESH");
  hardPuzzles.back().push_back("SPARTACUS");
  hardPuzzles.back().push_back("CLEOPATRA");

  hardPuzzles.push_back(std::vector<std::string>());
  hardPuzzles.back().push_back("LEONIDAS");
  hardPuzzles.back().push_back("RAMSES");
  hardPuzzles.back().push_back("ODYSSEUS");
  hardPuzzles.back().push_back("HANNIBAL");
  hardPuzzles.back().push_back("THEMISTOCLES");
  hardPuzzles.back().push_back("PERSEUS");

  hardPuzzles.push_back(std::vector<std::string>());
  hardPuzzles.back().push_back("JULIUS");
  hardPuzzles.back().push_back("CAESAR");
  hardPuzzles.back().push_back("BELLEROPHON");
  hardPuzzles.back().push_back("ACHILLES");
  hardPuzzles.back().push_back("XERXES");
  hardPuzzles.back().push_back("AGAMEMNON");

  std::vector<std::string> easyBank;
  std::vector<std::string> mediumBank;
  std::vector<std::string> hardBank;

  bool easyLoaded = loadWordBank("easy_words.txt", easyBank);
  bool mediumLoaded = loadWordBank("medium_words.txt", mediumBank);
  bool hardLoaded = loadWordBank("hard_words.txt", hardBank);

  if (easyLoaded) {
    std::vector<std::vector<std::string> > generated = generatePuzzlesFromWordBank(easyBank, 3);
    if (generated.size() >= 3) {
      easyPuzzles = generated;
    }
  }
  if (mediumLoaded) {
    std::vector<std::vector<std::string> > generated = generatePuzzlesFromWordBank(mediumBank, 3);
    if (generated.size() >= 3) {
      mediumPuzzles = generated;
    }
  }
  if (hardLoaded) {
    std::vector<std::vector<std::string> > generated = generatePuzzlesFromWordBank(hardBank, 3);
    if (generated.size() >= 3) {
      hardPuzzles = generated;
    }
  }
}

// Builds a flattened real-letter pool from target words.
//  Input parameters: words: list of target words.
//  Return value: vector of all letters in row-major source order before shuffling.

std::vector<char> WordPyramidGame::buildLetterPool(const std::vector<std::string>& words) const {
  std::vector<char> letters;
  for (size_t i = 0; i < words.size(); ++i) {
    for (size_t j = 0; j < words[i].size(); ++j) {
      letters.push_back(words[i][j]);
    }
  }
  return letters;
}

/**
 * Computes the smallest row count n where n(n+1)/2 >= letterCount.
 * Input parameters:
 * - letterCount: number of real letters.
 * Return value:
 * - integer n for triangular display size.
 */
int WordPyramidGame::computeTriangleRows(int letterCount) const {
  int n = 1;
  while ((n * (n + 1)) / 2 < letterCount) {
    ++n;
  }
  return n;
}

// Initializes one round using selected difficulty and random puzzle.
// Input parameters: difficultyChoice: 1 easy, 2 medium, 3 hard.
// Return value: true if a puzzle was successfully prepared, false otherwise.
 
bool WordPyramidGame::initialize(int difficultyChoice) {
  if (activePuzzle != NULL) {
    delete activePuzzle;
    activePuzzle = NULL;
  }
  buildPuzzleSets();

  std::vector<std::vector<std::string> >* selected = NULL;
  if (difficultyChoice == 1) {
    selected = &easyPuzzles;
  } else if (difficultyChoice == 2) {
    selected = &mediumPuzzles;
  } else if (difficultyChoice == 3) {
    selected = &hardPuzzles;
  } else {
    return false;
  }
  currentDifficulty = difficultyChoice;

  if (selected->empty()) {
    return false;
  }

  std::uniform_int_distribution<int> puzzleDist(0, static_cast<int>(selected->size()) - 1);
  int puzzleIndex = puzzleDist(rng);

  activePuzzle = new PuzzleState();
  activePuzzle->targetWords = (*selected)[puzzleIndex];
  activePuzzle->correctlyGuessed.assign(activePuzzle->targetWords.size(), false);
  activePuzzle->guessedWords.clear();
  activePuzzle->hintsRemaining = 3;

  activePuzzle->originalRealLetters = buildLetterPool(activePuzzle->targetWords);
  std::shuffle(activePuzzle->originalRealLetters.begin(), activePuzzle->originalRealLetters.end(), rng);
  activePuzzle->currentRealLetters = activePuzzle->originalRealLetters;

  int realCount = static_cast<int>(activePuzzle->originalRealLetters.size());
  activePuzzle->triangleRows = computeTriangleRows(realCount);
  int triangularCells = (activePuzzle->triangleRows * (activePuzzle->triangleRows + 1)) / 2;
  activePuzzle->dummyCount = triangularCells - realCount;

  return true;
}

// Restores the active puzzle to original shuffled letters and clears progress.
// Input parameters: none.
// Return value: none.

void WordPyramidGame::clearPuzzle() {
  if (activePuzzle == NULL) {
    return;
  }
  activePuzzle->currentRealLetters = activePuzzle->originalRealLetters;
  activePuzzle->guessedWords.clear();
  activePuzzle->correctlyGuessed.assign(activePuzzle->targetWords.size(), false);
  activePuzzle->hintsRemaining = 3;
}

// Converts user text to uppercase and removes spaces.
// Input parameters: input: raw user text.
// Return value:uppercase compacted string.
 
std::string WordPyramidGame::normalizeInput(const std::string& input) const {
  std::string result;
  for (size_t i = 0; i < input.size(); ++i) {
    unsigned char c = static_cast<unsigned char>(input[i]);
    if (!std::isspace(c)) {
      result.push_back(static_cast<char>(std::toupper(c)));
    }
  }
  return result;
}

// Checks whether a word contains only alphabetic characters.
// Input parameters: word: uppercase candidate guess.
// Return value:true if every character is A-Z and word is non-empty.

bool WordPyramidGame::isAlphabeticWord(const std::string& word) const {
  if (word.empty()) {
    return false;
  }
  for (size_t i = 0; i < word.size(); ++i) {
    if (!std::isalpha(static_cast<unsigned char>(word[i]))) {
      return false;
    }
  }
  return true;
}

// Verifies the current real-letter pool can build a given word using frequencies.
// Input parameters:word: uppercase guess.
// Return value: true if word can be formed from current real letters.
 
bool WordPyramidGame::canConstructWord(const std::string& word) const {
  if (activePuzzle == NULL) {
    return false;
  }
  std::vector<char> temp = activePuzzle->currentRealLetters;
  for (size_t i = 0; i < word.size(); ++i) {
    std::vector<char>::iterator it = std::find(temp.begin(), temp.end(), word[i]);
    if (it == temp.end()) {
      return false;
    }
    temp.erase(it);
  }
  return true;
}

// Removes letters of a guessed word from the real-letter pool while preserving order.
// Input parameters: word: uppercase word already validated against pool.
// Return value: none.

void WordPyramidGame::removeWordLettersFromPool(const std::string& word) {
  if (activePuzzle == NULL) {
    return;
  }
  for (size_t i = 0; i < word.size(); ++i) {
    std::vector<char>::iterator it =
        std::find(activePuzzle->currentRealLetters.begin(), activePuzzle->currentRealLetters.end(), word[i]);
    if (it != activePuzzle->currentRealLetters.end()) {
      activePuzzle->currentRealLetters.erase(it);
    }
  }
}

// Finds index of a target word in the active puzzle.
// Input parameters: word: uppercase candidate.
// Return value: non-negative index if found, or -1 if not a target word.

int WordPyramidGame::findTargetWordIndex(const std::string& word) const {
  if (activePuzzle == NULL) {
    return -1;
  }
  for (size_t i = 0; i < activePuzzle->targetWords.size(); ++i) {
    if (activePuzzle->targetWords[i] == word) {
      return static_cast<int>(i);
    }
  }
  return -1;
}

// Applies one validated user guess: consume letters and update guessed/correct state.
// Input parameters: word: uppercase valid input.
// Return value: none.

void WordPyramidGame::processGuess(const std::string& word) {
  removeWordLettersFromPool(word);
  activePuzzle->guessedWords.push_back(word);

  int index = findTargetWordIndex(word);
  if (index >= 0 && !activePuzzle->correctlyGuessed[index]) {
    activePuzzle->correctlyGuessed[index] = true;
    std::cout << "Correct! '" << word << "' is one of the hidden words.\n";
  } else if (index >= 0 && activePuzzle->correctlyGuessed[index]) {
    std::cout << "That target word was already marked correct earlier.\n";
  } else {
    std::cout << "Word accepted, but it is not a target word.\n";
  }
}

// Determines if all target words have already been guessed correctly.
// Input parameters: none.
// Return value:true when every target word flag is true.

bool WordPyramidGame::allTargetWordsGuessed() const {
  if (activePuzzle == NULL) {
    return false;
  }
  for (size_t i = 0; i < activePuzzle->correctlyGuessed.size(); ++i) {
    if (!activePuzzle->correctlyGuessed[i]) {
      return false;
    }
  }
  return true;
}

// Counts how many target words remain unguessed.
// Input parameters: none.
// Return value:number of target words still missing.

int WordPyramidGame::countWordsLeft() const {
  if (activePuzzle == NULL) {
    return 0;
  }
  int left = 0;
  for (size_t i = 0; i < activePuzzle->correctlyGuessed.size(); ++i) {
    if (!activePuzzle->correctlyGuessed[i]) {
      ++left;
    }
  }
  return left;
}

// Uses one hint to auto-solve a random unguessed target word and consume letters.
// Input parameters: none.
// Return value: none.

void WordPyramidGame::processHint() {
  if (activePuzzle->hintsRemaining <= 0) {
    std::cout << "No hints remaining.\n";
    return;
  }

  std::vector<int> remainingIndices;
  for (size_t i = 0; i < activePuzzle->targetWords.size(); ++i) {
    if (!activePuzzle->correctlyGuessed[i]) {
      remainingIndices.push_back(static_cast<int>(i));
    }
  }
  if (remainingIndices.empty()) {
    std::cout << "All target words are already guessed.\n";
    return;
  }

  std::uniform_int_distribution<int> dist(0, static_cast<int>(remainingIndices.size()) - 1);
  int chosen = remainingIndices[dist(rng)];
  std::string hintedWord = activePuzzle->targetWords[chosen];

  if (!canConstructWord(hintedWord)) {
    std::cout << "Hint could not be applied because letters are no longer available.\n";
    return;
  }

  --activePuzzle->hintsRemaining;
  std::cout << "Hint revealed word: " << hintedWord << "\n";
  processGuess(hintedWord);
}

// Prints guessed words list in order of guesses.
// Input parameters: none.
// Return value: none.

void WordPyramidGame::printGuessedWords() const {
  std::cout << "Guessed words: ";
  if (activePuzzle->guessedWords.empty()) {
    std::cout << "(none)\n";
    return;
  }
  for (size_t i = 0; i < activePuzzle->guessedWords.size(); ++i) {
    std::cout << activePuzzle->guessedWords[i];
    if (i + 1 < activePuzzle->guessedWords.size()) {
      std::cout << ", ";
    }
  }
  std::cout << "\n";
}

// Prints current pyramid as a perfect triangle using real letters plus fixed dummy cells.
// Input parameters: none.
// Return value: none.
 
void WordPyramidGame::printPyramid() const {
  std::vector<char> displayLetters = activePuzzle->currentRealLetters;
  for (int i = 0; i < activePuzzle->dummyCount; ++i) {
    displayLetters.push_back('#');
  }

  int cursor = 0;
  int rows = activePuzzle->triangleRows;
  std::cout << "\nLetter Pyramid\n";
  for (int row = 1; row <= rows; ++row) {
    int indent = rows - row;
    for (int s = 0; s < indent; ++s) {
      std::cout << "  ";
    }
    for (int col = 0; col < row; ++col) {
      std::cout << displayLetters[cursor++] << " ";
    }
    std::cout << "\n";
  }
}

// Displays all round UI details before asking for next input.
//  Input parameters: none.
// Return value: none.
 
void WordPyramidGame::displayGameState() const {
  std::cout << "\n--------------------------------------------------\n";
  switch (currentDifficulty) {
    case 1:
      std::cout<<"Category: Gods, Monsters, and Myths (Greek, Roman, Egyptian, Hindu, Nordic)\n";
      break;

    case 2:
      std::cout<<"Category: Empires, Innovation, Monuments\n";
      break;
    
    case 3:
      std::cout<<"Category: Rulers, Pharaohs, and Heroes (Greek, Roman, Egyptian, Nordic)\n";
      break;

    default:
      std::cout<<"Category: Unknown\n";
  }
  std::cout << "Number of words: " << activePuzzle->targetWords.size() << "\n";
  std::cout << "--------------------------------------------------\n";

  printPyramid();
  printGuessedWords();
  std::cout << "Hints remaining: " << activePuzzle->hintsRemaining << "\n";
  std::cout << "Target words left: " << countWordsLeft() << "\n";
  std::cout << "--------------------------------------------------\n";
}

// Appends game outcome to a local log file, demonstrating file output usage.
// Input parameters: won: true if round ended in victory, false otherwise.
// Return value: none.
 
void WordPyramidGame::logGameResult(bool won) const {
  std::ofstream out("game_log.txt", std::ios::app);
  if (!out.is_open() || activePuzzle == NULL) {
    return;
  }
  out << (won ? "WIN" : "EXIT") << " | targets=" << activePuzzle->targetWords.size()
      << " | guessed=" << activePuzzle->guessedWords.size()
      << " | hints_left=" << activePuzzle->hintsRemaining << "\n";
}

// Runs one full puzzle loop until victory or EOF/input termination.
// Input parameters: none.
//  Return value: none.
 
bool WordPyramidGame::runGameLoop() {
  if (activePuzzle == NULL) {
    std::cout << "No active puzzle available.\n";
    return false;
  }

  std::string rawInput;
  while (true) {
    displayGameState();
    std::cout << "Player input: ";
    if (!std::getline(std::cin, rawInput)) {
      std::cout << "\nInput ended. Leaving game.\n";
      logGameResult(false);
      return false;
    }

    std::string input = normalizeInput(rawInput);

    if (input == "CLEAR") {
      clearPuzzle();
      std::cout << "Puzzle reset to original shuffled letters.\n";
      continue;
    }
    if (input == "HINT") {
      processHint();
      if (allTargetWordsGuessed()) {
        std::cout << "Congratulations! You have successfully escaped from the Word Pyramids of Giza. "
                     "Good luck on your journey brave explorer!\n";
        logGameResult(true);
        return false;
      }
      continue;
    }

    if (input == "EXIT"){
      std::cout <<"Returning to main menu.\n";
      return true;
    }
        
    if (!isAlphabeticWord(input) || !canConstructWord(input)) {
      std::cout << "Invalid Input. Please ONLY use letters in the word pyramid.\n";
      continue;
    }

    processGuess(input);

    if (allTargetWordsGuessed()) {
      std::cout << "Congratulations! You have successfully escaped from the Word Pyramids of Giza. "
                   "Good luck on your journey brave explorer!\n";
      logGameResult(true);
      return false;
    }
  }
}

// Launches the full interactive game with menu and replay options.
//  Input parameters: none.
// Return value: none.

void runWordPyramidGame() {
  std::cout << "===============================================\n";
  std::cout << "        WORD PYRAMIDS OF GIZA MINI-GAME        \n";
  std::cout << "===============================================\n";
  std::cout << "Greetings brave traveller. You are stuck in the haunting Word Pyramids of Giza. "
               "To escape, find and unscramble the hidden words in the pyramid below. "
               "If you think you found a word, type it out. The letters, category, and number "
               "of words are provided. If you take a wrong turn, type 'clear'. If you want a hint,"
                "type 'hint'. To exit the game type 'exit'. Good luck with your mission!\n";

  bool keepPlaying = true;
  while (keepPlaying) {
    std::cout << "Choose Difficulty:\n";
    std::cout << "1) Easy\n2) Medium\n3) Hard\n";
    std::cout << "Enter choice (1-3): ";

    std::string choiceText;
    if (!std::getline(std::cin, choiceText)) {
      std::cout << "\nInput ended.\n";
      return;
    }

    int difficulty = 0;
    if (choiceText == "1" || choiceText == "2" || choiceText == "3") {
      difficulty = choiceText[0] - '0';
    } else {
      std::cout << "Invalid difficulty choice. Try again.\n\n";
      continue;
    }

    WordPyramidGame* game = new WordPyramidGame();
    bool ok = game->initialize(difficulty);
    if (!ok) {
      std::cout << "Unable to initialize puzzle for this difficulty.\n";
      delete game;
      continue;
    }

    bool userWantstoExit= game->runGameLoop();
    delete game;
    
    if (userWantstoExit) {
      break;
    }

    std::cout << "\nPlay again? (Y/N): ";
    std::string replay;
    if (!std::getline(std::cin, replay)) {
      std::cout << "\nInput ended.\n";
      return;
    }
    replay = replay.empty() ? "N" : replay;
    char c = static_cast<char>(std::toupper(static_cast<unsigned char>(replay[0])));
    if (c != 'Y') {
      keepPlaying = false;
    }
    std::cout << "\n";
  }

  std::cout << "Farewell, explorer.\n";
}
