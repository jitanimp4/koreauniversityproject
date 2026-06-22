#include "cryptex_boss.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <thread> //for my animations
#include <chrono> //for my animations
using namespace std;

//Color scheme

const string RESET  = "\033[0m";
const string PINK = "\033[95m";
const string RED    = "\033[31m";
const string GREEN  = "\033[32m";
const string YELLOW = "\033[33m";
const string BLUE   = "\033[34m";
const string MAGENTA= "\033[35m";
const string CYAN   = "\033[36m";
const string WHITE  = "\033[37m";
const string BRIGHT_RED   = "\033[91m";
const string BRIGHT_GREEN = "\033[92m";
const string BRIGHT_YELLOW= "\033[93m";
const string BRIGHT_CYAN  = "\033[96m";
const string BOLD   = "\033[1m";


// animations
void typeTextColor(string text, int delayMs, string color)
{
    cout << color;

    for (size_t i = 0; i < text.length(); i++)
    {
        cout << text[i] << flush;
        this_thread::sleep_for(chrono::milliseconds(delayMs));
    }

    cout << RESET << endl;
}

void loadingDots(string text, int steps, int delayMs)
{
    for (int i = 0; i < steps; i++)
    {
        int dots = i % 4;  // cycles 0,1,2,3

        cout << "\r" << text;

        for (int j = 0; j < dots; j++)
        {
            cout << ".";
        }

        cout << "   " << flush; // clears leftover dots

        this_thread::sleep_for(chrono::milliseconds(delayMs));
    }

    cout << "\r" << text << "..." << endl;
}

void countdown()
{
    for (int i = 3; i >= 1; i--)
    {
        cout << RED << BOLD << i << RESET << endl;
        this_thread::sleep_for(chrono::milliseconds(900));
    }
    cout << RED << BOLD << "BEGIN!" << RESET << endl;
}

void flashWarning(string text, string color, string reset, int times)
{
    for (int i = 0; i < times; i++)
    {
        cout << color << text << reset << "\r" << flush;
        this_thread::sleep_for(chrono::milliseconds(250));
        cout << string(text.length(), ' ') << "\r" << flush;
        this_thread::sleep_for(chrono::milliseconds(250));
    }
    cout << color << text << reset << endl;
}

void bootSequence()
{
    typeTextColor("Odyssey OS v1.0", 40, BOLD);
    typeTextColor("Initializing archive core...", 35, BOLD);
    cout << BRIGHT_CYAN;
    typeTextColor("Turning off the Oven...", 35, BOLD);
    cout << RESET;
    typeTextColor("Decrypting heritage records...", 35, BOLD);
    flashWarning("Access granted!", GREEN, RESET, 3);
}

//Change to lowercase
string toLowerCase(string text)
{
    for (size_t i = 0; i < text.length(); i++)
    {
        text[i] = tolower((unsigned char)text[i]);
    }
    return text;
}

//For whitespace problem
string trim(const string& str)
{
    size_t start = str.find_first_not_of(" \t\n\r");
    size_t end = str.find_last_not_of(" \t\n\r");

    if (start == string::npos) return "";

    return str.substr(start, end - start + 1);
}

string normalizeSpaces(const string& str)
{
    string result;
    bool inSpace = false;

    for (char c : str)
    {
        if (isspace(c))
        {
            if (!inSpace)
            {
                result += ' ';
                inSpace = true;
            }
        }
        else
        {
            result += c;
            inSpace = false;
        }
    }

    return result;
}

string normalizeAnswer(string text)
{
    text = toLowerCase(text);
    text = trim(text);
    text = normalizeSpaces(text);
    return text;
}

//For Multiple answer problem
vector<string> stopWords = {
    "the", "a", "an", "of", "in", "on", "at", "to", "for"
};

vector<string> splitWords(const string& text)
{
    vector<string> words;
    stringstream ss(text);
    string word;

    while (ss >> word)
    {
        words.push_back(word);
    }

    return words;
}

bool isStopWord(const string& word)
{
    vector<string> stopWords = {
        "the", "a", "an", "of", "in", "on", "at", "to", "for"
    };

    for (const string& w : stopWords)
    {
        if (word == w) return true;
    }
    return false;
}

bool smartMatch(string user, string correct)
{
    user = normalizeAnswer(user);
    correct = normalizeAnswer(correct);

    vector<string> userWords = splitWords(user);
    vector<string> correctWords = splitWords(correct);

    int totalKeywords = 0;
    int matchedKeywords = 0;

    for (const string& word : correctWords)
    {
        if (isStopWord(word)) continue;

        totalKeywords++;

        for (const string& u : userWords)
        {
            if (u == word)
            {
                matchedKeywords++;
                break;
            }
        }
    }

    // Require at least 70% of keywords to match
    if (totalKeywords == 0) return false;

    double ratio = (double)matchedKeywords / totalKeywords;

    return ratio >= 0.7;
}


//File Loading errors

vector<TriviaQuestion> loadQuestions(string filename, string mode)
{
    vector<TriviaQuestion> questions;
    ifstream fin(filename);

    if (!fin)
    {
        cout << "Error opening file: " << filename << endl;
        return questions;
    }

    string line;
    while (getline(fin, line))
    {
        stringstream ss(line);
        TriviaQuestion q;
        q.bonusLife = false;


        getline(ss, q.difficulty, '|');
        getline(ss, q.question, '|');
        getline(ss, q.answer, '|');
        getline(ss, q.hint, '|');

        if (q.question == "What famous island prison once held Al Capone?" || q.question == "Which kingdom in present-day Nigeria was known for its advanced bronze sculptures?"
                || q.question == "What fortress in India is known as the Red Fort?")
        {
            q.bonusLife = true;
        }

        if (q.difficulty == mode)
        {
            questions.push_back(q);
        }
    }

    fin.close();
    return questions;
}

//Changed Shuffle logic, this was too simple and not efficient
//void shuffleQuestions(vector<TriviaQuestion>& questions)
//{
//for (int i = 0; i < questions.size(); i++)
//{
//int r = rand() % questions.size();
//TriviaQuestion temp = questions[i];
//questions[i] = questions[r];
//questions[r] = temp;
//}
//}

void shuffleQuestions(vector<TriviaQuestion>& questions)
{
    if (questions.empty())
        return;

    for (int i = questions.size() - 1; i > 0; i--)
    {
        int r = rand() % (i + 1);

        TriviaQuestion temp = questions[i];
        questions[i] = questions[r];
        questions[r] = temp;
    }
}
//Mascot
void showMascotNormal()
{
    cout << PINK;
    cout << "   /\\_/\\\\ " << endl;
    cout << "  ( o.o )" << endl;
    cout << "   > ^ <   Curio is watching..." << endl;
    cout << RESET;
}

void showMascotAngry()
{
    cout << "   /\\_/\\\\ " << endl;
    cout << "  ( >.< )" << endl;
    cout << "   > ^ <  " << RED << "CURIO IS NOT PLEASED." << RESET << endl;
}

void showMascotSad()
{
    cout << "============================================" << endl;
    cout << BRIGHT_YELLOW;
    cout << "   /\\_/\\ " << endl;
    cout << "  ( T_T )" << endl;
    cout << "   > ^ <   " << RESET;
    typeTextColor("YOU FAILED CURIO *TSK *TSK", 70, RED);
    cout << "============================================" << endl;
}

void showMascotHappy()
{
    cout << "============================================" << endl;
    cout << PINK;
    cout << "   /\\_/\\ " << endl;
    cout << "  ( ^w^ )" << endl;
    cout << "   > ^ <  " << GREEN << "ARCHIVE RESTORED!" << RESET << endl;
    typeTextColor("Curio is happy with you UwU", 60, BOLD);
    cout << "============================================" << endl;
}

void showMascotSleeping()
{
    cout << "   /\\_/\\\\ " << endl;
    cout << "  ( -.- ) zZ" << endl;
    cout << "   > ^ <   Curio is waiting..." << endl;
}

void showMascotThinking()
{
    cout << BRIGHT_CYAN;
    cout << "   /\\_/\\\\ " << endl;
    cout << "  ( o~o )" << endl;
    cout << "   > ^ <   "<< RESET;
    typeTextColor("Curio is searching the archives...", 60, BOLD);
}

void showMascotGlitch()
{
    cout << "============================================" << endl;
    cout << RED;
    cout << "   /\\\\_/\\\\ " << endl;
    cout << "  ( x.x )" << endl;
    cout << "   > ^ <   ";
    typeTextColor("CURI0_//GL1TCH DETECTED", 60, RED);
    cout << RESET;
    cout << "============================================" << endl;
}

void showMascotAnimated()
{
    cout << PINK << "   /\\_/\\\\ " << endl;
    this_thread::sleep_for(chrono::milliseconds(120));
    cout << "  ( o.o )" << endl;
    this_thread::sleep_for(chrono::milliseconds(120));
    cout << "   > ^ <   Curio is watching..." << endl;
    cout << RESET;
}

void runCryptexBoss(string mode)
{
    mode = normalizeAnswer(mode);
    vector<TriviaQuestion> questions = loadQuestions("cryptex_questions.txt", mode);

    if (questions.size() == 0)
    {
        cout << "No questions found for this mode." << endl;
        return;
    }
    //srand(time(0)); //put this in main
    shuffleQuestions(questions);
    bootSequence();
    this_thread::sleep_for(chrono::milliseconds(500));
    time_t startTime = time(0);
    int timeLimit = 60; // seconds

    int lives;
    int score = 0;
    int totalQuestions = 10;
    int hintsLeft;

    if (mode == "easy")
    {
        lives = 5;
        timeLimit = 107;
        hintsLeft = 3;
    }
    else if (mode == "medium")
    {
        lives = 3;
        timeLimit = 87;
        hintsLeft = 2;
    }
    else
    {
        lives = 2;
        timeLimit = 63;
        hintsLeft = 1;
    }

    if ((int)questions.size() < totalQuestions)
    {
        totalQuestions = (int)questions.size();
    }

    showMascotSleeping();
    typeTextColor("Waking Curio...", 60, BRIGHT_CYAN);
    cout<<endl;
    cout << "==================================" << endl;
    cout << "     THE CURATOR'S CRYPTEX        " << endl;
    cout << "==================================" << endl;
    cout<<endl;
    showMascotAnimated();
    cout << BRIGHT_CYAN;
    loadingDots("Decrypting archive", 12, 250);
    cout << RESET;
    typeTextColor("[ARCHIVE STATUS] Final archive access initiated...", 40, CYAN);

    //Color Change Modes
    if (mode == "easy")
    {
        cout << "Mode: ";
        typeTextColor(mode, 40, GREEN);
    }
    else if (mode == "medium")
    {
        cout << "Mode: ";
        typeTextColor(mode, 40, YELLOW);
    }
    else
    {
        cout << "Mode: ";
        typeTextColor(mode, 40, RED);
    }
    if (lives == 1)
    {
        cout << "You have " << RED << lives << RESET << " life." << endl;
    }
    else
    {
        cout << "You have " << YELLOW << lives << RESET << " lives." << endl;
    }
    cout << "Answer " << totalQuestions << " questions." << endl << endl;
    cout << BLUE << "Need help? Type 'hint' for an easy cheat." << RESET << endl;

    if (mode == "hard")
    {
        //cout << BRIGHT_RED << BOLD;
        cout << endl;
        //cout << "==================================" << endl;
        flashWarning("SUDDEN DEATH MODE SELECTED!", RED, RESET, 3);
        //cout << "==================================" << RESET << endl;

        cout << RED << "[SECURITY NOTICE]" << RESET
             << " Errors will not be forgiven." << endl;
        //typeTextColor("Errors will not be FORGIVEN", 40, RED);

        showMascotGlitch();
        countdown();
    }

    for (int i = 0; i < totalQuestions && lives > 0; i++)
    {
        time_t now = time(0);

        if (now - startTime >= timeLimit)
        {
            cout << "[SYSTEM TIME BREACHED] Time is UP!" << endl;
            showMascotSad();
            break;
        }

        int remaining = timeLimit - (now - startTime);

        if( remaining > 10 )
        {
            if (remaining > 20)
            {
                cout << BRIGHT_GREEN << "Time left: " << remaining << " seconds" << RESET << endl;
            }
            else
            {
                cout << YELLOW << "Time left: " << remaining << " seconds" << RESET << endl;
            }
        }
        else
        {
            cout << RED << "Time left: " << remaining << " seconds" << RESET << endl;
        }

        cout << BOLD << "> Question " << i + 1 << ": " << questions[i].question << RESET << endl;
        cout << "Your answer: ";

        string userAnswer;
        getline(cin, userAnswer);

        time_t afterAnswer = time(0);
        if (afterAnswer - startTime >= timeLimit)
        {
            cout << RED << "[SYSTEM TIME BREACHED] Time is UP!" << RESET << endl;
            break;
        }

        if (userAnswer == "")
        {
            showMascotSleeping();
        }

        if (toLowerCase(userAnswer) == "hint")
        {
            showMascotThinking();
            if (hintsLeft > 0)
            {
                cout << CYAN << "Hint: " << questions[i].hint << RESET << endl;
                hintsLeft--;
                cout << BRIGHT_CYAN << "Hints left: " << hintsLeft << RESET << endl;
                cout << "Now enter your answer: ";
                getline(cin, userAnswer);
            }
            else
            {
                cout << RED << "No hints left." << RESET << endl;
                cout << "Enter your answer: ";
                getline(cin, userAnswer);
            }
        }

        if (smartMatch(userAnswer, questions[i].answer))
        {
            cout << endl;
            cout << GREEN << "[ACCESS STABLE] Correct!" << RESET << endl;
            score++;

            if (questions[i].bonusLife)
            {
                lives++;
                cout << endl;
                flashWarning("[RELIC BONUS] Extra life granted.", MAGENTA, RESET, 3);
            }
        }
        else
        {
            lives--;
            if (lives == 1)
            {
                //cout << RED << BOLD << "============================================" << endl;
                flashWarning("WARNING: Final life remaining!", RED, RESET, 3);
                //cout << "============================================" << RESET << endl;
                showMascotAngry();
            }
            cout << endl;
            flashWarning("[ARCHIVE BREACH!!!] Incorrect response.", RED, RESET, 3);
            cout << "Correct answer: " << questions[i].answer << endl;
            if (lives == 1)
            {
                cout << RED << "Lives left: " << lives << RESET << endl;
            }
            else
            {
                cout << YELLOW << "Lives left: " << lives << RESET << endl;
            }
        }


        cout << endl;
    }

    cout << "===== GAME OVER =====" << endl;
    cout << "Score: " << score << "/" << totalQuestions << endl;

    if (lives > 0)
    {
        showMascotHappy();
        flashWarning("You survived the Cryptex!", GREEN, RESET, 3);
        // cout<< GREEN << "You survived the Cryptex!" << endl;
    }
    else
    {
        showMascotSad();
        flashWarning("The Cryptex defeated you!", RED, RESET, 3);
        //cout << "The Cryptex defeated you." << endl;
    }
}
