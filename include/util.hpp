#pragma once

// =========================================================================
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

// =========================================================================
// 2. DATA STRUCTURES DEFINITIONS
// =========================================================================

struct userData
{
    std::string username;
    std::string date;
    int mood;
    int productivity;
    std::string note;
};

struct Account
{
    std::string username;
    std::string password;
};

// =========================================================================
// 3. LOW-LEVEL TERMINAL & OS UTILITIES (CROSS-PLATFORM)
// =========================================================================

int getCh();
int getKey();
void moveCursor(int row, int col);

// =========================================================================
// 4. STRING & DATETIME MANIPULATION UTILS
// =========================================================================

int boundInput(int val, int low, int high);
std::string trimSpaces(const std::string &src);
std::string nextToken(const std::string &line, std::size_t &position);
std::string getCurrentDate(); // Disinkronkan dengan penambahan fitur real-time date kemarin

// =========================================================================
// 5. UI COMPONENTS & RENDERING UTILS
// =========================================================================

void drawBorder(int startRow, int startCol, int height, int width);
void drawLifeLogLogoLogin(int startRow, int startCol);
void textField();
void drawOption(const std::string *arrOptions, int optionslength, int userChoose);
void drawOption(const string *arrOptions, int optionslength, int userChoose, int borderHeight);

// =========================================================================
// 6. MEMORY MANAGEMENT UTILS
// =========================================================================

userData *resizeRecords(userData *oldArr, int oldSize, int newSize);

// =========================================================================
// 7. AUTHENTICATION SYSTEMS
// =========================================================================

bool parseAccount(const string &line, Account &parsedData);
bool login(const std::string &username, const std::string &password);
bool usernameExists(const std::string &username);
bool writeAccount(const Account &account);

// =========================================================================
// 8. UI & MENUS FLOWS
// =========================================================================

bool loginMenu();
bool registerMenu();
void loginInterface(bool &stop);
void mainMenuInterface(bool &stop);

// =========================================================================
// 9. JOURNAL MANAGEMENT SYSTEM
// =========================================================================

bool parseLine(const std::string &line, userData &parsedData);
void readInt(int &num, int minValue, int maxValue);
void inputUserData(userData &journalBUffer);
userData *readFile(const std::string &filename, int &outCount);
userData *searchByUsername(const std::string &filename, const std::string &targetUsername, int &outCount);
void printUserJournal(const std::string &filename, const std::string &targetUsername);
bool writeFile(const std::string &filename, const userData &data);

constexpr int NIM_KEY[] = {67, 64, 100, 104, 129, 81, 29};
constexpr int KEY_LENGTH = 7;

string encryptPassword(const string &plain);

// MENU JURNAL HARIAN
void jurnalHarian();
// sub menu JURNAL HARIAN
void tulisJurnal();
void tampilkanSemuaJurnal();
void cariJurnal();
void hapusJurnal();

// MENU LAINNYA
void statistik();
void achievement();
void profilAkun();
void about();


