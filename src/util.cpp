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
using std::endl;
using std::getline;
using std::ifstream;
using std::ios;
using std::ofstream;
using std::size_t;
using std::stoi;
using std::string;

// =========================================================================
// 2. LOW-LEVEL TERMINAL & OS UTILITIES (CROSS-PLATFORM)
// =========================================================================

int getCh()
{
#ifdef _WIN32
    return getch();
#else
    struct termios asli, ubah;
    tcgetattr(STDIN_FILENO, &asli);
    ubah = asli;

    ubah.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &ubah);

    int karakter = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &asli);
    return karakter;
#endif
}

int getKey()
{
    int key = getCh();
#ifdef _WIN32
    if (key == 0 || key == 0xE0)
    {
        key = getCh();
        return key;
    }
#else
    if (key == 27)
    {
        getCh();
        char direction = getCh();
        if (direction == 'A')
            return 72;
        if (direction == 'B')
            return 80;
    }
    if (key == 10)
        return 13;
#endif
    return key;
}

void moveCursor(int row, int col)
{
    cout << "\033[" << row << ";" << col << "H";
}

void clearInputBuffer()
{
    cin.ignore(1000000, '\n');
}

// =========================================================================
// 3. STRING & DATETIME MANIPULATION UTILS
// =========================================================================
int boundInput(int val, int low, int high)
{
    if (val < low)
        return low;
    if (val > high)
        return high;
    return val;
    cout << " ";
}

string trimSpaces(const string &src)
{
    int start = 0;
    int end = (int)src.size() - 1;

    while (start <= end && src[start] == ' ')
        start++;
    while (end >= start && src[end] == ' ')
        end--;

    if (start > end)
        return "";
    return src.substr(start, end - start + 1);
}

string nextToken(const string &line, size_t &position)
{
    size_t found = line.find(DELIM, position);
    if (found == string::npos)
        return "";
    string token = line.substr(position, found - position);
    position = found + DELIM_LEN;
    return token;
}

string getCurrentDate()
{
    time_t now = time(0);
    tm *localTime = localtime(&now);
    char dateFormatted[11];

    strftime(dateFormatted, sizeof(dateFormatted), "%Y-%m-%d", localTime);

    return dateFormatted;
}

// =========================================================================
// 4. UI COMPONENTS & RENDERING UTILS
// =========================================================================

void drawBorder(int startRow, int startCol, int height, int width)
{
    for (int i = 1; i <= height; i++)
    {
        moveCursor(startRow + i, startCol);
        for (int j = 1; j <= width; j++)
        {
            if (i == 1 || i == height)
            {
                if (j == 1)
                    cout << "||";
                else if (j == width)
                    cout << "||";
                else
                    cout << "=";
            }

            else
            {
                if (j == 1)
                    cout << "||";
                else if (j == width)
                    cout << "||";
                else
                    cout << " ";
            }
        }
        cout << "\n";
    }
}

void drawLifeLogLogoLogin(int startRow, int startCol)
{
    moveCursor(startRow, startCol);
    cout << " _     _  __     _      ";
    moveCursor(startRow + 1, startCol);
    cout << "| |   (_)/ _|   | |   ";
    moveCursor(startRow + 2, startCol);
    cout << "| |    _| |_ ___| |     ___   __ _ ";
    moveCursor(startRow + 3, startCol);
    cout << "| |   | |  _/ _ \\ |    / _ \\ / _` |";
    moveCursor(startRow + 4, startCol);
    cout << "| |___| | ||  __/ |___| (_) | (_| |";
    moveCursor(startRow + 5, startCol);
    cout << "\\_____/_|_| \\___\\_____/\\___/ \\__, |";
    moveCursor(startRow + 6, startCol);
    cout << "                              __/ |";
    moveCursor(startRow + 7, startCol);
    cout << "                             |___/";
}

void mainMenuInterface(bool &stop)
{
    int menuChoice;
    const string mainMenuOption[] = {"JURNAL HARIAN", "STATISTIK", "ACHIEVEMENT", "PROFIL AKUN", "ABOUT", "EXIT"};
    const int mainMenuOptionLength = 6;

    CLEAR_SCREEN;
    HIDE_CURSOR;

    menuChoice = 0;
    bool running = true;
    while (running)
    {
        CLEAR_SCREEN;
        drawOption(mainMenuOption, mainMenuOptionLength, menuChoice, 35);

        int key = getKey();
        switch (key)
        {
        case 72:
            menuChoice = (menuChoice == 0) ? mainMenuOptionLength - 1 : menuChoice - 1;
            break;
        case 80:
            menuChoice = (menuChoice == mainMenuOptionLength - 1) ? 0 : menuChoice + 1;
            break;
        case 13:
            switch (menuChoice)
            {
            case 0:
                jurnalHarian();
                break;
            case 1:
                statistik();
                break;
            case 2:
                achievement();
                break;
            case 3:
                profilAkun();
                break;
            case 4:
                about();
                break;
            case 5:
                stop = true;
                SHOW_CURSOR;
                CLEAR_SCREEN;
                return;
            }
        }
    }
}

void textField()
{
    moveCursor(BORDER_ROW + 11, 53);
    cout << "Username:";
    moveCursor(BORDER_ROW + 12, 52);
    cout << "+~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+";
    moveCursor(BORDER_ROW + 13, 52);
    cout << "|                                                         |";
    moveCursor(BORDER_ROW + 14, 52);
    cout << "+~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+";
    moveCursor(BORDER_ROW + 16, 53);
    cout << "Password:";
    moveCursor(BORDER_ROW + 17, 52);
    cout << "+~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+";
    moveCursor(BORDER_ROW + 18, 52);
    cout << "|                                                         |";
    moveCursor(BORDER_ROW + 19, 52);
    cout << "+~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+";
}

void drawOption(const string *arrOptions, int optionslength, int userChoose, int borderHeight)
{
    drawBorder(BORDER_ROW, BORDER_COLUMN, borderHeight, 80);
    drawLifeLogLogoLogin(BORDER_ROW + 2, BORDER_COLUMN + 23);
    int buttonrow = BORDER_ROW + 11, buttonColumn = BORDER_COLUMN + 26;
    for (int i = 0; i < optionslength; i++)
    {
        if (i == userChoose)
            cout << CYAN;
        else
            cout << RESET_COLOR;

        int padTotal = 26;
        int labelLen = arrOptions[i].length();
        int padLeft = (padTotal - labelLen) / 2;
        int padRight = padTotal - labelLen - padLeft;

        moveCursor(buttonrow, buttonColumn);
        cout << "============================";
        moveCursor(buttonrow + 1, buttonColumn);
        cout << "|" << string(padLeft, ' ') << arrOptions[i] << string(padRight, ' ') << "|";
        moveCursor(buttonrow + 2, buttonColumn);
        cout << "============================";

        buttonrow += 4;
    }
    cout << RESET_COLOR;
}

// =========================================================================
// 5. MEMORY MANAGEMENT UTILS
// =========================================================================

userData *resizeRecords(userData *oldArr, int oldSize, int newSize)
{
    userData *newArr = new userData[newSize];
    for (int idx = 0; idx < oldSize; idx++)
    {
        newArr[idx] = oldArr[idx];
    }
    delete[] oldArr;
    return newArr;
}

// =========================================================================
// 6. AUTHENTICATION SYSTEM
// =========================================================================

bool parseAccount(const string &line, Account &parsedData)
{
    if (line.empty())
        return false;

    size_t pos = 0;
    parsedData.username = nextToken(line, pos);
    parsedData.password = line.substr(pos);

    if (parsedData.username.empty() || parsedData.password.empty())
        return false;
    return true;
}

bool login(const string &username, const string &password)
{
    ifstream file("data/userAccounts.txt");
    if (!file.is_open())
        return false;

    string line;
    while (true)
    {
        getline(file, line);
        if (file.fail())
            break;

        Account account;
        if (!parseAccount(line, account))
            continue;

        if (account.username == username && account.password == encryptPassword(password))
        {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

bool usernameExists(const string &username)
{
    ifstream file("data/userAccounts.txt");
    if (!file.is_open())
        return false;

    string line;
    while (true)
    {
        getline(file, line);
        if (file.fail())
            break;

        Account account;
        if (!parseAccount(line, account))
            continue;

        if (account.username == username)
        {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

bool writeAccount(const Account &account)
{
    ofstream file("data/userAccounts.txt", ios::app);
    if (!file.is_open())
        return false;

    file << account.username << DELIM << encryptPassword(account.password) << "\n";
    bool success = !file.fail();
    file.close();
    return success;
}

// =========================================================================
// 7. UI & MENUS FLOWS
// =========================================================================
string currentUser;

bool loginMenu()
{
    CLEAR_SCREEN;
    drawBorder(BORDER_ROW, BORDER_COLUMN, 22, 80);
    drawLifeLogLogoLogin(BORDER_ROW + 2, BORDER_COLUMN + 24);
    moveCursor(BORDER_ROW + 9, 73);
    cout << "===== LOGIN =====\n\n";
    textField();

    SHOW_CURSOR;
    string username, password;

    moveCursor(BORDER_ROW + 13, 53);
    getline(cin, username);
    moveCursor(BORDER_ROW + 18, 53);
    getline(cin, password);

    username = trimSpaces(username);
    password = trimSpaces(password);

    if (login(username, password))
    {
        currentUser = username;
        moveCursor(BORDER_ROW + 20, 53);
        cout << GREEN << "Login berhasil." << RESET_COLOR;
        moveCursor(BORDER_ROW + 21, 53);
        cout << "PRESS ANY KEY TO CONTINUE...";
        HIDE_CURSOR;
        getCh();
        return true;
    }
    else
    {
        moveCursor(BORDER_ROW + 20, 53);
        cout << RED << "Akun anda tidak Ditemukan / Username atau password salah." << RESET_COLOR;
        moveCursor(BORDER_ROW + 21, 53);
        cout << "PRESS ANY KEY TO RETURN...";
        HIDE_CURSOR;
        getCh();
        return false;
    }
}

bool registerMenu()
{
    CLEAR_SCREEN;
    drawBorder(BORDER_ROW, BORDER_COLUMN, 22, 80);
    drawLifeLogLogoLogin(BORDER_ROW + 2, BORDER_COLUMN + 24);
    moveCursor(BORDER_ROW + 9, 71);
    cout << "===== REGISTER =====\n\n";
    textField();

    Account account;
    SHOW_CURSOR;
    moveCursor(BORDER_ROW + 13, 53);
    getline(cin, account.username);
    moveCursor(BORDER_ROW + 18, 53);
    getline(cin, account.password);

    account.username = trimSpaces(account.username);
    account.password = trimSpaces(account.password);

    if (account.username.empty() || account.password.empty())
    {
        moveCursor(BORDER_ROW + 20, 53);
        cout << RED << "Data tidak boleh kosong." << RESET_COLOR;
        moveCursor(BORDER_ROW + 21, 53);
        cout << "PRESS ANY KEY TO RETURN...";
        HIDE_CURSOR;
        getCh();
        return false;
    }
    else if (usernameExists(account.username))
    {
        moveCursor(BORDER_ROW + 20, 53);
        cout << RED << "Username sudah digunakan." << RESET_COLOR;
        moveCursor(BORDER_ROW + 21, 53);
        cout << "PRESS ANY KEY TO RETURN...";
        HIDE_CURSOR;
        getCh();
        return false;
    }
    else if (writeAccount(account))
    {
        currentUser = account.username;
        moveCursor(BORDER_ROW + 20, 53);
        cout << GREEN << "Register berhasil." << RESET_COLOR;
        moveCursor(BORDER_ROW + 21, 53);
        cout << "PRESS ANY KEY TO CONTINUE...";
        HIDE_CURSOR;
        getCh();
        return true;
    }
    else
    {
        moveCursor(BORDER_ROW + 20, 53);
        cout << RED << "Register gagal." << RESET_COLOR;
        moveCursor(BORDER_ROW + 21, 53);
        cout << "PRESS ANY KEY TO RETURN...";
        HIDE_CURSOR;
        getCh();
        return false;
    }
}

void loginInterface(bool &stop)
{
    int menuChoice;
    const string loginOption[] = {"Login", "Register", "Exit"};
    const int loginOptionLength = 3;

    CLEAR_SCREEN;
    HIDE_CURSOR;

    menuChoice = 0;
    bool running = true;
    while (running)
    {
        CLEAR_SCREEN;
        drawOption(loginOption, loginOptionLength, menuChoice, 23);

        int key = getKey();
        switch (key)
        {
        case 72:
            menuChoice = (menuChoice == 0) ? loginOptionLength - 1 : menuChoice - 1;
            break;
        case 80:
            menuChoice = (menuChoice == loginOptionLength - 1) ? 0 : menuChoice + 1;
            break;
        case 13:
            switch (menuChoice)
            {
            case 0:
                if (loginMenu())
                {
                    return;
                }
                break;
            case 1:
                if (registerMenu())
                {
                    return;
                }
                break;
            case 2:
                stop = true;
                SHOW_CURSOR;
                CLEAR_SCREEN;
                return;
            }
        }
    }
}

// =========================================================================
// 8. JOURNAL MANAGEMENT SYSTEM
// =========================================================================

bool parseLine(const string &line, userData &parsedData)
{
    if (line.empty())
        return false;

    size_t position = 0;
    string tokens[5];

    for (int idx = 0; idx < 4; idx++)
    {
        tokens[idx] = nextToken(line, position);
    }
    tokens[4] = line.substr(position);

    if (tokens[0].empty() || tokens[1].empty() || tokens[4].empty())
        return false;

    parsedData.username = tokens[0];
    parsedData.date = tokens[1];
    parsedData.mood = boundInput(stoi(tokens[2]), 1, 5);
    parsedData.productivity = boundInput(stoi(tokens[3]), 1, 10);
    parsedData.note = tokens[4];

    return true;
}

void readInt(int &num, int minValue, int maxValue, const string instruction)
{
    while (true)
    {
        cout << instruction;
        cin >> num;
        if (cin.fail() || cin.peek() != '\n')
        {
            cout << RED << "  [!] Input tidak valid, masukkan angka bulat." << RESET_COLOR << "\n";
            cin.clear();
            clearInputBuffer();
            continue;
        }
        if (num < minValue)
        {
            num = minValue;
            cout << "  [!] Terlalu kecil, di-set ke " << minValue << ".\n";
        }
        else if (num > maxValue)
        {
            num = maxValue;
            cout << "  [!] Terlalu besar, di-set ke " << maxValue << ".\n";
        }
        break;
    }
}

userData *readFile(const string &filename, int &outCount)
{
    ifstream fileStream(filename);
    if (!fileStream.is_open())
    {
        cout << RED << "[readFile] Tidak bisa membuka '" << filename << "'" << RESET_COLOR << "\n";
        outCount = -1;
        return nullptr;
    }

    int capacity = 8;
    int recordCount = 0;
    userData *records = new userData[capacity];
    string currentLine;

    while (getline(fileStream, currentLine))
    {
        if (currentLine.empty())
            continue;

        if (recordCount == capacity)
        {
            capacity *= 2;
            records = resizeRecords(records, recordCount, capacity);
        }

        if (parseLine(currentLine, records[recordCount]))
            recordCount++;
        else
            cout << RED << "[readFile] Baris dilewati (format salah): " << currentLine << RESET_COLOR << "\n";
    }

    fileStream.close();
    outCount = recordCount;
    return records;
}

userData *searchByUsername(const string &filename, const string &targetUsername, int &outCount)
{
    ifstream fileStream(filename);
    if (!fileStream.is_open())
    {
        cout << RED << "[searchByUsername] Tidak bisa membuka '" << filename << "'" << RESET_COLOR << "\n";
        outCount = -1;
        return nullptr;
    }

    int capacity = 8;
    int matchCount = 0;
    userData *matchedRecords = new userData[capacity];
    string currentLine;

    while (getline(fileStream, currentLine))
    {
        if (currentLine.empty())
            continue;

        userData parsedRecord;
        if (!parseLine(currentLine, parsedRecord))
            continue;

        if (parsedRecord.username == targetUsername)
        {
            if (matchCount == capacity)
            {
                capacity *= 2;
                matchedRecords = resizeRecords(matchedRecords, matchCount, capacity);
            }
            matchedRecords[matchCount++] = parsedRecord;
        }
    }

    fileStream.close();
    outCount = matchCount;
    return matchedRecords;
}

userData *searchByDate(const string &filename, const string &targetDateRaw, int &outCount)
{
    string targetDate = trimSpaces(targetDateRaw);

    ifstream fileStream(filename);
    if (!fileStream.is_open())
    {
        cout << RED << "[searchByDate] Tidak bisa membuka '" << filename << "'" << RESET_COLOR << "\n";
        outCount = -1;
        return nullptr;
    }

    int capacity = 8;
    int matchCount = 0;
    userData *matchedRecords = new userData[capacity];
    string currentLine;

    while (getline(fileStream, currentLine))
    {
        if (currentLine.empty())
            continue;

        userData parsedRecord;
        if (!parseLine(currentLine, parsedRecord))
            continue;

        if (parsedRecord.username == currentUser && parsedRecord.date == targetDate)
        {
            if (matchCount == capacity)
            {
                capacity *= 2;
                matchedRecords = resizeRecords(matchedRecords, matchCount, capacity);
            }
            matchedRecords[matchCount++] = parsedRecord;
        }
    }
    fileStream.close();
    outCount = matchCount;
    return matchedRecords;
}

userData *searchByMoodCategory(const string &filename, int categoryOption, int &outCount)
{
    ifstream fileStream(filename);
    if (!fileStream.is_open())
    {
        cout << RED << "[searchByMoodCategory] Tidak bisa membuka '" << filename << "'" << RESET_COLOR << "\n";
        outCount = -1;
        return nullptr;
    }

    int capacity = 8;
    int matchCount = 0;
    userData *matchedRecords = new userData[capacity];
    string currentLine;

    while (getline(fileStream, currentLine))
    {
        if (currentLine.empty())
            continue;

        userData parsedRecord;
        if (!parseLine(currentLine, parsedRecord))
            continue;

        bool isMatch = false;
        if (parsedRecord.username == currentUser)
        {
            if (categoryOption == 1 && parsedRecord.mood < 3)
                isMatch = true;
            if (categoryOption == 2 && parsedRecord.mood == 3)
                isMatch = true;
            if (categoryOption == 3 && parsedRecord.mood > 3)
                isMatch = true;
        }

        if (isMatch)
        {
            if (matchCount == capacity)
            {
                capacity *= 2;
                matchedRecords = resizeRecords(matchedRecords, matchCount, capacity);
            }
            matchedRecords[matchCount++] = parsedRecord;
        }
    }
    fileStream.close();
    outCount = matchCount;
    return matchedRecords;
}

userData *searchByProductivity(const string &filename, int targetProd, int &outCount)
{
    ifstream fileStream(filename);
    if (!fileStream.is_open())
    {
        cout << RED << "[searchByProductivity] Tidak bisa membuka '" << filename << "'" << RESET_COLOR << "\n";
        outCount = -1;
        return nullptr;
    }

    int capacity = 8;
    int matchCount = 0;
    userData *matchedRecords = new userData[capacity];
    string currentLine;

    while (getline(fileStream, currentLine))
    {
        if (currentLine.empty())
            continue;

        userData parsedRecord;
        if (!parseLine(currentLine, parsedRecord))
            continue;

        if (parsedRecord.username == currentUser && parsedRecord.productivity == targetProd)
        {
            if (matchCount == capacity)
            {
                capacity *= 2;
                matchedRecords = resizeRecords(matchedRecords, matchCount, capacity);
            }
            matchedRecords[matchCount++] = parsedRecord;
        }
    }
    fileStream.close();
    outCount = matchCount;
    return matchedRecords;
}

userData *searchByKeyword(const string &filename, const string &keywordRaw, int &outCount)
{
    string keyword = trimSpaces(keywordRaw);

    ifstream fileStream(filename);
    if (!fileStream.is_open())
    {
        cout << RED << "[searchByKeyword] Tidak bisa membuka '" << filename << "'" << RESET_COLOR << "\n";
        outCount = -1;
        return nullptr;
    }

    int capacity = 8;
    int matchCount = 0;
    userData *matchedRecords = new userData[capacity];
    string currentLine;

    while (getline(fileStream, currentLine))
    {
        if (currentLine.empty())
            continue;

        userData parsedRecord;
        if (!parseLine(currentLine, parsedRecord))
            continue;

        if (parsedRecord.username == currentUser && parsedRecord.note.find(keyword) != string::npos)
        {
            if (matchCount == capacity)
            {
                capacity *= 2;
                matchedRecords = resizeRecords(matchedRecords, matchCount, capacity);
            }
            matchedRecords[matchCount++] = parsedRecord;
        }
    }
    fileStream.close();
    outCount = matchCount;
    return matchedRecords;
}

void cariJurnal(const string &filename)
{
    int menuChoice = 0;
    const string searchMenuOption[] = {
        "Cari Berdasarkan Tanggal", 
        "Cari Berdasarkan Mood ", 
        "Cari Produktivitas", 
        "Cari Berdasarkan Kata", 
        "Kembali"
    };
    const int searchMenuOptionLength = 5;

    bool running = true;
    while (running)
    {
        CLEAR_SCREEN;
        HIDE_CURSOR;
        
        drawOption(searchMenuOption, searchMenuOptionLength, menuChoice, 35);
        const string title = "Cari Jurnal";
        int titlePad = (28 - (int)title.length()) / 2;
        moveCursor(BORDER_ROW + 10, BORDER_COLUMN + 26 + titlePad);
        cout << CYAN << title << RESET_COLOR;

        int key = getKey();
        switch (key)
        {
        case 72:
            menuChoice = (menuChoice == 0) ? searchMenuOptionLength - 1 : menuChoice - 1;
            break;
        case 80:
            menuChoice = (menuChoice == searchMenuOptionLength - 1) ? 0 : menuChoice + 1;
            break;
        case 13:
        {
            SHOW_CURSOR;

            userData *matchedRecords = nullptr;
            int matchCount = 0;
            bool processSearch = (menuChoice != 4);

            if (processSearch)
            {
                CLEAR_SCREEN;
                drawBorder(BORDER_ROW, BORDER_COLUMN, 22, 80);
                drawLifeLogLogoLogin(BORDER_ROW + 2, BORDER_COLUMN + 24);
            }

            switch (menuChoice)
            {
            case 0: 
            {
                moveCursor(BORDER_ROW + 9, 68);
                cout << "===== CARI BERDASARKAN TANGGAL =====\n";

                moveCursor(BORDER_ROW + 11, 53);
                cout << "Tanggal (YYYY-MM-DD):";
                moveCursor(BORDER_ROW + 12, 53);
                cout << "+~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+";
                moveCursor(BORDER_ROW + 13, 53);
                cout << "|                                                          |";
                moveCursor(BORDER_ROW + 14, 53);
                cout << "+~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+";

                SHOW_CURSOR;
                moveCursor(BORDER_ROW + 13, 54);
                string targetDate;
                clearInputBuffer();
                getline(cin, targetDate);
                HIDE_CURSOR;

                matchedRecords = searchByDate(filename, targetDate, matchCount);
                break;
            }
            case 1: 
            {
                moveCursor(BORDER_ROW + 9, 71);
                cout << "===== CARI BERDASARKAN MOOD =====\n";
                moveCursor(BORDER_ROW + 11, 53);
                cout << "Pilih Kategori Mood:";
                moveCursor(BORDER_ROW + 12, 53);
                cout << "1. Mood Buruk (< 3)";
                moveCursor(BORDER_ROW + 13, 53);
                cout << "2. Mood Biasa (== 3)";
                moveCursor(BORDER_ROW + 14, 53);
                cout << "3. Mood Bagus (> 3)";
                moveCursor(BORDER_ROW + 16, 53);
                int moodChoice;
                readInt(moodChoice, 1, 3, "Pilihan Kategori (1-3): ");
                matchedRecords = searchByMoodCategory(filename, moodChoice, matchCount);
                break;
            }
            case 2: 
            {
                moveCursor(BORDER_ROW + 9, 67);
                cout << "===== CARI BERDASARKAN PRODUKTIVITAS =====\n";
                moveCursor(BORDER_ROW + 11, 53);
                int targetProd;
                readInt(targetProd, 1, 10, "Masukkan produktivitas (1-10): ");
                matchedRecords = searchByProductivity(filename, targetProd, matchCount);
                break;
            }
            case 3: 
            {
                moveCursor(BORDER_ROW + 9, 70);
                cout << "===== CARI BERDASARKAN KATA =====\n";

                moveCursor(BORDER_ROW + 11, 53);
                cout << "Keyword:";
                moveCursor(BORDER_ROW + 12, 53);
                cout << "+~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+";
                moveCursor(BORDER_ROW + 13, 53);
                cout << "|                                                          |";
                moveCursor(BORDER_ROW + 14, 53);
                cout << "+~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+";

                SHOW_CURSOR;
                moveCursor(BORDER_ROW + 13, 54);
                string keyword;
                clearInputBuffer();
                getline(cin, keyword);
                HIDE_CURSOR;

                matchedRecords = searchByKeyword(filename, keyword, matchCount);
                break;
            }
            case 4: 
                running = false;
                break;
            }

            if (processSearch)
            {
                SHOW_CURSOR;

                if (matchCount < 0)
                {
                    moveCursor(BORDER_ROW + 18, 53);
                    cout << RED << "[!] Gagal membuka file atau file tidak ditemukan." << RESET_COLOR;
                    moveCursor(BORDER_ROW + 20, 53);
                    cout << "PRESS ANY KEY TO RETURN...";
                    HIDE_CURSOR;
                    getCh();
                }
                else if (matchCount == 0)
                {
                    moveCursor(BORDER_ROW + 18, 53);
                    cout << "Data tidak ditemukan.";
                    moveCursor(BORDER_ROW + 20, 53);
                    cout << "PRESS ANY KEY TO RETURN...";
                    HIDE_CURSOR;
                    getCh();
                }
                else
                {
                    CLEAR_SCREEN;
                    cout << "\n\"" << matchCount << " Jurnal Ditemukan\"\n";
                    for (int i = 0; i < matchCount; i++)
                    {
                        cout << "\n--- Journal " << (i + 1) << " ---\n";
                        cout << "Date        : " << matchedRecords[i].date << "\n";
                        cout << "Mood        : " << matchedRecords[i].mood << "\n";
                        cout << "Productivity: " << matchedRecords[i].productivity << "\n";
                        cout << "Note        : " << matchedRecords[i].note << "\n";
                    }
                    cout << "\nPRESS ANY KEY TO RETURN...";
                    HIDE_CURSOR;
                    getCh();
                }
                delete[] matchedRecords;
            }
            break;
        }
        }
    }
}
void printUserJournal(const string &filename)
{
    int matchCount = 0;
    userData *matchedRecords =
        searchByUsername(filename, currentUser, matchCount);

    if (matchCount < 0)
        return;

    if (matchCount == 0)
    {
        cout << "Anda belum memiliki jurnal.\n";
    }
    else
    {
        cout << "\n=== Journal milik " << currentUser << " ===\n";

        for (int idx = 0; idx < matchCount; idx++)
        {
            cout << "\n--- Journal " << idx + 1 << " ---\n";
            cout << "Date        : "
                 << matchedRecords[idx].date << "\n";

            cout << "Mood        : "
                 << matchedRecords[idx].mood << "\n";

            cout << "Productivity: "
                 << matchedRecords[idx].productivity << "\n";

            cout << "Note        : "
                 << matchedRecords[idx].note << "\n";
        }
    }

    delete[] matchedRecords;
}

bool writeFile(const string &filename, const userData &data)
{
    int existingCount = 0;
    userData *existingRecords = readFile(filename, existingCount);
    if (existingCount < 0) existingCount = 0; // file belum ada = anggap kosong

    ofstream fileStream(filename, ios::trunc);
    if (!fileStream.is_open())
    {
        cout << RED << "[writeFile] Tidak bisa membuka '" << filename << "'" << RESET_COLOR << "\n";
        delete[] existingRecords;
        return false;
    }

    for (int i = 0; i < existingCount; i++)
    {
        fileStream << existingRecords[i].username << DELIM
                   << existingRecords[i].date << DELIM
                   << existingRecords[i].mood << DELIM
                   << existingRecords[i].productivity << DELIM
                   << existingRecords[i].note << "\n";
    }
    fileStream << data.username << DELIM
               << data.date << DELIM
               << data.mood << DELIM
               << data.productivity << DELIM
               << data.note << "\n";

    bool success = !fileStream.fail();
    fileStream.close();
    delete[] existingRecords;
    return success;
}

bool deleteJournal(const string &filename, const string &targetUsername, int journalNumber)
{
    int totalCount = 0;
    userData *allRecords = readFile(filename, totalCount);

    if (totalCount <= 0)
    {
        delete[] allRecords;
        return false;
    }
    int matchSeen = 0;
    int targetIndex = -1;
    for (int i = 0; i < totalCount; i++)
    {
        if (allRecords[i].username == targetUsername)
        {
            matchSeen++;
            if (matchSeen == journalNumber)
            {
                targetIndex = i;
                break;
            }
        }
    }

    if (targetIndex == -1)
    {
        delete[] allRecords;
        return false;
    }

    ofstream fileStream(filename, ios::trunc);
    if (!fileStream.is_open())
    {
        delete[] allRecords;
        return false;
    }

    for (int i = 0; i < totalCount; i++)
    {
        if (i == targetIndex)
            continue;

        fileStream << allRecords[i].username << DELIM
                   << allRecords[i].date << DELIM
                   << allRecords[i].mood << DELIM
                   << allRecords[i].productivity << DELIM
                   << allRecords[i].note << "\n";
    }

    fileStream.close();
    delete[] allRecords;
    return true;
}

string encryptPassword(const string &plain)
{
    std::string result = plain;
    for (int i = 0; i < (int)plain.size(); i++)
        result[i] = (char)((plain[i] + NIM_KEY[i % KEY_LENGTH]) % 128);
    return result;
}

// MENU JURNAL HARIAN
void jurnalHarian()
{
    int menuChoice = 0;
    const string jurnalMenuOption[] = {"Tulis Jurnal", "Lihat Semua Jurnal", "Cari Jurnal", "Hapus Jurnal", "Kembali"};
    const int jurnalMenuOptionLength = 5;

    bool running = true;
    while (running)
    {
        CLEAR_SCREEN;
        HIDE_CURSOR;
        drawOption(jurnalMenuOption, jurnalMenuOptionLength, menuChoice, 35);

        int key = getKey();
        switch (key)
        {
        case 72:
            menuChoice = (menuChoice == 0) ? jurnalMenuOptionLength - 1 : menuChoice - 1;
            break;
        case 80:
            menuChoice = (menuChoice == jurnalMenuOptionLength - 1) ? 0 : menuChoice + 1;
            break;
        case 13:
            switch (menuChoice)
            {
            case 0:
                tulisJurnal();
                break;
            case 1:
                tampilkanSemuaJurnal();
                break;
            case 2:
                cariJurnal("data/userData.txt");
                break;
            case 3:
                hapusJurnal();
                break;
            case 4:
                running = false;
                break;
            }
            break;
        }
    }
}

// sub menu JURNAL HARIAN
void tulisJurnal()
{
    CLEAR_SCREEN;
    SHOW_CURSOR;

    cout << CYAN << "======== TULIS JURNAL ========" << RESET_COLOR << "\n\n";

    userData journalBuffer;
    journalBuffer.username = currentUser;
    journalBuffer.date = getCurrentDate();

    cout << "Username           : " << journalBuffer.username << "\n";
    cout << "Date               : " << journalBuffer.date << "\n";

    readInt(journalBuffer.mood, 1, 5, "Mood (1-5)         : ");
    readInt(journalBuffer.productivity, 1, 10, "Productivity (1-10): ");

    clearInputBuffer();

    string noteBuffer;
    do
    {
        cout << "Note               : ";
        getline(cin, noteBuffer);
        journalBuffer.note = trimSpaces(noteBuffer);
        if (journalBuffer.note.empty())
            cout << RED << "  [!] Note tidak boleh kosong.\n" << RESET_COLOR;
    } while (journalBuffer.note.empty());

    // Simpan ke userdata.txt 
    bool ok = writeFile("data/userdata.txt", journalBuffer);

    if (ok)
        cout << GREEN << "\nJurnal berhasil disimpan." << RESET_COLOR << "\n";
    else if (!ok)
        cout << RED << "\nGagal menyimpan jurnal" << RESET_COLOR << "\n";
    else
        cout << RED << "\nGagal menyimpan jurnal" << RESET_COLOR << "\n";

    cout << "\nPRESS ANY KEY TO RETURN...";
    HIDE_CURSOR;
    getCh();
}

void tampilkanSemuaJurnal()
{
    CLEAR_SCREEN;
    SHOW_CURSOR;

    printUserJournal("data/userData.txt");

    cout << "\nPRESS ANY KEY TO RETURN...";
    HIDE_CURSOR;
    getCh();
}

void cariJurnal()
{
    CLEAR_SCREEN;
    SHOW_CURSOR;

    cout << CYAN << "===== CARI JURNAL =====" << RESET_COLOR << "\n\n";
    cout << "Masukkan tanggal / bagian tanggal (YYYY-MM-DD), kosongkan untuk semua: ";

    string keyword;
    getline(cin, keyword);
    keyword = trimSpaces(keyword);

    int matchCount = 0;
    userData *userJournals = searchByUsername("data/userData.txt", currentUser, matchCount);

    if (matchCount < 0)
    {
        cout << RED << "Gagal membaca data jurnal." << RESET_COLOR << "\n";
    }
    else
    {
        int found = 0;
        for (int i = 0; i < matchCount; i++)
        {
            if (keyword.empty() || userJournals[i].date.find(keyword) != string::npos)
            {
                found++;
                cout << "\n--- Journal " << found << " ---\n";
                cout << "Date        : " << userJournals[i].date << "\n";
                cout << "Mood        : " << userJournals[i].mood << "\n";
                cout << "Productivity: " << userJournals[i].productivity << "\n";
                cout << "Note        : " << userJournals[i].note << "\n";
            }
        }

        if (found == 0)
            cout << "\nTidak ada jurnal yang cocok dengan '" << keyword << "'.\n";
    }

    delete[] userJournals;

    cout << "\nPRESS ANY KEY TO RETURN...";
    HIDE_CURSOR;
    getCh();
}

void hapusJurnal()
{
    CLEAR_SCREEN;
    SHOW_CURSOR;

    int matchCount = 0;
    userData *userJournals = searchByUsername("data/userData.txt", currentUser, matchCount);

    if (matchCount <= 0)
    {
        cout << "Belum ada jurnal untuk dihapus.\n";
        cout << "\nPRESS ANY KEY TO RETURN...";
        delete[] userJournals;
        HIDE_CURSOR;
        getCh();
        return;
    }

    cout << CYAN << "===== HAPUS JURNAL =====" << RESET_COLOR << "\n";
    for (int i = 0; i < matchCount; i++)
    {
        cout << "\n--- Journal " << i + 1 << " ---\n";
        cout << "Date        : " << userJournals[i].date << "\n";
        cout << "Mood        : " << userJournals[i].mood << "\n";
        cout << "Productivity: " << userJournals[i].productivity << "\n";
        cout << "Note        : " << userJournals[i].note << "\n";
    }
    delete[] userJournals;

    int journalNumber;
    readInt(journalNumber, 0, matchCount, "\nMasukkan nomor jurnal yang ingin dihapus (0 untuk batal): ");

    if (journalNumber == 0)
    {
        cout << "\nDibatalkan.\n";
    }
    else if (deleteJournal("data/userData.txt", currentUser, journalNumber))
    {
        cout << GREEN << "\nJurnal berhasil dihapus." << RESET_COLOR << "\n";
    }
    else
    {
        cout << RED << "\nGagal menghapus jurnal." << RESET_COLOR << "\n";
    }

    cout << "\nPRESS ANY KEY TO RETURN...";
    HIDE_CURSOR;
    getCh();
}

// =========================================================================
// 9. FITUR STATISTIK 
// =========================================================================
string getWeekLabel(const string &date)
{
    if (date.size() < 10) return "Unknown";

    int year  = stoi(date.substr(0, 4));
    int month = stoi(date.substr(5, 2));
    int day   = stoi(date.substr(8, 2));

    int daysInMonth[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
        daysInMonth[2] = 29;

    int dayOfYear = day;
    for (int m = 1; m < month; m++)
        dayOfYear += daysInMonth[m];

    int weekNum = (dayOfYear - 1) / 7 + 1;

    char buf[16];
    snprintf(buf, sizeof(buf), "%04d-W%02d", year, weekNum);
    return string(buf);
}

string getMonthLabel(const string &date)
{
    if (date.size() < 7) return "Unknown";
    return date.substr(0, 7);
}

void printBar(double value, double maxValue, int barWidth = 25)
{
    int filled = (maxValue > 0) ? (int)((value / maxValue) * barWidth) : 0;
    cout << "[";
    for (int i = 0; i < barWidth; i++)
        cout << (i < filled ? "#" : "-");
    cout << "]";
}

string getNamaBulan(const string &monthLabel)
{
    const string namaBulan[] = {
        "", "Januari", "Februari", "Maret", "April", "Mei", "Juni",
        "Juli", "Agustus", "September", "Oktober", "November", "Desember"
    };
    if (monthLabel.size() < 7) return monthLabel;
    int m = stoi(monthLabel.substr(5, 2));
    if (m < 1 || m > 12) return monthLabel;
    return monthLabel.substr(0, 4) + " - " + namaBulan[m];
}

struct PeriodStat
{
    string label;
    double totalMood;
    double totalProd;
    int    count;
};

int findOrAddPeriod(PeriodStat *arr, int &size, int capacity, const string &label)
{
    for (int i = 0; i < size; i++)
        if (arr[i].label == label) return i;

    if (size >= capacity) return -1;

    arr[size].label     = label;
    arr[size].totalMood = 0;
    arr[size].totalProd = 0;
    arr[size].count     = 0;
    return size++;
}

void sortPeriodStats(PeriodStat *arr, int size)
{
    for (int i = 1; i < size; i++)
    {
        PeriodStat key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j].label > key.label)
        {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void tampilkanTabelStatistik(PeriodStat *stats, int size, bool isWeekly)
{
    if (size == 0)
    {
        cout << "  Tidak ada data untuk ditampilkan.\n";
        return;
    }
    sortPeriodStats(stats, size);

    cout << "\n" << CYAN;
    cout << "  " << string(68, '=') << "\n";
    cout << "  " << (isWeekly ? "  PERIODE (MINGGU)  " : "    PERIODE (BULAN)   ")
         << "| Jml |  Rata Mood  | Rata Produktivitas\n";
    cout << "  " << string(68, '=') << "\n";
    cout << RESET_COLOR;

    for (int i = 0; i < size; i++)
    {
        double avgMood = stats[i].totalMood / stats[i].count;
        double avgProd = stats[i].totalProd / stats[i].count;

        string periodeLabel = isWeekly
            ? stats[i].label
            : getNamaBulan(stats[i].label);

        while ((int)periodeLabel.size() < 22) periodeLabel += " ";

        char moodBuf[8], prodBuf[8];
        snprintf(moodBuf, sizeof(moodBuf), "%.2f", avgMood);
        snprintf(prodBuf, sizeof(prodBuf), "%.2f", avgProd);

        cout << "  " << periodeLabel << "|  " << stats[i].count
             << (stats[i].count < 10 ? "  " : " ") << " | ";

        if (avgMood >= 4.0)      cout << GREEN;
        else if (avgMood <= 2.0) cout << RED;
        else                     cout << YELLOW;
        cout << moodBuf << "/5.00 " << RESET_COLOR << "  | ";

        if (avgProd >= 7.0)      cout << GREEN;
        else if (avgProd <= 3.0) cout << RED;
        else                     cout << YELLOW;
        cout << prodBuf << "/10.00" << RESET_COLOR << "\n";
    }

    cout << CYAN << "  " << string(68, '=') << RESET_COLOR << "\n";

    cout << "\n" << CYAN << "  [ Grafik Rata-rata Mood (skala 1-5) ]" << RESET_COLOR << "\n";
    for (int i = 0; i < size; i++)
    {
        double avgMood = stats[i].totalMood / stats[i].count;
        string lbl = isWeekly ? stats[i].label : getNamaBulan(stats[i].label);
        while ((int)lbl.size() < 22) lbl += " ";

        char buf[8]; snprintf(buf, sizeof(buf), "%.2f", avgMood);

        if (avgMood >= 4.0)      cout << GREEN;
        else if (avgMood <= 2.0) cout << RED;
        else                     cout << YELLOW;

        cout << "  " << lbl << " ";
        printBar(avgMood, 5.0);
        cout << " " << buf << "/5.00\n";
        cout << RESET_COLOR;
    }

    cout << "\n" << CYAN << "  [ Grafik Rata-rata Produktivitas (skala 1-10) ]" << RESET_COLOR << "\n";
    for (int i = 0; i < size; i++)
    {
        double avgProd = stats[i].totalProd / stats[i].count;
        string lbl = isWeekly ? stats[i].label : getNamaBulan(stats[i].label);
        while ((int)lbl.size() < 22) lbl += " ";

        char buf[8]; snprintf(buf, sizeof(buf), "%.2f", avgProd);

        if (avgProd >= 7.0)      cout << GREEN;
        else if (avgProd <= 3.0) cout << RED;
        else                     cout << YELLOW;

        cout << "  " << lbl << " ";
        printBar(avgProd, 10.0);
        cout << " " << buf << "/10.00\n";
        cout << RESET_COLOR;
    }
}

void hitungDanTampilkanStatistik(userData *records, int count, bool isWeekly)
{
    int capacity = isWeekly ? 520 : 120;
    PeriodStat *stats = new PeriodStat[capacity];
    int size = 0;

    for (int i = 0; i < count; i++)
    {
        string label = isWeekly
            ? getWeekLabel(records[i].date)
            : getMonthLabel(records[i].date);

        int idx = findOrAddPeriod(stats, size, capacity, label);
        if (idx < 0) continue;

        stats[idx].totalMood += records[i].mood;
        stats[idx].totalProd += records[i].productivity;
        stats[idx].count++;
    }

    tampilkanTabelStatistik(stats, size, isWeekly);
    delete[] stats;
}

void tampilkanRingkasan(userData *records, int count)
{
    if (count == 0) return;

    double totalMood = 0, totalProd = 0;
    int maxMood = records[0].mood, minMood = records[0].mood;
    int maxProd = records[0].productivity, minProd = records[0].productivity;
    string maxMoodDate = records[0].date, minMoodDate = records[0].date;

    for (int i = 0; i < count; i++)
    {
        totalMood += records[i].mood;
        totalProd += records[i].productivity;
        if (records[i].mood > maxMood) { maxMood = records[i].mood; maxMoodDate = records[i].date; }
        if (records[i].mood < minMood) { minMood = records[i].mood; minMoodDate = records[i].date; }
        if (records[i].productivity > maxProd) maxProd = records[i].productivity;
        if (records[i].productivity < minProd) minProd = records[i].productivity;
    }

    char moodBuf[8], prodBuf[8];
    snprintf(moodBuf, sizeof(moodBuf), "%.2f", totalMood / count);
    snprintf(prodBuf, sizeof(prodBuf), "%.2f", totalProd / count);

    cout << CYAN << "\n  ===== RINGKASAN KESELURUHAN =====" << RESET_COLOR << "\n";
    cout << "  Total Jurnal           : " << count << " entri\n";

    cout << "  Rata-rata Mood         : ";
    if ((totalMood / count) >= 4.0) cout << GREEN;
    else if ((totalMood / count) <= 2.0) cout << RED;
    else cout << YELLOW;
    cout << moodBuf << " / 5.00" << RESET_COLOR << "\n";

    cout << "  Rata-rata Produktivitas: ";
    if ((totalProd / count) >= 7.0) cout << GREEN;
    else if ((totalProd / count) <= 3.0) cout << RED;
    else cout << YELLOW;
    cout << prodBuf << " / 10.00" << RESET_COLOR << "\n";

    cout << "  Mood Tertinggi         : " << GREEN << maxMood << RESET_COLOR
         << " (pada " << maxMoodDate << ")\n";
    cout << "  Mood Terendah          : " << RED << minMood << RESET_COLOR
         << " (pada " << minMoodDate << ")\n";
    cout << "  Produktivitas Maks     : " << GREEN << maxProd << RESET_COLOR << "\n";
    cout << "  Produktivitas Min      : " << RED   << minProd << RESET_COLOR << "\n";
}

void statistik()
{
    int menuChoice = 0;
    const string statMenuOption[] = {
        "Statistik Mingguan",
        "Statistik Bulanan",
        "Kembali"
    };
    const int statMenuOptionLength = 3;

    bool running = true;
    while (running)
    {
        CLEAR_SCREEN;
        HIDE_CURSOR;
        drawOption(statMenuOption, statMenuOptionLength, menuChoice, 27);

        int key = getKey();
        switch (key)
        {
        case 72:
            menuChoice = (menuChoice == 0) ? statMenuOptionLength - 1 : menuChoice - 1;
            break;
        case 80:
            menuChoice = (menuChoice == statMenuOptionLength - 1) ? 0 : menuChoice + 1;
            break;
        case 13:
            if (menuChoice == 2)
            {
                running = false;
                break;
            }
            {
                CLEAR_SCREEN;
                SHOW_CURSOR;

                bool isWeekly = (menuChoice == 0);

                cout << CYAN;
                cout << "  +--------------------------------------------------+\n";
                cout << "  ¦  " << (isWeekly ? "STATISTIK MINGGUAN" : "STATISTIK BULANAN ")
                     << " - " << currentUser;
                int pad = 25 - (int)currentUser.size();
                if (pad < 0) pad = 0;
                cout << string(pad, ' ') << "¦\n";
                cout << "  +--------------------------------------------------+\n";
                cout << RESET_COLOR;

                int count = 0;
                userData *records = searchByUsername("data/userData.txt", currentUser, count);

                if (count < 0)
                {
                    cout << RED << "\n  [!] Gagal membaca file data jurnal.\n" << RESET_COLOR;
                }
                else if (count == 0)
                {
                    cout << YELLOW << "\n  [!] Belum ada jurnal.\n"
                         << "      Silakan tulis jurnal terlebih dahulu.\n" << RESET_COLOR;
                }
                else
                {
                    tampilkanRingkasan(records, count);
                    cout << CYAN << "\n  ===== DETAIL PER "
                         << (isWeekly ? "MINGGU" : "BULAN")
                         << " =====" << RESET_COLOR << "\n";
                    hitungDanTampilkanStatistik(records, count, isWeekly);
                }

                delete[] records;

                cout << "\n\n  PRESS ANY KEY TO RETURN...";
                HIDE_CURSOR;
                getCh();
            }
            break;
        }
    }
}

// =========================================================================
// 10. FITUR Achievement
// =========================================================================

bool tahunKabisat(int tahun)
{
    if (tahun % 4 != 0)
        return false;
    if (tahun % 100 != 0)
        return true;
    if (tahun % 400 != 0)
        return false;
    return true;
}

int jumlahHariDalamBulan(int bulan, int tahun)
{
    int hari[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (bulan == 2 && tahunKabisat(tahun))
        return 29;
    return hari[bulan - 1];
}

bool cekStreak(userData *dataUser, int jmlData, int target)
{
    if (jmlData < target)
        return false;

    int hitungStreak = 1;
    for (int i = 1; i < jmlData; i++)
    {
        int tahun1 = stoi(dataUser[i - 1].date.substr(0, 4));
        int bulan1 = stoi(dataUser[i - 1].date.substr(5, 2));
        int hari1 = stoi(dataUser[i - 1].date.substr(8, 2));

        int tahun2 = stoi(dataUser[i].date.substr(0, 4));
        int bulan2 = stoi(dataUser[i].date.substr(5, 2));
        int hari2 = stoi(dataUser[i].date.substr(8, 2));

        bool lanjut = false;
        int akhirBulan1 = jumlahHariDalamBulan(bulan1, tahun1);

        if (tahun2 == tahun1 && bulan2 == bulan1 && hari2 == hari1 + 1)
            lanjut = true;
        else if (tahun2 == tahun1 && bulan2 == bulan1 + 1 && hari1 == akhirBulan1 && hari2 == 1)
            lanjut = true;
        else if (tahun2 == tahun1 + 1 && bulan1 == 12 && bulan2 == 1 && hari1 == 31 && hari2 == 1)
            lanjut = true;

        if (lanjut)
        {
            hitungStreak++;
            if (hitungStreak >= target)
                return true;
        }
        else
        {
            hitungStreak = 1;
        }
    }
    return false;
}

void urutkanByTanggal(userData *data, int jmlData)
{
    for (int i = 1; i < jmlData; i++)
    {
        userData kunci = data[i];
        int j = i - 1;
        while (j >= 0 && data[j].date > kunci.date)
        {
            data[j + 1] = data[j];
            j--;
        }
        data[j + 1] = kunci;
    }
}

bool sudahPunyaAchievement(const string &username, const string &namaAchievement)
{
    ifstream fileStream("data/achievements.txt");
    if (!fileStream.is_open())
        return false;

    string line;
    while (getline(fileStream, line))
    {
        if (line.empty())
            continue;
        size_t pos = 0;
        string user = nextToken(line, pos);
        string nama = line.substr(pos);
        if (user == username && nama == namaAchievement)
        {
            fileStream.close();
            return true;
        }
    }
    fileStream.close();
    return false;
}

bool simpanAchievement(const string &username, const string &namaAchievement)
{
    ofstream fileStream("data/achievements.txt", ios::app);
    if (!fileStream.is_open())
        return false;

    fileStream << username << DELIM << namaAchievement << "\n";
    bool success = !fileStream.fail();
    fileStream.close();
    return success;
}

void cekAchievementBaru(const string &username)
{
    int jmlData = 0;
    userData *dataUser = searchByUsername("data/userData.txt", username, jmlData);

    if (jmlData <= 0)
    {
        delete[] dataUser;
        cout << "Belum ada jurnal, belum ada achievement yang bisa dicek.\n";
        return;
    }

    urutkanByTanggal(dataUser, jmlData);

    bool adaYgBaru = false;

    if (jmlData >= 1 && !sudahPunyaAchievement(username, "First Entry"))
    {
        simpanAchievement(username, "First Entry");
        cout << GREEN << "[!] Selamat! Kamu mulai menulis jurnal pertamamu!" << RESET_COLOR << "\n";
        adaYgBaru = true;
    }

    if (cekStreak(dataUser, jmlData, 7) && !sudahPunyaAchievement(username, "7-Day Streak"))
    {
        simpanAchievement(username, "7-Day Streak");
        cout << GREEN << "[!] Selamat! Kamu konsisten menulis jurnal 7 hari berturut-turut!" << RESET_COLOR << "\n";
        adaYgBaru = true;
    }

    if (cekStreak(dataUser, jmlData, 30) && !sudahPunyaAchievement(username, "30-Day Streak"))
    {
        simpanAchievement(username, "30-Day Streak");
        cout << GREEN << "[!] Selamat! Kamu konsisten menulis jurnal 30 hari berturut-turut!" << RESET_COLOR << "\n";
        adaYgBaru = true;
    }

    if (!adaYgBaru)
        cout << YELLOW << "Tidak ada achievement baru." << RESET_COLOR << "\n";

    delete[] dataUser;
}

void tampilkanAchievement(const string &username)
{
    ifstream fileStream("data/achievements.txt");
    if (!fileStream.is_open())
    {
        cout << "Belum ada achievement yang didapat.\n";
        return;
    }

    cout << "\n===== ACHIEVEMENTS - " << username << " =====\n";
    string line;
    int nomor = 1;
    bool ada = false;

    while (getline(fileStream, line))
    {
        if (line.empty())
            continue;
        size_t pos = 0;
        string user = nextToken(line, pos);
        string nama = line.substr(pos);
        if (user == username)
        {
            cout << nomor << ". " << nama << "\n";
            nomor++;
            ada = true;
        }
    }
    fileStream.close();

    if (!ada)
        cout << "Belum ada achievement yang didapat.\n";
}

void achievement()
{
    int menuChoice = 0;
    const string achievementMenuOption[] = {"Cek Achievement Baru", "Lihat Semua Achievement", "Kembali"};
    const int achievementMenuOptionLength = 3;

    bool running = true;
    while (running)
    {
        CLEAR_SCREEN;
        HIDE_CURSOR;
        drawOption(achievementMenuOption, achievementMenuOptionLength, menuChoice, 25);

        int key = getKey();
        switch (key)
        {
        case 72:
            menuChoice = (menuChoice == 0) ? achievementMenuOptionLength - 1 : menuChoice - 1;
            break;
        case 80:
            menuChoice = (menuChoice == achievementMenuOptionLength - 1) ? 0 : menuChoice + 1;
            break;
        case 13:
            switch (menuChoice)
            {
            case 0:
                CLEAR_SCREEN;
                SHOW_CURSOR;
                cout << CYAN << "======== CEK ACHIEVEMENT ========" << RESET_COLOR << "\n\n";
                cekAchievementBaru(currentUser);
                cout << "\nPRESS ANY KEY TO RETURN...";
                HIDE_CURSOR;
                getCh();
                break;
            case 1:
                CLEAR_SCREEN;
                SHOW_CURSOR;
                tampilkanAchievement(currentUser);
                cout << "\nPRESS ANY KEY TO RETURN...";
                HIDE_CURSOR;
                getCh();
                break;
            case 2:
                running = false;
                break;
            }
            break;
        }
    }
}

void profilAkun()
{
    CLEAR_SCREEN;
    SHOW_CURSOR;

    int jumlahJurnal = 0;
    userData *records = searchByUsername("data/userData.txt", currentUser, jumlahJurnal);
    if (jumlahJurnal < 0)
        jumlahJurnal = 0;

    int jumlahAchievement = 0;
    ifstream achFile("data/achievements.txt");
    if (achFile.is_open())
    {
        string line;
        while (getline(achFile, line))
        {
            if (line.empty())
                continue;
            size_t pos = 0;
            string user = nextToken(line, pos);
            if (user == currentUser)
                jumlahAchievement++;
        }
        achFile.close();
    }

    cout << CYAN << "======== PROFIL AKUN ========" << RESET_COLOR << "\n\n";
    cout << "Username          : " << currentUser << "\n";
    cout << "Total Jurnal      : " << jumlahJurnal << " entri\n";
    cout << "Total Achievement : " << jumlahAchievement << "\n";

    delete[] records;

    cout << "\nPRESS ANY KEY TO RETURN...";
    HIDE_CURSOR;
    getCh();
}

void about()
{
    CLEAR_SCREEN;
    SHOW_CURSOR;

    cout << CYAN
         << "========== ABOUT ==========\n\n"
         << RESET_COLOR;

    cout << "LifeLog v1.0\n\n";

    cout << "LifeLog adalah aplikasi jurnal harian\n";
    cout << "berbasis C++ yang digunakan untuk\n";
    cout << "mencatat mood, produktivitas, dan\n";
    cout << "aktivitas harian pengguna.\n\n";

    cout << "Fitur Utama:\n";
    cout << "- Jurnal Harian\n";
    cout << "- Statistik Mood & Produktivitas\n";
    cout << "- Achievement System\n";
    cout << "- Profil Akun\n\n";

    cout << "===== DEVELOPED BY =====\n\n";

    cout << "1. Vidia Iqmalia Putri          (F1D02510029)\n";
    cout << "2. Lale Juliesti Ayuningkusuma (F1D02510064)\n";
    cout << "3. Lalu Muhammad Izza Abda     (F1D02510067)\n";
    cout << "4. Muhammad Bijak Rizqullah    (F1D02510081)\n";
    cout << "5. Zidny Ilma                  (F1D02510100)\n";
    cout << "6. Andika Ramdhanu             (F1D02510104)\n";
    cout << "7. Nurlaeli Hidayati           (F1D02510129)\n";

    cout << "\nUniversitas Mataram\n";
    cout << "Program Studi Teknik Informatika\n";

    cout << "\nPRESS ANY KEY TO RETURN...";
    HIDE_CURSOR;
    getCh();
}
