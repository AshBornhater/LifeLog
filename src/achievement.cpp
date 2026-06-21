#include "../include/utils.hpp"
#include <ctime>

using namespace std;

// Fungsi untuk memeriksa apakah suatu tahun merupakan tahun kabisat
bool tahunKabisat(int tahun)
{
    if (tahun % 4 != 0) return false;
    if (tahun % 100 != 0) return true;
    if (tahun % 400 != 0) return false;
    return true;
}

// Fungsi untuk menghitung jumlah total hari dalam suatu bulan spesifik
int jumlahHariDalamBulan(int bulan, int tahun)
{
    int hari[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (bulan == 2 && tahunKabisat(tahun))
        return 29;
    return hari[bulan - 1];
}

// Fungsi untuk memvalidasi pencapaian streak menulis jurnal berturut-turut
bool cekStreak(userData *dataUser, int jmlData, int target)
{
    if (jmlData < target)
        return false;

    int maxStreak = 1;
    int currentStreak = 1;

    for (int i = 1; i < jmlData; i++)
    {
        tm t1 = {0}, t2 = {0};
        
        t1.tm_year = stoi(dataUser[i - 1].date.substr(0, 4)) - 1900;
        t1.tm_mon  = stoi(dataUser[i - 1].date.substr(5, 2)) - 1;
        t1.tm_mday = stoi(dataUser[i - 1].date.substr(8, 2));

        t2.tm_year = stoi(dataUser[i].date.substr(0, 4)) - 1900;
        t2.tm_mon  = stoi(dataUser[i].date.substr(5, 2)) - 1;
        t2.tm_mday = stoi(dataUser[i].date.substr(8, 2));

        time_t time1 = mktime(&t1);
        time_t time2 = mktime(&t2);

        double selisihDetik = difftime(time2, time1);
        
        // Konversi selisih waktu dari detik menjadi satuan hari (86400 detik = 1 hari)
        long long selisihHari = selisihDetik / 86400;

        if (selisihHari == 1)
        {
            currentStreak++;
            if (currentStreak > maxStreak) {
                maxStreak = currentStreak;
            }
        }
        else if (selisihHari > 1)
        {
            currentStreak = 1;
        }
    }

    return maxStreak >= target;
}

// Fungsi untuk menghitung total hari streak tertinggi yang pernah dicapai user
int hitungMaksimalStreak(userData *dataUser, int jmlData)
{
    if (jmlData <= 0) return 0;

    int maxStreak = 1;
    int currentStreak = 1;

    for (int i = 1; i < jmlData; i++)
    {
        tm t1 = {0}, t2 = {0};
        
        t1.tm_year = stoi(dataUser[i - 1].date.substr(0, 4)) - 1900;
        t1.tm_mon  = stoi(dataUser[i - 1].date.substr(5, 2)) - 1;
        t1.tm_mday = stoi(dataUser[i - 1].date.substr(8, 2));

        t2.tm_year = stoi(dataUser[i].date.substr(0, 4)) - 1900;
        t2.tm_mon  = stoi(dataUser[i].date.substr(5, 2)) - 1;
        t2.tm_mday = stoi(dataUser[i].date.substr(8, 2));

        time_t time1 = mktime(&t1);
        time_t time2 = mktime(&t2);

        double selisihDetik = difftime(time2, time1);
        long long selisihHari = selisihDetik / 86400;

        if (selisihHari == 1)
        {
            currentStreak++;
            if (currentStreak > maxStreak) {
                maxStreak = currentStreak;
            }
        }
        else if (selisihHari > 1)
        {
            currentStreak = 1;
        }
    }
    return maxStreak;
}

// Fungsi untuk mengurutkan riwayat data jurnal berdasarkan tanggal secara menaik
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

// Fungsi untuk mengecek apakah user sudah mengunci klaim pencapaian tertentu
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

// Fungsi untuk menyimpan record perolehan pencapaian baru ke file teks
bool simpanAchievement(const string &username, const string &namaAchievement)
{
    ofstream fileStream("data/achievements.txt", ios::app);
    if (!fileStream.is_open())
        return false;

    fileStream << username << "<|>" << namaAchievement << "\n";
    bool success = !fileStream.fail();
    fileStream.close();
    return success;
}

// Fungsi untuk mengevaluasi data jurnal terkini demi membuka pencapaian baru
void cekAchievementBaru(const string &username)
{
    int jmlDataRaw = 0;
    userData *dataUserRaw = searchByUsername("data/userData.txt", username, jmlDataRaw);

    if (jmlDataRaw <= 0)
    {
        delete[] dataUserRaw;
        cout << "Belum ada jurnal, belum ada achievement yang bisa dicek.\n";
        return;
    }

    urutkanByTanggal(dataUserRaw, jmlDataRaw);

    userData *dataUser = new userData[jmlDataRaw];
    int jmlData = 0;
    
    if (jmlDataRaw > 0) {
        dataUser[jmlData++] = dataUserRaw[0];
    }
    
    for (int i = 1; i < jmlDataRaw; i++) {
        if (dataUserRaw[i].date != dataUserRaw[i - 1].date) {
            dataUser[jmlData++] = dataUserRaw[i];
        }
    }

    int totalMaksStreak = hitungMaksimalStreak(dataUser, jmlData);
    cout << YELLOW << "Rekor Streak Menulis Jurnal Kamu: " << totalMaksStreak << " Hari Berturut-turut!\n" << RESET_COLOR;
    cout << "---------------------------------------------------------\n\n";

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

    // Penambahan pengecekan One Year Streak (365 hari berturut-turut)
    if (cekStreak(dataUser, jmlData, 365) && !sudahPunyaAchievement(username, "One Year Streak"))
    {
        simpanAchievement(username, "One Year Streak");
        cout << GREEN << "[!] LUAR BIASA! Kamu konsisten menulis jurnal selama 1 tahun (365 hari) berturut-turut!" << RESET_COLOR << "\n";
        adaYgBaru = true;
    }

    if (!adaYgBaru)
        cout << YELLOW << "Tidak ada achievement baru." << RESET_COLOR << "\n";

    delete[] dataUserRaw;
    delete[] dataUser;
}

// Fungsi untuk menayangkan seluruh daftar pencapaian yang telah dikoleksi user
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

// Fungsi untuk mengatur percabangan opsi navigasi menu sistem pencapaian
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
                getch();
                break;
            case 1:
                CLEAR_SCREEN;
                SHOW_CURSOR;
                tampilkanAchievement(currentUser);
                cout << "\nPRESS ANY KEY TO RETURN...";
                HIDE_CURSOR;
                getch();
                break;
            case 2:
                running = false;
                break;
            }
            break;
        }
    }
}