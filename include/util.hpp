#pragma once

// 1. HEADERS, MACROS, AND NAMESPACE DECLARATIONS
// =========================================================================
#include <iostream>
#include <string>
#include <fstream>
#include <ctime>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

#include "../include/util.hpp"

#define BORDER_ROW 3
#define BORDER_COLUMN 42
#define DELIM "<|>"
#define DELIM_LEN 3

#define RESET_COLOR "\033[0m"
#define GREEN "\033[32m"
#define CYAN "\033[36m"
#define YELLOW "\033[93m"
#define RED "\033[31m"
#define CLEAR_SCREEN cout << "\033[2J\033[H"
#define HIDE_CURSOR cout << "\033[?25l"
#define SHOW_CURSOR cout << "\033[?25h"

using std::cin;
using std::cout;
using std::getline;
using std::ifstream;
using std::ios;
using std::ofstream;
using std::size_t;
using std::stoi;
using std::string;

// 2. CONSTANTS FOR ENCRYPTION KEY
// =========================================================================
constexpr int NIM_KEY[] = {67, 64, 100, 104, 129, 81, 29};
constexpr int KEY_LENGTH = 7;

// 3. STRUCT DEFINITIONS AND GLOBAL VARIABLE
// =========================================================================

struct userData
{
    string username;
    string date;
    int mood;
    int productivity;
    string note;
};

struct Account
{
    string username;
    string password;
};

// 4. FUNCTION PROTOTYPES
// =========================================================================

// Low-Level Terminal Utilities
int getCh();
int getKey();
void moveCursor(int row, int col);

// String & Datetime Manipulations
int boundInput(int val, int low, int high);
string trimSpaces(const string &src);
string nextToken(const string &line, size_t &position);
string getCurrentDate();

// UI Components & Rendering
void drawBorder(int startRow, int startCol, int height, int width);
void drawLifeLogLogoLogin(int startRow, int startCol);
void mainMenuInterface(bool &stop);
void textField();
void drawOption(const string *arrOptions, int optionslength, int userChoose, int borderHeight);

// Memory Management
userData *resizeRecords(userData *oldArr, int oldSize, int newSize);

// Authentication System
bool parseAccount(const string &line, Account &parsedData);
bool login(const string &username, const string &password);
bool usernameExists(const string &username);
bool writeAccount(const Account &account);
string encryptPassword(const string &plain);

// UI & Menus Flows
bool loginMenu();
bool registerMenu();
void loginInterface(bool &stop);

//  Data I/O Management System
bool parseLine(const string &line, userData &parsedData);
void readInt(int &num, int minValue, int maxValue);
userData *readFile(const string &filename, int &outCount);

// Journal Search Functions
userData *searchByUsername(const string &filename, const string &targetUsername, int &outCount); // Penyebab Linker Error Kemarin
userData *searchByDate(const string &filename, const string &targetDate, int &outCount);
userData *searchByMoodCategory(const string &filename, int categoryOption, int &outCount);
userData *searchByProductivity(const string &filename, int targetProd, int &outCount);
userData *searchByKeyword(const string &filename, const string &keyword, int &outCount);

// Journal Actions & Menus
void searchMenu(const string &filename);
void printUserJournal(const string &filename, const string &targetUsername);
bool writeFile(const string &filename, const userData &data);
bool deleteJournal(const string &filename, const string &targetUsername, int journalNumber);
void printAllJournal(const string &filename);

// Application Screens
void jurnalHarian();
void tulisJurnal();
void tampilkanSemuaJurnal();
void cariJurnal();
void hapusJurnal();

void statistik();
void achievement();
void profilAkun();
void about();
