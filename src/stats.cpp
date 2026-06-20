#include "../include/utils.hpp"

using namespace std;

// Fungsi untuk mengekstrak label format minggu (YYYY-Wxx) dari string tanggal
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

// Fungsi untuk mengekstrak label format bulan (YYYY-MM) dari string tanggal
string getMonthLabel(const string &date)
{
    if (date.size() < 7) return "Unknown";
    return date.substr(0, 7);
}

// Fungsi untuk menggambar grafik batang horizontal pada skala visual tertentu
void printBar(double value, double maxValue, int barWidth)
{
    int filled = (maxValue > 0) ? (int)((value / maxValue) * barWidth) : 0;
    cout << "[";
    for (int i = 0; i < barWidth; i++)
        cout << (i < filled ? "#" : "-");
    cout << "]";
}

// Fungsi untuk mengonversi label bulan angka menjadi nama bulan bahasa Indonesia
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

// Fungsi untuk mencari indeks periode yang ada atau menambahkan baru ke array kontainer
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

// Fungsi untuk mengurutkan data statistik berdasarkan label periode secara menaik
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

// Fungsi untuk menampilkan visualisasi tabel statistik beserta grafik batangnya
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
         << "| Jml |  Rata Mood   | Rata Produktivitas\n";
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

        if (avgMood >= 7.0)      cout << GREEN;
        else if (avgMood <= 3.0) cout << RED;
        else                     cout << YELLOW;
        cout << moodBuf << "/10.00" << RESET_COLOR << " | ";

        if (avgProd >= 7.0)      cout << GREEN;
        else if (avgProd <= 3.0) cout << RED;
        else                     cout << YELLOW;
        cout << prodBuf << "/10.00" << RESET_COLOR << "\n";
    }

    cout << CYAN << "  " << string(68, '=') << RESET_COLOR << "\n";

    cout << "\n" << CYAN << "  [ Grafik Rata-rata Mood (skala 1-10) ]" << RESET_COLOR << "\n";
    for (int i = 0; i < size; i++)
    {
        double avgMood = stats[i].totalMood / stats[i].count;
        string lbl = isWeekly ? stats[i].label : getNamaBulan(stats[i].label);
        while ((int)lbl.size() < 22) lbl += " ";

        char buf[8]; snprintf(buf, sizeof(buf), "%.2f", avgMood);

        if (avgMood >= 7.0)      cout << GREEN;
        else if (avgMood <= 3.0) cout << RED;
        else                     cout << YELLOW;

        cout << "  " << lbl << " ";
        printBar(avgMood, 10.0, 25);
        cout << " " << buf << "/10.00\n";
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
        printBar(avgProd, 10.0, 25);
        cout << " " << buf << "/10.00\n";
        cout << RESET_COLOR;
    }
}

// Fungsi untuk mengelompokkan riwayat rekaman jurnal menjadi metrik berkala
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

// Fungsi untuk menghitung nilai tertinggi, terendah, dan rata-rata seluruh jurnal
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
    if ((totalMood / count) >= 7.0) cout << GREEN;
    else if ((totalMood / count) <= 3.0) cout << RED;
    else cout << YELLOW;
    cout << moodBuf << " / 10.00" << RESET_COLOR << "\n";

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

// Fungsi untuk mengontrol navigasi menu utama peninjauan dashboard statistik
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
                cout << "  |  " << (isWeekly ? "STATISTIK MINGGUAN" : "STATISTIK BULANAN ")
                     << " - " << currentUser;
                int pad = 25 - (int)currentUser.size();
                if (pad < 0) pad = 0;
                cout << string(pad, ' ') << "  |\n";
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
                getch();
            }
            break;
        }
    }
}