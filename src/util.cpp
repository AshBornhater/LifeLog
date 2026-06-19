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

userData *searchByDate(const string &filename, const string &targetDate, int &outCount)
{
    ifstream fileStream(filename);
    if (!fileStream.is_open())
    {
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

        if (parsedRecord.date == targetDate)
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
        if (categoryOption == 1 && parsedRecord.mood < 3)
            isMatch = true;
        if (categoryOption == 2 && parsedRecord.mood == 3)
            isMatch = true;
        if (categoryOption == 3 && parsedRecord.mood > 3)
            isMatch = true;

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

        if (parsedRecord.productivity == targetProd)
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

userData *searchByKeyword(const string &filename, const string &keyword, int &outCount)
{
    ifstream fileStream(filename);
    if (!fileStream.is_open())
    {
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

        if (parsedRecord.note.find(keyword) != string::npos)
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
            
            SHOW_CURSOR; 

            userData *matchedRecords = nullptr;
            int matchCount = 0;
            bool processSearch = false; 

            cout << "\n\n"; 

            switch (menuChoice)
            {
            case 0: 
            {
                string targetDate;
                cout << "Masukkan tanggal (YYYY-MM-DD): ";
                clearInputBuffer();
                getline(cin, targetDate);
                matchedRecords = searchByDate(filename, targetDate, matchCount);
                processSearch = true;
                break;
            }
            case 1: 
            {
                int moodChoice;
                cout << "Pilih Kategori Mood:\n";
                cout << "1. Mood Buruk (< 3)\n";
                cout << "2. Mood Biasa (== 3)\n";
                cout << "3. Mood Bagus (> 3)\n";
                readInt(moodChoice, 1, 3, "Pilihan Kategori (1-3): ");
                matchedRecords = searchByMoodCategory(filename, moodChoice, matchCount);
                processSearch = true;
                break;
            }
            case 2: 
            {
                int targetProd;
                readInt(targetProd, 1, 10, "Masukkan produktivitas (1-10): ");
                matchedRecords = searchByProductivity(filename, targetProd, matchCount);
                processSearch = true;
                break;
            }
            case 3: 
            {
                string keyword;
                cout << "Masukkan keyword: ";
                clearInputBuffer();
                getline(cin, keyword);
                matchedRecords = searchByKeyword(filename, keyword, matchCount);
                processSearch = true;
                break;
            }
            case 4: 
                running = false;
                break;
            }

            if (processSearch)
            {
                if (matchCount < 0)
                {
                    cout << RED << "\n[!] Gagal membuka file atau file tidak ditemukan." << RESET_COLOR << "\n";
                }
                else if (matchCount == 0)
                {
                    cout << "\nData tidak ditemukan.\n";
                }
                else
                {
                    cout << "\n\"" << matchCount << " Jurnal Ditemukan\"\n\n";
                    for (int i = 0; i < matchCount; i++)
                    {
                        cout << "username     = \"" << matchedRecords[i].username << "\";\n";
                        cout << "date         = \"" << matchedRecords[i].date << "\";\n";
                        cout << "mood         = " << matchedRecords[i].mood << ";\n";
                        cout << "productivity = " << matchedRecords[i].productivity << ";\n";
                        cout << "note         = \"" << matchedRecords[i].note << "\";\n";
                        cout << "--------------------------\n";
                    }
                }
                delete[] matchedRecords; 
                
                cout << "\nTekan Enter untuk kembali ke menu...";
                clearInputBuffer();
                cin.get(); 
            }
            break;
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
    ofstream fileStream(filename, ios::app);
    if (!fileStream.is_open())
    {
        cout << RED << "[writeFile] Tidak bisa membuka '" << filename << "'" << RESET_COLOR << "\n";
        return false;
    }

    fileStream << data.username << DELIM
               << data.date << DELIM
               << data.mood << DELIM
               << data.productivity << DELIM
               << data.note << "\n";

    if (fileStream.fail())
    {
        cout << RED << "[writeFile] Gagal menulis ke '" << filename << "'" << RESET_COLOR << "\n";
        fileStream.close();
        return false;
    }

    fileStream.close();
    return true;
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
                cariJurnal();
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
            cout << RED << "  [!] Note tidak boleh kosong.\n"
                 << RESET_COLOR;
    } while (journalBuffer.note.empty());

    if (writeFile("data/userData.txt", journalBuffer))
        cout << GREEN << "\nJurnal berhasil disimpan." << RESET_COLOR << "\n";
    else
        cout << RED << "\nGagal menyimpan jurnal." << RESET_COLOR << "\n";

    cout << "\nPRESS ANY KEY TO RETURN...";
    HIDE_CURSOR;
    getCh();
}

void tampilkanSemuaJurnal()
{
}

void cariJurnal()
{
}

void hapusJurnal()
{
}

void statistik()
{
    // TODO: Implementasi fitur statistik
}

void achievement()
{
    // TODO: Implementasi fitur achievement
}

void profilAkun()
{
    // TODO: Implementasi fitur profil akun
}

void about()
{
    // TODO: Implementasi informasi aplikasi
}
