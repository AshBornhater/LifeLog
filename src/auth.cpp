#include "../include/utils.hpp"

using namespace std;

// Alokasi variabel global untuk menampung user aktif
string currentUser = "";

// Fungsi untuk mengurai baris file menjadi data akun
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

// Fungsi untuk memvalidasi kredensial login user
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

// Fungsi untuk mengecek ketersediaan username di file penyedia akun
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

// Fungsi untuk menulis data registrasi akun baru ke file
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

// Fungsi untuk menampilkan form input login
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
        getch();
        return true;
    }
    else
    {
        moveCursor(BORDER_ROW + 20, 53);
        cout << RED << "Akun anda tidak Ditemukan / Username atau password salah." << RESET_COLOR;
        moveCursor(BORDER_ROW + 21, 53);
        cout << "PRESS ANY KEY TO RETURN...";
        HIDE_CURSOR;
        getch();
        return false;
    }
}

// Fungsi untuk menampilkan form registrasi akun
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
        getch();
        return false;
    }
    else if (usernameExists(account.username))
    {
        moveCursor(BORDER_ROW + 20, 53);
        cout << RED << "Username sudah digunakan." << RESET_COLOR;
        moveCursor(BORDER_ROW + 21, 53);
        cout << "PRESS ANY KEY TO RETURN...";
        HIDE_CURSOR;
        getch();
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
        getch();
        return true;
    }
    else
    {
        moveCursor(BORDER_ROW + 20, 53);
        cout << RED << "Register gagal." << RESET_COLOR;
        moveCursor(BORDER_ROW + 21, 53);
        cout << "PRESS ANY KEY TO RETURN...";
        HIDE_CURSOR;
        getch();
        return false;
    }
}

// Fungsi untuk mengontrol alur menu utama autentikasi
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

// Fungsi untuk mengenkripsi password dengan kunci NIM
string encryptPassword(const string &plain)
{
    string result = plain;
    for (int i = 0; i < (int)plain.size(); i++)
        result[i] = (char)((plain[i] + NIM_KEY[i % KEY_LENGTH]) % 128);
    return result;
}

// Fungsi untuk merender ringkasan profil akun user aktif
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
    getch();
}