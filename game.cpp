#include "game.h"
#include "puzzle.h"
#include <algorithm>
#include <chrono>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <random>
#include <sstream>

// ... [The entire top helper namespace section of your original code remains here] ...
namespace {

std::mt19937 &globalRng() {
    static std::mt19937 rng(
        static_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count()));
    return rng;
}

std::string toUpper(std::string s) {
    for (char &c : s) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    return s;
}

std::string trim(const std::string &s) {
    std::size_t i = 0;
    std::size_t j = s.size();
    while (i < s.size() && std::isspace(static_cast<unsigned char>(s[i]))) i++;
    while (j > i && std::isspace(static_cast<unsigned char>(s[j - 1]))) j--;
    return s.substr(i, j - i);
}

std::string normalizeWord(const std::string &s) {
    std::string out;
    for (char c : s) {
        if (std::isalpha(static_cast<unsigned char>(c))) {
            out.push_back(static_cast<char>(std::toupper(static_cast<unsigned char>(c))));
        }
    }
    return out;
}

int directionToInt(Direction d) { return d == Direction::Across ? 0 : 1; }

Direction intToDirection(int x) { return x == 0 ? Direction::Across : Direction::Down; }

bool saveGame(const std::string &path, const SaveMeta &meta, const GameState &state) {
    std::ofstream out(path.c_str());
    if (!out) return false;
    out << "CW_SAVE_V1\n";
    out << meta.difficultyChoice << " " << meta.elapsedSeconds << " " << (meta.hardBellPlayed ? 1 : 0) << "\n";
    out << state.score << " " << state.hintsUsed << " " << state.currentStreak << "\n";
    out << state.solutionGrid.size() << "\n";
    for (const auto &row : state.solutionGrid) out << row << "\n";

    out << state.numbering.size() << " " << (state.numbering.empty() ? 0 : state.numbering[0].size()) << "\n";
    for (const auto &row : state.numbering) {
        for (std::size_t i = 0; i < row.size(); i++) {
            if (i) out << " ";
            out << row[i];
        }
        out << "\n";
    }

    out << state.clues.size() << "\n";
    for (const auto &cl : state.clues) {
        out << cl.number << "|" << directionToInt(cl.direction) << "|" << cl.answer << "|" << cl.row << "|" << cl.col
            << "|" << cl.clueText << "\n";
    }

    out << state.wordBank.size() << "\n";
    for (const auto &w : state.wordBank) out << w << "\n";
    out << state.remainingWords.size() << "\n";
    for (const auto &w : state.remainingWords) out << w << "\n";

    out << state.solvedByKey.size() << "\n";
    for (const auto &entry : state.solvedByKey) {
        out << entry.first.first << "|" << directionToInt(entry.first.second) << "|" << entry.second << "\n";
    }
    return true;
}

bool loadGame(const std::string &path, SaveMeta &meta, GameState &state) {
    std::ifstream in(path.c_str());
    if (!in) return false;
    std::string line;
    if (!std::getline(in, line) || line != "CW_SAVE_V1") return false;

    if (!std::getline(in, line)) return false;
    {
        std::istringstream iss(line);
        int bell = 0;
        if (!(iss >> meta.difficultyChoice >> meta.elapsedSeconds >> bell)) return false;
        meta.hardBellPlayed = (bell != 0);
    }

    if (!std::getline(in, line)) return false;
    {
        std::istringstream iss(line);
        if (!(iss >> state.score >> state.hintsUsed >> state.currentStreak)) return false;
    }

    if (!std::getline(in, line)) return false;
    int gridRows = std::stoi(line);
    state.solutionGrid.clear();
    for (int i = 0; i < gridRows; i++) {
        if (!std::getline(in, line)) return false;
        state.solutionGrid.push_back(line);
    }

    if (!std::getline(in, line)) return false;
    {
        std::istringstream iss(line);
        int rows = 0, cols = 0;
        if (!(iss >> rows >> cols)) return false;
        state.numbering.assign(rows, std::vector<int>(cols, 0));
        for (int r = 0; r < rows; r++) {
            if (!std::getline(in, line)) return false;
            std::istringstream rowStream(line);
            for (int c = 0; c < cols; c++) rowStream >> state.numbering[r][c];
        }
    }

    if (!std::getline(in, line)) return false;
    int clueCount = std::stoi(line);
    state.clues.clear();
    for (int i = 0; i < clueCount; i++) {
        if (!std::getline(in, line)) return false;
        std::stringstream ss(line);
        std::string a, b, c, d, e, f;
        std::getline(ss, a, '|');
        std::getline(ss, b, '|');
        std::getline(ss, c, '|');
        std::getline(ss, d, '|');
        std::getline(ss, e, '|');
        std::getline(ss, f);
        state.clues.push_back(
            {std::stoi(a), intToDirection(std::stoi(b)), c, f, std::stoi(d), std::stoi(e)});
    }

    if (!std::getline(in, line)) return false;
    int bankCount = std::stoi(line);
    state.wordBank.clear();
    for (int i = 0; i < bankCount; i++) {
        if (!std::getline(in, line)) return false;
        state.wordBank.push_back(line);
    }

    if (!std::getline(in, line)) return false;
    int remainingCount = std::stoi(line);
    state.remainingWords.clear();
    for (int i = 0; i < remainingCount; i++) {
        if (!std::getline(in, line)) return false;
        state.remainingWords.insert(line);
    }

    if (!std::getline(in, line)) return false;
    int solvedCount = std::stoi(line);
    state.solvedByKey.clear();
    for (int i = 0; i < solvedCount; i++) {
        if (!std::getline(in, line)) return false;
        std::stringstream ss(line);
        std::string num, dir, word;
        std::getline(ss, num, '|');
        std::getline(ss, dir, '|');
        std::getline(ss, word);
        state.solvedByKey[{std::stoi(num), intToDirection(std::stoi(dir))}] = word;
    }
    return true;
}

void printScrollIntro() {
    std::cout << "\nAn ancient parchment map unfurls before your eyes...\n";
    std::cout << "Mysterious clues emerge from the crumbling scrolls...\n";
    std::cout << "The Crossword of the Ancient Scrolls awaits!\n\n";
}

void printSeparator() {
    std::cout << "------------------------------------------------------\n";
}

void printRules(const DifficultyConfig &cfg) {
    printSeparator();
    std::cout << "=== " << cfg.name << " Mode ===\n";
    std::cout << "Theme: " << cfg.theme << "\n";
    std::cout << "Grid: " << cfg.gridSize << "x" << cfg.gridSize << "\n";
    if (cfg.timeLimitSec > 0) std::cout << "Time Limit: " << cfg.timeLimitSec / 60 << " minute(s)\n";
    else std::cout << "Time Limit: None\n";
    if (cfg.unlimitedHints) std::cout << "Hints: Unlimited\n";
    else std::cout << "Hints: " << cfg.maxHints << "\n";
    std::cout << "Base points/word: " << cfg.basePointsPerWord << "\n";
    printSeparator();
}

void printClues(const std::vector<Clue> &clues) {
    std::vector<Clue> across, down;
    for (const auto &c : clues) {
        if (c.direction == Direction::Across) across.push_back(c);
        else down.push_back(c);
    }
    std::sort(across.begin(), across.end(), [](const Clue &a, const Clue &b) { return a.number < b.number; });
    std::sort(down.begin(), down.end(), [](const Clue &a, const Clue &b) { return a.number < b.number; });
    std::cout << "\nAcross:\n";
    for (const auto &c : across) std::cout << "  " << c.number << ". " << c.clueText << " (" << c.answer.size() << ")\n";
    std::cout << "Down:\n";
    for (const auto &c : down) std::cout << "  " << c.number << ". " << c.clueText << " (" << c.answer.size() << ")\n";
}

void printWordBank(const std::set<std::string> &remaining) {
    std::cout << "\nWord Bank (" << remaining.size() << " remaining):\n";
    int cnt = 0;
    for (const auto &w : remaining) {
        std::cout << std::setw(14) << std::left << w;
        cnt++;
        if (cnt % 5 == 0) std::cout << "\n";
    }
    if (cnt % 5 != 0) std::cout << "\n";
}

bool isCellSolved(const GameState &state, int row, int col, char &outCh) {
    outCh = '_';
    for (const auto &entry : state.solvedByKey) {
        int num = entry.first.first;
        Direction d = entry.first.second;
        const std::string &word = entry.second;
        for (const auto &cl : state.clues) {
            if (cl.number != num || cl.direction != d) continue;
            int dr = (d == Direction::Across ? 0 : 1);
            int dc = (d == Direction::Across ? 1 : 0);
            for (int i = 0; i < static_cast<int>(word.size()); i++) {
                int rr = cl.row + dr * i;
                int cc = cl.col + dc * i;
                if (rr == row && cc == col) {
                    outCh = word[i];
                    return true;
                }
            }
        }
    }
    return false;
}

void printPlayerView(const GameState &state) {
    int n = static_cast<int>(state.solutionGrid.size());
    std::cout << "\nCurrent Puzzle:\n   ";
    for (int c = 0; c < n; c++) std::cout << std::setw(3) << c;
    std::cout << "\n";
    for (int r = 0; r < n; r++) {
        std::cout << std::setw(3) << r;
        for (int c = 0; c < n; c++) {
            if (state.solutionGrid[r][c] == '#') {
                std::cout << "  #";
            } else {
                char out = '_';
                bool solved = isCellSolved(state, r, c, out);
                std::cout << "  " << (solved ? out : '_');
            }
        }
        std::cout << "\n";
    }
    std::cout << "\nCell numbering (start cells):\n";
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {
            if (state.numbering[r][c] == 0) continue;
            std::cout << "[" << state.numbering[r][c] << "@(" << r << "," << c << ")] ";
        }
    }
    std::cout << "\n";
}

void printSolutionGrid(const GameState &state) {
    int n = static_cast<int>(state.solutionGrid.size());
    std::cout << "\n=== Final Board (Solution) ===\n\nGrid:\n   ";
    for (int c = 0; c < n; c++) std::cout << std::setw(3) << c;
    std::cout << "\n";
    for (int r = 0; r < n; r++) {
        std::cout << std::setw(3) << r;
        for (int c = 0; c < n; c++) {
            char ch = state.solutionGrid[r][c];
            if (ch == '#') std::cout << "  #";
            else std::cout << "  " << ch;
        }
        std::cout << "\n";
    }
}

} // namespace

CrosswordGame::CrosswordGame(const std::string &savePath)
    : savePath_(savePath),
      difficultyChoice_(1),
      loadedElapsedSeconds_(0),
      hardBellPlayed_(false) {
    state_.reset(new GameState());
}

void CrosswordGame::run() {
    printScrollIntro();
    std::cout << "App 5: Crossword (Anshika)\n";
    if (!tryLoadGame()) {
        if (!startNewGame()) return;
    }
    printRules(cfg_);
    printClues(state_->clues);
    gameLoop();
    applyFinalScoring();
    printSolutionGrid(*state_);
    std::cout << "\nFinal Score: " << state_->score << "\n";
    std::cout << "Solved " << (state_->clues.size() - state_->remainingWords.size()) << " / "
              << state_->clues.size() << " clues.\n";
    std::cout << "Game over.\n";
}

bool CrosswordGame::tryLoadGame() {
    SaveMeta meta;
    GameState loaded;
    if (!loadGame(savePath_, meta, loaded)) return false;

    std::cout << "Saved game found. Type 'yes' to continue saved game: ";
    std::string ans;
    std::getline(std::cin, ans);
    ans = toUpper(trim(ans));
    if (ans != "YES" && ans != "Y") return false;

    difficultyChoice_ = meta.difficultyChoice;
    cfg_ = getConfig(difficultyChoice_);
    *state_ = loaded;
    loadedElapsedSeconds_ = meta.elapsedSeconds;
    hardBellPlayed_ = meta.hardBellPlayed;
    startTime_ = std::chrono::steady_clock::now() - std::chrono::seconds(loadedElapsedSeconds_);
    std::cout << "Loaded saved game.\n";
    return true;
}

// ----------------------------------------------------
// THE NEW startNewGame FUNCTION WITH INPUT VALIDATION!
// ----------------------------------------------------
bool CrosswordGame::startNewGame() {
    std::cout << "Choose Level:\n";
    std::cout << "  1) Easy\n";
    std::cout << "  2) Medium\n";
    std::cout << "  3) Hard\n";
    std::cout << "Enter choice: " << std::flush;

    while (true) {
        std::cin >> difficultyChoice_;

        if (std::cin.fail() || difficultyChoice_ < 1 || difficultyChoice_ > 3) {
            std::cout << "\033[1;31m [ERROR] Invalid input. Please enter 1, 2, or 3: \033[0m";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        }
    }

    cfg_ = getConfig(difficultyChoice_);
    auto raw = getRawByDifficulty(difficultyChoice_);
    std::vector<std::string> solution;
    std::vector<Clue> clues;
    if (!buildCrossword(cfg_.gridSize, raw, solution, clues)) {
        std::cout << "Failed to generate crossword. Please retry.\n";
        return false;
    }

    state_.reset(new GameState());
    state_->solutionGrid = solution;
    state_->clues = clues;
    state_->numbering = buildNumbering(solution);
    for (const auto &cl : clues) {
        state_->wordBank.push_back(cl.answer);
        state_->remainingWords.insert(cl.answer);
    }
    std::sort(state_->wordBank.begin(), state_->wordBank.end());
    state_->wordBank.erase(std::unique(state_->wordBank.begin(), state_->wordBank.end()), state_->wordBank.end());

    loadedElapsedSeconds_ = 0;
    hardBellPlayed_ = false;
    startTime_ = std::chrono::steady_clock::now();
    return true;
}

int CrosswordGame::secondsRemaining() const {
    if (cfg_.timeLimitSec <= 0) return std::numeric_limits<int>::max();
    auto now = std::chrono::steady_clock::now();
    int elapsed = static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(now - startTime_).count());
    return cfg_.timeLimitSec - elapsed;
}

bool CrosswordGame::isSolved() const { return state_->remainingWords.empty(); }

const Clue *CrosswordGame::findClue(int number, Direction d) const {
    for (const auto &cl : state_->clues) {
        if (cl.number == number && cl.direction == d) return &cl;
    }
    return nullptr;
}

bool CrosswordGame::revealOneLetter() {
    std::vector<std::pair<int, Direction>> unsolved;
    for (const auto &cl : state_->clues) {
        if (!state_->solvedByKey.count({cl.number, cl.direction})) unsolved.push_back({cl.number, cl.direction});
    }
    if (unsolved.empty()) return false;
    std::uniform_int_distribution<int> dist(0, static_cast<int>(unsolved.size()) - 1);
    auto pick = unsolved[dist(globalRng())];
    const Clue *cl = findClue(pick.first, pick.second);
    if (!cl) return false;

    std::uniform_int_distribution<int> ldist(0, static_cast<int>(cl->answer.size()) - 1);
    int idx = ldist(globalRng());
    std::cout << "Hint: Clue " << cl->number << (cl->direction == Direction::Across ? "A" : "D")
              << " has letter '" << cl->answer[idx] << "' at position " << (idx + 1) << ".\n";
    return true;
}

void CrosswordGame::processHint() {
    bool canHint = cfg_.unlimitedHints || state_->hintsUsed < cfg_.maxHints;
    if (!canHint) {
        std::cout << "No hints remaining.\n";
        return;
    }
    if (revealOneLetter()) state_->hintsUsed++;
    else std::cout << "No hint available.\n";
}

void CrosswordGame::processSolve() {
    std::cout << "Enter clue number: " << std::flush;
    std::string numS;
    std::getline(std::cin, numS);
    int number = 0;
    try {
        number = std::stoi(trim(numS));
    } catch (...) {
        std::cout << "Invalid clue number.\n";
        return;
    }

    std::cout << "Direction (A/D): " << std::flush;
    std::string dirS;
    std::getline(std::cin, dirS);
    dirS = toUpper(trim(dirS));
    if (dirS.empty() || (dirS[0] != 'A' && dirS[0] != 'D')) {
        std::cout << "Invalid direction.\n";
        return;
    }
    Direction dir = (dirS[0] == 'A' ? Direction::Across : Direction::Down);
    const Clue *cl = findClue(number, dir);
    if (!cl) {
        std::cout << "No such clue with that direction.\n";
        return;
    }
    if (state_->solvedByKey.count({number, dir})) {
        std::cout << "That clue is already solved.\n";
        return;
    }

    std::cout << "Choose word from bank: " << std::flush;
    std::string word;
    std::getline(std::cin, word);
    word = normalizeWord(word);
    if (word.empty()) {
        std::cout << "Invalid word input.\n";
        return;
    }
    if (!state_->remainingWords.count(word)) {
        std::cout << "Word not available in remaining bank.\n";
        return;
    }

    if (word == cl->answer) {
        state_->solvedByKey[{cl->number, cl->direction}] = cl->answer;
        state_->remainingWords.erase(cl->answer);
        state_->score += cfg_.basePointsPerWord;
        state_->currentStreak++;
        if (cfg_.streakBonus && state_->currentStreak > 0 &&
            state_->currentStreak % cfg_.streakThreshold == 0) {
            state_->score += cfg_.streakBonusValue;
            std::cout << "Streak bonus! +" << cfg_.streakBonusValue << " points.\n";
        }
        std::cout << "Correct! Word placed. +" << cfg_.basePointsPerWord << " points\n";
    } else {
        if (cfg_.wrongGuessPenalty > 0) state_->score -= cfg_.wrongGuessPenalty;
        state_->currentStreak = 0;
        std::cout << "Incorrect match.\n";
    }
}

void CrosswordGame::saveCurrentGame() const {
    SaveMeta meta;
    meta.difficultyChoice = difficultyChoice_;
    meta.elapsedSeconds = static_cast<int>(
        std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - startTime_).count());
    meta.hardBellPlayed = hardBellPlayed_;
    if (saveGame(savePath_, meta, *state_)) std::cout << "Game saved to " << savePath_ << "\n";
    else std::cout << "Could not save game.\n";
}

void CrosswordGame::gameLoop() {
    while (true) {
        int remain = secondsRemaining();
        if (cfg_.timeLimitSec > 0) {
            if (remain <= 0) {
                std::cout << "\nTime's up! The ancient hourglass is empty.\n";
                break;
            }
            if (cfg_.finalMinuteBell && remain <= 60 && !hardBellPlayed_) {
                std::cout << "\a[Temple Bell] Final minute begins!\n";
                hardBellPlayed_ = true;
            }
            std::cout << "\nTime Remaining: " << remain << " sec\n";
        }

        printPlayerView(*state_);
        printWordBank(state_->remainingWords);
        
        std::cout << "Score: " << state_->score << " | Hints used: " << state_->hintsUsed;
        if (cfg_.streakBonus) std::cout << " | Current streak: " << state_->currentStreak;
        std::cout << "\n";

        if (isSolved()) {
            printSeparator();
            std::cout << "PUZZLE CLEARED!\n";
            printSeparator();
            break;
        }

        std::cout << "\nCommand options:\n";
        std::cout << "  solve  -> enter clue number + direction + word\n";
        std::cout << "  hint   -> reveal one letter (if available)\n";
        std::cout << "  clues  -> show clues again\n";
        std::cout << "  save   -> save game progress\n";
        std::cout << "  quit   -> save and end game\n";
        std::cout << "Enter command: " << std::flush;

        std::string cmd;
        std::getline(std::cin, cmd);
        cmd = toUpper(trim(cmd));
        if (cmd == "QUIT") {
            std::cout << "You leave the ruins for now.\n";
            saveCurrentGame();
            break;
        } else if (cmd == "CLUES") {
            printClues(state_->clues);
        } else if (cmd == "HINT") {
            processHint();
        } else if (cmd == "SOLVE") {
            processSolve();
        } else if (cmd == "SAVE") {
            saveCurrentGame();
        } else {
            std::cout << "Unknown command.\n";
        }
    }
}

void CrosswordGame::applyFinalScoring() {
    int remain = secondsRemaining();
    if (cfg_.timeLimitSec > 0 && remain > 0 && isSolved()) {
        int bonus = remain * cfg_.finishTimeBonusFactor;
        state_->score += bonus;
        std::cout << "Time bonus: +" << bonus << "\n";
    }
    if (difficultyChoice_ == 2 && state_->hintsUsed > 0) {
        int reductionPercent = std::min(90, state_->hintsUsed * cfg_.hintPenaltyValue);
        int reduceBy = (state_->score * reductionPercent) / 100;
        state_->score -= reduceBy;
        std::cout << "Hint penalty: -" << reductionPercent << "% (" << reduceBy << " points)\n";
    }
    if (difficultyChoice_ == 3 && state_->hintsUsed > 0) {
        state_->score -= cfg_.hintPenaltyValue;
        std::cout << "Sacred hint penalty: -" << cfg_.hintPenaltyValue << " points\n";
    }
}
