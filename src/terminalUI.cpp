#include "../include/utils.hpp"
#include <ctime>

using namespace std;

// Fungsi untuk memetakan input tombol panah dan enter dari terminal 
int getKey()
{
    int key = getch();
    if (key == 0 || key == 0xE0)
    {
        key = getch();
        return key;
    }
    return key;
}

// Fungsi untuk memindahkan posisi kursor terminal ke baris dan kolom tertentu
void moveCursor(int row, int col)
{
    cout << "\033[" << row << ";" << col << "H";
}

// Fungsi untuk membersihkan sisa data atau karakter dari buffer input cin
void clearInputBuffer()
{
    cin.ignore(1000000, '\n');
}

// Fungsi untuk membatasi nilai input numerik agar tetap berada dalam range tertentu
int boundInput(int val, int low, int high)
{
    if (val < low)
        return low;
    if (val > high)
        return high;
    return val;
    cout << " ";
}

// Fungsi untuk memotong karakter spasi di awal dan di akhir string
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

// Fungsi untuk mengekstrak potongan teks selanjutnya berdasarkan pembatas DELIM
string nextToken(const string &line, size_t &position)
{
    size_t found = line.find(DELIM, position);
    if (found == string::npos)
        return "";
    string token = line.substr(position, found - position);
    position = found + DELIM_LEN;
    return token;
}

// Fungsi untuk mengambil tanggal sistem saat ini dalam format YYYY-MM-DD
string getCurrentDate()
{
    time_t now = time(0);
    tm *localTime = localtime(&now);
    char dateFormatted[11];

    strftime(dateFormatted, sizeof(dateFormatted), "%Y-%m-%d", localTime);

    return dateFormatted;
}

// Fungsi untuk menggambar bingkai kotak UI pada koordinat tertentu
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

// Fungsi untuk merender logo ASCII art aplikasi LifeLog pada halaman login
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

// Fungsi untuk merender komponen kotak teks input username dan password
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

// Fungsi untuk merender daftar tombol pilihan menu navigasi dengan highlight warna
void drawOption(const string *arrOptions, int optionslength, int userChoose, int borderHeight)
{
    drawBorder(BORDER_ROW, BORDER_COLUMN, borderHeight, 80);
    drawLifeLogLogoLogin(BORDER_ROW + 2, BORDER_COLUMN + 24);
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

// Fungsi untuk mengalokasikan ulang memori array dinamis struct userData
userData* resizeRecords(userData *oldArr, int oldSize, int newSize)
{
    userData *newArr = new userData[newSize];
    for (int idx = 0; idx < oldSize; idx++)
    {
        newArr[idx] = oldArr[idx];
    }
    delete[] oldArr;
    return newArr;
}

// Fungsi untuk mengontrol alur kemudi navigasi menu utama aplikasi
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

// Fungsi untuk mencetak kredensial pengembang kelompok aplikasi LifeLog v1.0
void about()
{
    CLEAR_SCREEN;
    SHOW_CURSOR;

    cout << CYAN << "\n\n========== ABOUT ==========\n\n" << RESET_COLOR;
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
    cout << "1. Vidia Iqmalia Putri                  (F1D02510029)\n";
    cout << "2. Lale Juliesti Ayuningkusuma          (F1D02510064)\n";
    cout << "3. Lalu Muhammad Izza Abda Maulana      (F1D02510067)\n";
    cout << "4. Muhammad Bijak Rizqullah             (F1D02510081)\n";
    cout << "5. Zidny Ilma                           (F1D02510100)\n";
    cout << "6. Andika Ramdhanu                      (F1D02510104)\n";
    cout << "7. Nurlaeli Hidayati                    (F1D02510129)\n";

    cout << "\nUniversitas Mataram\n";
    cout << "Program Studi Teknik Informatika\n";

    cout << "\nPRESS ANY KEY TO RETURN...";
    HIDE_CURSOR;
    getch();
}