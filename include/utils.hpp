#pragma once

#include <iostream>
#include <string>
#include <conio.h>
#include <fstream>

// Makro untuk tata letak batas UI terminal global
#define BORDER_ROW 3
#define BORDER_COLUMN 42
#define DELIM "<|>"
#define DELIM_LEN 3

// Makro untuk kode warna ANSI dan manipulasi layar terminal
#define RESET_COLOR "\033[0m"
#define GREEN "\033[32m"
#define CYAN "\033[36m"
#define YELLOW "\033[93m"
#define RED "\033[31m"
#define CLEAR_SCREEN cout << "\033[2J\033[H"
#define HIDE_CURSOR cout << "\033[?25l"
#define SHOW_CURSOR cout << "\033[?25h"

// Struct untuk menampung data akun
struct Account {
    std::string username;
    std::string password;
};

// Struct untuk menampung rekaman data harian user
struct userData {
    std::string username;
    std::string date;
    int mood;
    int productivity;
    std::string note;
};

// Struct untuk akumulasi data statistik per periode
struct PeriodStat {
    std::string label;
    double totalMood;
    double totalProd;
    int count;
};

// Variabel global untuk menyimpan username aktif
extern std::string currentUser;

// Konstanta global untuk kunci enkripsi password berbasis NIM anggota kelompok
constexpr int NIM_KEY[] = {67, 64, 100, 104, 129, 81, 29};
constexpr int KEY_LENGTH = 7;

// =========================================================================
// FUNCTION PROTOTYPES
// =========================================================================

// --- MODUL: AUTENTIKASI (`auth.cpp`) ---
bool parseAccount(const std::string &line, Account &parsedData);
bool login(const std::string &username, const std::string &password);
bool usernameExists(const std::string &username);
bool writeAccount(const Account &account);
bool loginMenu();
bool registerMenu();
void loginInterface(bool &stop);
std::string encryptPassword(const std::string &plain);
void profilAkun();

// --- MODUL: MANAJEMEN JURNAL (`journal.cpp`) ---
bool parseLine(const std::string &line, userData &parsedData);
void readInt(int &num, int minValue, int maxValue, const std::string instruction);
int boundInput(int val, int low, int high);
userData* readFile(const std::string &filename, int &outCount);
userData* searchByUsername(const std::string &filename, const std::string &targetUsername, int &outCount);
userData* searchByDate(const std::string &filename, const std::string &targetDateRaw, int &outCount);
userData* searchByMoodCategory(const std::string &filename, int categoryOption, int &outCount);
userData* searchByProductivity(const std::string &filename, int targetProd, int &outCount);
userData* searchByKeyword(const std::string &filename, const std::string &keywordRaw, int &outCount);
void cariJurnal(const std::string &filename);
void printUserJournal(const std::string &filename);
bool writeFile(const std::string &filename, const userData &data);
bool deleteJournal(const std::string &filename, const std::string &targetUsername, int journalNumber);
void jurnalHarian();
void tulisJurnal();
void tampilkanSemuaJurnal();
void cariJurnal();
void hapusJurnal();

// --- MODUL: STATISTIK (`stats.cpp`) ---
std::string getWeekLabel(const std::string &date);
std::string getMonthLabel(const std::string &date);
void printBar(double value, double maxValue, int barWidth = 25);
std::string getNamaBulan(const std::string &monthLabel);
int findOrAddPeriod(PeriodStat *arr, int &size, int capacity, const std::string &label);
void sortPeriodStats(PeriodStat *arr, int size);
void tampilkanTabelStatistik(PeriodStat *stats, int size, bool isWeekly);
void hitungDanTampilkanStatistik(userData *records, int count, bool isWeekly);
void tampilkanRingkasan(userData *records, int count);
void statistik();

// --- MODUL: SISTEM PENCAPAIAN (`achievement.cpp`) ---
bool tahunKabisat(int tahun);
int jumlahHariDalamBulan(int bulan, int tahun);
bool cekStreak(userData *dataUser, int jmlData, int target);
void urutkanByTanggal(userData *data, int jmlData);
bool sudahPunyaAchievement(const std::string &username, const std::string &namaAchievement);
bool simpanAchievement(const std::string &username, const std::string &namaAchievement);
void cekAchievementBaru(const std::string &username);
void tampilkanAchievement(const std::string &username);
void achievement();

// --- MODUL: TERMINAL & ANTARMUKA CLI (`terminalUI.cpp`) ---
int getKey();
void moveCursor(int row, int col);
void clearInputBuffer();
void drawBorder(int startRow, int startCol, int height, int width);
void drawLifeLogLogoLogin(int startRow, int startCol);
void mainMenuInterface(bool &stop);
void textField();
void drawOption(const std::string *arrOptions, int optionslength, int userChoose, int borderHeight);
userData* resizeRecords(userData *oldArr, int oldSize, int newSize);
std::string trimSpaces(const std::string &src);
std::string nextToken(const std::string &line, std::size_t &position);
std::string getCurrentDate();
void about();
