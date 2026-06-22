# COMP2113-Group-129
# Odyssey OS: The Heritage Terminal

Group Members: 
1. Menaka Kalpally Pulapra Menon (3036505757)
2. Anushka Jitani (3036474336)
3. Krislyn Mariah Mendonca
4. Anshika Mittal (3036476023)
5. Arpita Sharma (3036476279)
6. Miracle Ukazu Oluebube (3036566892)

Contributions: 

App 1: Menaka Kalpally Pulapra Menon
App 2: Anushka Jitani
App 3: Krislyn Mariah Mendonca
App 4: Anshika Mittal
App 5: Arpita Sharma
App 6: Miracle Ukazu Oluebube


Overview:

Odyssey OS: The Heritage Terminal is a vintage-style interactive terminal game that simulates an old-school computer system. The player logs into "Odyssey OS," a simulated classified database, to decrypt corrupted files related to world heritage sites, history, art, and architecture. The player will need to complete various mini-games, each focused on different themes like ancient civilizations, movies, art history, and more.

## Table of Contents
- [Game Concept](#game-concept)
- [Mini-Games Overview](#mini-games-overview)
  - [App 1: Word Pyramids of Giza](#app-1-word-pyramids-of-giza)
  - [App 2: The Director's Archive](#app-2-the-directors-archive)
  - [App 3: The Time-Slip Syndicate](#app-3-the-time-slip-syndicate)
  - [App 4: Crossword](#app-5-crossword)
  - [App 5: The Forger's Table](#app-6-the-forgers-table)
  - [App 6: The Curator's Cryptex](#app-4-the-curators-cryptex)]
- [Technologies Used](#technologies-used)
- [Installation](#installation)
  

## Game Concept

The player interacts with **Odyssey OS**, a system housing encrypted data of historical and cultural artifacts. Each **mini-game app** challenges the player’s knowledge of history, architecture, mythology through a series of immersive puzzles. Players must complete these games to eventually unlock the final trial, **The Curator's Cryptex**.

## Mini-Games Overview

## App 1: Word Pyramids of Giza
### Overview
A terminal-based word pyramid puzzle where letters are arranged in a triangular formation. Players must identify and guess all hidden words using only the letters shown in the pyramid. The game features multiple difficulty levels with different historical and mythological categories, a hint system, a clear/reset function, and file-based word banks.

### Coding Elements Implementation

| Requirement | Implementation Location |
|-------------|------------------------|
| **Generation of random events** | `WordPyramidGame()` constructor seeds `mt19937` with `random_device`; `generatePuzzlesFromWordBank()` randomly selects word subsets; `initialize()` randomly picks a puzzle from difficulty bank; `processHint()` randomly chooses an unguessed target word; `std::shuffle` randomises letter order in pyramid (all in `word_pyramid.cpp`) |
| **Data structures for storing data** | `struct PuzzleState` in `word_pyramid.h` (holds `vector<string> targetWords`, `vector<bool> correctlyGuessed`, `vector<string> guessedWords`, `vector<char>` letter pools, `int` metadata); `vector<vector<string>> easyPuzzles`, `mediumPuzzles`, `hardPuzzles` in `WordPyramidGame` class |
| **Dynamic memory management** | `activePuzzle = new PuzzleState()` in `initialize()` (`word_pyramid.cpp`); `delete activePuzzle` in `~WordPyramidGame()` destructor; `WordPyramidGame* game = new WordPyramidGame()` in `runWordPyramidGame()`; `delete game` after each round |
| **File input/output** | `loadWordBank()` reads `easy_words.txt`, `medium_words.txt`, `hard_words.txt` (input); `logGameResult()` appends game outcomes (WIN/EXIT, targets, guesses, hints left) to `game_log.txt` (output) – both in `word_pyramid.cpp` |
| **Program codes in multiple files** | `word_pyramid.h`, `word_pyramid.cpp`, `main.cpp`, `Makefile` |
| **Multiple Difficulty Levels** | `buildPuzzleSets()` creates three separate puzzle banks; user selects 1 (Easy), 2 (Medium), or 3 (Hard) in `runWordPyramidGame()`; `initialize(int difficultyChoice)` chooses the appropriate bank; `currentDifficulty` changes displayed category – all in `word_pyramid.cpp` |

### Difficulty Levels

| Difficulty | Word Category | Words per Puzzle | Total Letters | Hints |
|------------|---------------|------------------|---------------|-------|
| Easy (1) | Gods, Monsters, and Myths (Greek, Roman, Egyptian, Hindu, Nordic) | 4–7 | 40–60 | 3 |
| Medium (2) | Empires, Innovation, Monuments | 4–7 | 40–60 | 3 |
| Hard (3) | Rulers, Pharaohs, and Heroes (Greek, Roman, Egyptian, Nordic) | 4–7 | 40–60 | 3 |

### Commands

| Command | Description |
|---------|-------------|
| `[word]` | Guess a word (e.g., `FOX`). Must use only letters currently in the pyramid |
| `clear` | Resets the letter pool and guessed words – restarts the current puzzle |
| `hint` | Reveals one random unguessed target word (max 3 hints per puzzle) |
| `exit` | Returns to the main difficulty selection menu |

### How to Play

1. Run `./word_pyramid` and select a difficulty level (1=Easy, 2=Medium, 3=Hard)
2. Read the category and the number of hidden words
3. Look at the letter pyramid – all available letters are shown
4. Type a word that can be formed from those letters
5. If correct, letters are removed and the word appears in the “Guessed words” list
6. Use `clear` to restart the puzzle if stuck
7. Use `hint` for a random revealed word (max 3 hints)
8. Guess all hidden words to win and escape the pyramid
9. Type `exit` to go back and change difficulty or quit


# App 2: The Director's Archive 

## 1. Detailed Overview
**The Director's Archive** is a cinematic, cryptography-style hangman game built into the Odyssey OS. Operating under the guise of recovering corrupted film reels, players act as cyber-archaeologists. The system presents a specific scene description from a famous movie (e.g., *“Giant alien robots fight in the Egyptian desert...”*). 

The player must deduce and decrypt the real-world **Heritage Site** where the scene takes place (e.g., *PYRAMIDS OF GIZA*). The hidden location is displayed as a series of underscores (`_ _ _ _`), and the player must guess it letter by letter. Incorrect guesses "burn a frame of film" (lose a life) and trigger progressive hints about the location's history to guide the player toward the correct answer.

---

## 2. Coding Elements Implementation
This module was specifically engineered to showcase all five core course requirements within a single application. 

*   **1. Generation of Random Events:**
    *   **Implementation:** The game utilizes the `<random>` and `<algorithm>` libraries to ensure replayability. Upon loading the database, `std::random_device` seeds a Mersenne Twister engine (`std::mt19937`), and `std::shuffle` randomizes the `siteDatabase` vector. 
    *   **Purpose:** This guarantees that the player receives a completely random sequence of movie clues every time they launch the app, preventing predictable gameplay. If the player exhausts the entire deck, the system detects this and automatically reshuffles the archive.
*   **2. Data Structures for Storing Data:**
    *   **Implementation:** A custom C++ `struct` named `HeritageSite` is used to group the data. It contains `movieClue` (string), `answer` (string), and `hints` (a vector of strings). 
    *   **Purpose:** This struct provides a clean blueprint for each puzzle. During runtime, a `std::vector<HeritageSite> siteDatabase` stores all loaded locations, acting as the primary data structure for the game deck.
*   **3. Dynamic Memory Management:**
    *   **Implementation:** Memory is managed dynamically on the heap via Standard Template Library (STL) containers. 
    *   **Purpose:** Because each movie location in `movies.txt` has a variable number of hints (some have two, some have three), `std::vector<string> hints` safely and dynamically allocates the exact amount of heap memory required as it parses the file using `.push_back()`. Additionally, `std::string guessedState` dynamically resizes based on the character length of the hidden answer.
*   **4. File Input/Output (I/O):**
    *   **File Input:** The `loadFiles()` function uses `std::ifstream` to open and read `movies.txt`. It processes the text line by line, using a `std::stringstream` and `getline()` with a pipe character (`|`) delimiter to neatly separate the clue, answer, and hints into the data structure.
    *   **Console I/O:** Standard `std::cin` and `std::cout` are used extensively. The game features robust input validation, ensuring the user only inputs a single alphabetical character (A-Z) and trapping them in a `while` loop with an error message if they type numbers or multiple letters.
*   **5. Program Codes in Multiple Files:**
    *   **Implementation:** The code is strictly compartmentalized. `celluloid_app.h` contains the class declarations, member variables, and struct definitions. `celluloid_app.cpp` contains the heavy logic and implementation. The app is then instantiated and executed from the central `main.cpp` file.

---

## 3. Difficulty Levels
When launching The Director's Archive, the player is prompted to choose a security clearance level (1-3). This directly manipulates the internal state variables (`lives` and `hintsAllowed`), altering the risk/reward mechanics of the loop:

*   **Level 1 (Easy):** 
    *   **Lives:** 4 Frames 
    *   **Hints Allowed:** 3 Hints
    *   *Design:* Highly forgiving. Gives the player maximum contextual clues to figure out obscure heritage sites.
*   **Level 2 (Medium):** 
    *   **Lives:** 3 Frames
    *   **Hints Allowed:** 2 Hints
    *   *Design:* The standard experience. Requires a balance of movie knowledge and careful guessing.
*   **Level 3 (Hard):** 
    *   **Lives:** 2 Frames
    *   **Hints Allowed:** 1 Hint
    *   *Design:* Punishing. A single wrong guess triggers the only hint. A second wrong guess destroys the file. Players must rely heavily on their own trivia knowledge.

---

## 4. How to Play

1.  **Launch:** Select Option 2 from the Odyssey OS Main Menu.
2.  **Difficulty Selection:** Enter `1`, `2`, or `3` to set your parameters.
3.  **The Clue:** The terminal will display a yellow text prompt detailing a famous movie scene (e.g., *"An aristocratic adventurer navigates through massive, ancient tree roots crushing the ruins of a jungle temple in Cambodia. (Movie: Lara Croft: Tomb Raider, 2001)"*).
4.  **Decryption:** A target word made of underscores (`_ _ _ _ _`) will appear. 
5.  **Input:** Type a single letter and press `ENTER`. 
    *   If correct, the letter fills into the blank spaces.
    *   If incorrect, a warning bell sounds (`\a`), a "Frame" (life) burns up, and a blue text hint about the location's real-world history is revealed.
6.  **Loop:** You will be prompted to "Load another reel? (Y/N)" to play another random round until you decide to exit to the main menu.

---

## 5. Scoring System & Progression
The Director's Archive does not use a numerical points system; it uses a **binary state progression**.

*   **Victory State (`Match found!`):** If the player's `guessedState` string exactly matches the `hiddenAnswer` string before their `currentLives` counter reaches zero, the record is marked as `[DECRYPTED]`. 
*   **Failure State (`[DATA LOST]`):** If `currentLives` hits `0`, the loop breaks, the film reel is "burned," and the correct answer is revealed to the player so they can learn from their mistake.
*   **Meta-Progression:** Winning at least one round of The Director's Archive flips the `directorsArchiveDone` boolean in the main operating system to `true`. This is a mandatory prerequisite for bypassing the system lock and accessing the final boss game, The Curator's Cryptex.</HeritageSite>

## App 3: The Time-Slip Syndicate

**Developer:** Krislyn Mariah Mendonca

### Theme
Architecture & History - A Wordle-style game where players decode 5-letter historical and architectural terms.

### Core Gameplay
- 6 attempts to guess a 5-letter historical/architectural word
- Color-coded feedback system
- 6x5 grid with persistent guess history
- Hint system available in Easy mode

### Difficulty Levels

| Difficulty | Word Bank Size | Hints |
|------------|----------------|-------|
| **EASY** | 10 common words | Yes (after 3 guesses) |
| **MEDIUM** | 10 mixed words | No |
| **HARD** | 10 obscure terms | No |

### Word Banks by Difficulty

**EASY Mode Words:** RUINS, MAYA, ARCH, DOME, WALL, TOWER, TEMPLE, STONE, BRICK, PILLAR

**MEDIUM Mode Words:** TOMBS, AZTEC, STOAS, PYLON, OBELI, MAYAN, INCA, FORUM, COLUMN, VAULT

**HARD Mode Words:** ZIGG, STELA, LINTL, OGIVE, TRABE, DORIC, IONIC, CORIN, CARYA, ROSET

### Controls

| Action | Input |
|--------|-------|
| Type your guess | Any 5 letters A-Z |
| Submit guess | `ENTER` |
| Exit game | Type `exit` or `quit` |

### Visual Feedback Guide

After each guess, the game shows visual indicators for each letter:

| Symbol | Meaning | What To Do |
|--------|---------|-------------|
| `[R]` | **GREEN** - Correct letter, right position | Keep this letter in this position |
| `(U)` | **YELLOW** - Correct letter, wrong position | Use this letter elsewhere in your next guess |
| `{N}` | **GRAY** - Letter not in the word | Don't use this letter again |
| `_` | Empty cell (not yet guessed) | Fill with your next guess |

### How to Play

1. **Start the Game:** Run the program and select difficulty from the main menu (1-3)
2. **View Instructions:** Press 4 from main menu to see detailed instructions
3. **Make a Guess:** Type any 5-letter word and press `ENTER`
4. **Analyze Feedback:** Look at the symbols to see which letters are correct:
   - `[Letter]` = Perfect! Keep it there
   - `(Letter)` = Good letter, wrong spot - try moving it
   - `{Letter}` = Wrong letter - avoid using it again
5. **Use Hints (Easy Mode Only):** After 3 wrong guesses, the game reveals all letters in the target word
6. **Win or Lose:** 
   - **WIN** by guessing the word within 6 attempts
   - **LOSE** if you use all 6 attempts




## App 4: Crossword
**Theme:** Wonders & Landmarks, Art Movements, Iconoclasts  

### Overview
A terminal-based crossword puzzle game where players solve clues to uncover words from a word bank. The game features multiple difficulty levels, time limits, hint systems, scoring mechanics, and save/load functionality.

### Coding Elements Implementation

| Requirement | Implementation Location |
|-------------|------------------------|
| **Generation of random events** | `revealOneLetter()` in `game.cpp` - randomly selects unsolved clues and random letter positions |
| **Data structures for storing data** | `struct GameState`, `struct Clue`, `struct DifficultyConfig` in `puzzle.h` - uses vectors, sets, maps |
| **Dynamic memory management** | `std::unique_ptr<GameState>` in `game.h` - automatic memory management |
| **File input/output** | `saveGame()` and `loadGame()` in `game.cpp` - saves/loads game state to `save_data.txt` |
| **Program codes in multiple files** | `main.cpp`, `game.cpp`, `game.h`, `puzzle.cpp`, `puzzle.h`, `art.h` |
| **Multiple Difficulty Levels** | `getConfig()` in `puzzle.cpp` - Easy, Medium, Hard with different settings |

### Difficulty Levels

| Difficulty | Grid Size | Time Limit | Max Hints | Base Points |
|------------|-----------|------------|-----------|-------------|
| Easy | 7x7 | None | Unlimited | 100 |
| Medium | 9x9 | 10 minutes | 3 | 150 |
| Hard | 11x11 | 7 minutes | 1 | 220 |

### How to Play

1. Run the program and select a difficulty level (1=Easy, 2=Medium, 3=Hard)
2. View the clues (Across and Down) and the word bank
3. Use the `solve` command to guess a word for a clue : Enter clue number, direction (A/D), and word from bank
4. Use `hint` to reveal a random letter (costs points on higher difficulties) : Reveals one random letter from an unsolved clue
5. Use `clues` to view clues again 
6. Use `save` to save progress
7. Use `quit` to save and exit

### Scoring System

| Action | Points |
|--------|--------|
| Correct word | +100 (Easy), +150 (Medium), +220 (Hard) |
| Wrong guess | 0 (Easy), -10 (Medium), -25 (Hard) |
| Streak bonus (Medium) | +2 points every 7 correct words |
| Time bonus (Medium/Hard) | Points per second remaining |
| Hint penalty (Medium) | 7% reduction per hint (max 90%) |
| Hint penalty (Hard) | -150 points per hint |






## **App 5: The Forger's Table**

**Theme:** Heritage Crimes Investigation  

### Overview
A terminal-based artifact authentication game. Two classified dossiers appear side by side — one genuine, one tampered by a forger. The player must identify which field(s) have been falsified. Across three difficulty levels the challenge escalates: labels disappear, multiple fields are forged simultaneously, a countdown timer is introduced, and some dossier pairs may be entirely authentic (requiring the player to call it out).

---

### Coding Elements Implementation

| Requirement | Implementation Location |
|-------------|------------------------|
| **Generation of random events** | `std::mt19937 g_rng{std::random_device{}()}` seeds the RNG; `std::shuffle` randomises the artifact pool each round in `playRound()`; `std::uniform_int_distribution<int> coin(0,4)` gives a ~20% chance of an authentic (unforged) pair in Master mode — all in `forgers_table.cpp` |
| **Data structures for storing data** | `struct Artifact` holds an ordered `vector<pair<string,string>>` of fields plus three `vector<ForgedField>` sets (one per difficulty); `struct QResult` stores per-question results for the end-of-round summary; `std::map<string,string>` built inside `displayArtifacts()` for O(1) forged-value lookup — all in `forgers_table.cpp` |
| **Dynamic memory management** | `std::vector<Artifact> arts` grown dynamically as `loadArtifacts()` parses the file; `pool` vector extended at runtime if the artifact bank is smaller than the question count; `std::thread thr(timerFn); thr.detach()` allocates a background timer thread dynamically in Master mode — all in `forgers_table.cpp` |
| **File input/output** | `loadArtifacts("forger_bank.txt")` reads the full artifact database line-by-line using `std::ifstream` (input); `saveScore()` appends a timestamped result (difficulty, score, best streak) to `save_data.txt` using `std::ofstream` in append mode (output) — both in `forgers_table.cpp` |
| **Program codes in multiple files** | `forgers_table.h` (function declaration for `runForgersTable()`), `forgers_table.cpp` (all game logic), `main.cpp` (Odyssey OS menu integration), `forger_bank.txt` (artifact database), `Makefile` |
| **Multiple Difficulty Levels** | Three levels selectable at runtime — Intern, Archivist, Master Curator — each with different lives, question counts, label visibility, timer presence, and forgery complexity; selected via `selectDifficulty()` and branched throughout `playRound()` in `forgers_table.cpp` |

---

### Difficulty Levels

| Difficulty | Lives | Questions | Field Labels | Forged Fields | Timer | Authentic Pairs |
|------------|-------|-----------|--------------|---------------|-------|-----------------|
| Intern (Easy) | 5 | 5 | ✅ Shown | 1 (obvious) | ❌ | No |
| Archivist (Medium) | 3 | 5 | ❌ Hidden | 1 (subtle) | ❌ | No |
| Master Curator (Hard) | 2 | 7 | ❌ Hidden | Up to 3 | ✅ 30s | ~20% chance |

---

### Scoring System

| Event | Points |
|-------|--------|
| Correct identification | +100 pts |
| Streak of 3+ correct | ×2 multiplier on all subsequent correct answers |
| Speed bonus (Master only, answer < 15s) | +50 pts |
| Life bonus (end of round) | +50 pts per life remaining |
| Wrong answer or timeout | −1 life, streak reset |

---

### Commands

| Input | Description |
|-------|-------------|
| `DATE` / `LOCATION` / `BUILDER` / `MATERIAL` / `STYLE` | Name the forged field (Intern & Archivist) |
| `FIELD1, FIELD2, ...` | Comma-separated list of forged fields (Master Curator) |
| `AUTHENTIC` | Declare that no fields are forged — both dossiers are genuine (Master Curator only) |
| `Q` or `QUIT` | Exit the game at any prompt and return to the Odyssey OS main menu |

---

### How to Play

1. Select a difficulty level from the menu (1 = Intern, 2 = Archivist, 3 = Master Curator).
2. Two classified dossiers appear side by side — **FILE A** (authentic) and **FILE B** (potentially tampered).
3. Compare the five fields: **DATE, LOCATION, BUILDER, MATERIAL, STYLE**.
4. Type the name of the field that has been forged and press ENTER.
   - In **Master Curator** mode, list all forged fields separated by commas (e.g. `DATE, BUILDER`).
   - If both dossiers are identical, type `AUTHENTIC`.
5. Receive feedback — correct answers earn points; wrong answers cost a life.
6. A historical debriefing note about the artifact is shown after each question.
7. Finish all questions (or survive on remaining lives) to see the **Case File Summary** with your final score.
8. Type `Q` at any prompt to return to the main menu immediately.

---

# App 6: The Curator’s Cryptex

## **Overview**

**The Curator’s Cryptex** is the final boss challenge of *Odyssey OS*, designed as a high-pressure, terminal-based trivia gauntlet. Acting as the system’s last line of defense, the Cryptex tests the player’s knowledge of world history, heritage sites, architecture, and cultural artifacts under strict time and life constraints.

Players must answer a sequence of trivia questions correctly while managing limited lives, hints, and a countdown timer. The experience is enhanced with animated system messages, a dynamic mascot (**Curio**), and escalating tension across difficulty levels.

---

## **Coding Elements Implementation**

| Requirement                          | Implementation Location                                                                                                                      |              |
| ------------------------------------ | -------------------------------------------------------------------------------------------------------------------------------------------- | ------------ |
| **Generation of random events**      | `shuffleQuestions()` randomly reorders questions using `rand()`; ensures each playthrough has a different sequence (`cryptex_boss.cpp`)      |              |
| **Data structures for storing data** | `struct TriviaQuestion` stores `difficulty`, `question`, `answer`, `hint`, and `bonusLife`; questions are stored in `vector<TriviaQuestion>` |              |
| **Dynamic memory management**        | STL containers (`vector`, `string`) dynamically manage memory for questions and user input                                                   |              |
| **File input/output**                | `loadQuestions()` reads from `cryptex_questions.txt` using `ifstream` and `stringstream` with `                                              | ` delimiters |
| **Program codes in multiple files**  | `cryptex_boss.h`, `cryptex_boss.cpp`, `main.cpp`, and `cryptex_questions.txt`                                                                |              |
| **Multiple Difficulty Levels**       | Easy, Medium, Hard modes implemented in `runCryptexBoss()` with different lives, time limits, and hints                                      |              |

---

## **Core Gameplay Features**

### 🎯 Trivia System

* Players answer up to **10 randomized questions**
* Questions are filtered by difficulty level
* Answers are validated using a **custom normalization system**

  * Case-insensitive
  * Ignores extra whitespace
  * Supports flexible matching (medium/hard)

---

### ⏱️ Time Pressure

* Global countdown timer per game
* Visual color feedback:

  * 🟢 Green → safe time
  * 🟡 Yellow → warning
  * 🔴 Red → critical

---

### ❤️ Lives System

* Incorrect answers reduce lives
* Game ends when lives reach 0
* Special **bonus life questions** reward extra lives

---

### 💡 Hint System

* Limited hints per difficulty
* Typing `"hint"` reveals contextual help
* Encourages strategic use

---

### 🎭 Mascot System (Curio)

A dynamic ASCII character that reacts to gameplay:

* Watching 👀
* Thinking 🤔
* Angry 😡
* Happy 😊
* Glitching ⚠️

Enhances immersion and feedback.

---

### 🎬 Terminal Animations

* Typing effects (`typeTextColor`)
* Loading sequences (`loadingDots`)
* Flash warnings (`flashWarning`)
* Countdown sequences for hard mode

---

## **Difficulty Levels**

| Difficulty                  | Lives | Time Limit | Hints | Matching System                               |
| --------------------------- | ----- | ---------- | ----- | --------------------------------------------- |
| **Easy (Training Curator)** | 5     | 107 sec    | 3     | Exact match (ignores case & whitespace)       |
| **Medium (Senior Curator)** | 3     | 87 sec     | 2     | Smart keyword matching (70%)                  |
| **Hard (Master Curator)**   | 2     | 63 sec     | 1     | Strict smart matching + sudden death pressure |

---

## **Answer Validation System**

The Cryptex uses a **multi-layered input handling system**:

### Easy Mode:

* Uses `normalizeAnswer()`
* Ignores:

  * Case differences
  * Extra spaces

### Medium & Hard:

* Uses `smartMatch()`
* Features:

  * Keyword-based validation
  * Ignores filler words (`the`, `of`, etc.)
  * Requires ≥70% keyword match

**Example:**

| User Input                | Correct Answer | Result          |
| ------------------------- | -------------- | --------------- |
| `great wall`              | Great Wall     | ✅               |
| `the great wall of china` | Great Wall     | ✅ (medium/hard) |
| `great`                   | Great Wall     | ❌               |

---

## **Bonus Mechanics**

### 🧬 Relic Bonus System

Certain questions grant:

* **+1 Life** when answered correctly
* Encourages risk-taking and rewards knowledge

---

### ⚠️ Sudden Death Mode (Hard)

* Triggered at start of hard mode
* Dramatic warnings and animations
* Minimal margin for error

---

## **How to Play**

1. Launch the Cryptex from the Odyssey OS main system
2. Enter a difficulty (`easy`, `medium`, `hard`)
3. Watch the system boot sequence
4. Answer each question by typing and pressing `ENTER`
5. Optional:

   * Type `hint` to use a hint
6. Survive all questions before:

   * Running out of lives
   * Running out of time

---
# Technologies Used 

## Core Technologies

| Technology | Purpose |
|------------|---------|
| **C++** | Main programming language |
| **STL (Standard Template Library)** | Data structures (vector, list, map, set, string) |
| **GCC/G++** | Compiler with `-std=c++17 -Wall -pthread` flags |
| **Make** | Build automation |
| **Git** | Version control |

## Libraries Used (All Standard)

| Library | Purpose | Used In |
|---------|---------|---------|
| `<iostream>` | Input/output operations | All files |
| `<fstream>` | File I/O for save/load | game.cpp, forgers_table.cpp, cryptex_boss.cpp |
| `<string>` | String manipulation | All files |
| `<vector>` | Dynamic arrays | puzzle.cpp, wordle.cpp, cryptex_boss.cpp |
| `<map>` | Key-value pairs | puzzle.cpp, forgers_table.cpp |
| `<set>` | Unique collections | puzzle.cpp |
| `<algorithm>` | Sorting, searching | puzzle.cpp, forgers_table.cpp, wordle.cpp |
| `<cmath>` | Mathematical functions | puzzle.cpp |
| `<cstdlib>` | Random number generation | All game files |
| `<ctime>` | Time-based seeding | All game files |
| `<chrono>` | Timing and animations | game.cpp, art.cpp, cryptex_boss.cpp |
| `<thread>` | Background timer threads | forgers_table.cpp, cryptex_boss.cpp |
| `<atomic>` | Thread-safe variables | forgers_table.cpp |
| `<random>` | Modern random generation | puzzle.cpp, forgers_table.cpp |
| `<iomanip>` | Formatted output | game.cpp, forgers_table.cpp |
| `<sstream>` | String stream parsing | forgers_table.cpp, cryptex_boss.cpp |
| `<cctype>` | Character checking | game.cpp, wordle.cpp |
| `<limits>` | Input stream clearing | main.cpp |

## ANSI Escape Sequences

| Code | Effect | Used In |
|------|--------|---------|
| `\033[31m` | Red text | All files |
| `\033[32m` | Green text | All files |
| `\033[33m` | Yellow text | All files |
| `\033[34m` | Blue text | All files |
| `\033[35m` | Magenta text | All files |
| `\033[36m` | Cyan text | All files |
| `\033[1m` | Bold text | All files |
| `\033[2m` | Dim text | main.cpp |
| `\033[2J\033[1;1H` | Clear screen | main.cpp |

# Installation 
How to run on linux server: 

1. git clone https://github.com/jitanimp4/COMP2113-Group-129.git
2. cd COMP2113-Group-129
3. make clean
4. make
5. ./odyssey_os
