#include "../include/utils.hpp"

using namespace std;

// Fungsi untuk mengurai baris file teks menjadi struct userData
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
    parsedData.mood = boundInput(stoi(tokens[2]), 1, 10); 
    parsedData.productivity = boundInput(stoi(tokens[3]), 1, 10);
    parsedData.note = tokens[4];

    return true;
}

// Fungsi untuk memvalidasi input angka bulat dalam batas tertentu
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

// Fungsi untuk membaca seluruh rekaman data jurnal dari file
userData* readFile(const string &filename, int &outCount)
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

// Fungsi untuk mencari data jurnal berdasarkan username spesifik
userData* searchByUsername(const string &filename, const string &targetUsername, int &outCount)
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

// Fungsi untuk mencari data jurnal user aktif berdasarkan tanggal
userData* searchByDate(const string &filename, const string &targetDateRaw, int &outCount)
{
    string targetDate = trimSpaces(targetDateRaw);
    
    if (targetDate.empty()) {
        outCount = 0;
        return nullptr;
    }

    ifstream fileStream(filename);
    if (!fileStream.is_open())
    {
        cout << RED << "[searchByDate] Tidak bisa membuka '" << filename << "'" << RESET_COLOR << "\n";
        outCount = -1;
        return nullptr;
    }

    int matchCount = 0;
    string currentLine;
    while (getline(fileStream, currentLine))
    {
        if (currentLine.empty()) continue;
        
        if (currentLine.find(currentUser) != string::npos && currentLine.find(targetDate) != string::npos) {
            matchCount++;
        }
    }

    if (matchCount == 0) {
        fileStream.close();
        outCount = 0;
        return nullptr;
    }

    userData *matchedRecords = new userData[matchCount];
    
    fileStream.clear();
    fileStream.seekg(0, ios::beg);

    int idx = 0;
    while (getline(fileStream, currentLine))
    {
        if (currentLine.empty()) continue;

        if (currentLine.find(currentUser) != string::npos && currentLine.find(targetDate) != string::npos) {
            userData parsedRecord;
            if (parseLine(currentLine, parsedRecord)) {
                if (parsedRecord.username == currentUser && parsedRecord.date == targetDate) {
                    matchedRecords[idx++] = parsedRecord;
                }
            }
        }
    }

    fileStream.close();
    outCount = idx;
    
    if (idx == 0) {
        delete[] matchedRecords;
        return nullptr;
    }
    
    return matchedRecords;
}

// Fungsi untuk mencari data jurnal user aktif berdasarkan kategori skala mood
userData* searchByMoodCategory(const string &filename, int categoryOption, int &outCount)
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
            if (categoryOption == 1 && parsedRecord.mood <= 3)
                isMatch = true;
            if (categoryOption == 2 && (parsedRecord.mood >= 4 && parsedRecord.mood <= 6))
                isMatch = true;
            if (categoryOption == 3 && parsedRecord.mood >= 7)
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

// Fungsi untuk mencari data jurnal user aktif berdasarkan nilai produktivitas
userData* searchByProductivity(const string &filename, int targetProd, int &outCount)
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

// Fungsi untuk mencari data jurnal user aktif berdasarkan fragmen kata kunci catatan
userData* searchByKeyword(const string &filename, const string &keywordRaw, int &outCount)
{
    string keyword = trimSpaces(keywordRaw);

    if (keyword.empty()) {
        outCount = 0;
        return nullptr;
    }

    for (int i = 0; i < (int)keyword.length(); i++) {
        keyword[i] = tolower(keyword[i]);
    }

    ifstream fileStream(filename);
    if (!fileStream.is_open())
    {
        cout << RED << "[searchByKeyword] Tidak bisa membuka '" << filename << "'" << RESET_COLOR << "\n";
        outCount = -1;
        return nullptr;
    }

    int matchCount = 0;
    string currentLine;
    while (getline(fileStream, currentLine))
    {
        if (currentLine.empty()) continue;

        if (currentLine.find(currentUser) == string::npos) continue;

        string tempLine = currentLine;
        for (int i = 0; i < (int)tempLine.length(); i++) {
            tempLine[i] = tolower(tempLine[i]);
        }

        if (tempLine.find(keyword) != string::npos) {
            matchCount++;
        }
    }

    if (matchCount == 0) {
        fileStream.close();
        outCount = 0;
        return nullptr;
    }

    userData *matchedRecords = new userData[matchCount];
    
    fileStream.clear();
    fileStream.seekg(0, ios::beg);

    int idx = 0;
    while (getline(fileStream, currentLine))
    {
        if (currentLine.empty() || currentLine.find(currentUser) == string::npos) continue;

        userData parsedRecord;
        if (parseLine(currentLine, parsedRecord)) {
            if (parsedRecord.username == currentUser) {
                string tempNote = parsedRecord.note;
                for (int i = 0; i < (int)tempNote.length(); i++) {
                    tempNote[i] = tolower(tempNote[i]);
                }

                if (tempNote.find(keyword) != string::npos) {
                    matchedRecords[idx++] = parsedRecord;
                }
            }
        }
    }

    fileStream.close();
    outCount = idx;
    
    if (idx == 0) {
        delete[] matchedRecords;
        return nullptr;
    }

    return matchedRecords;
}

// Fungsi untuk menampilkan menu pencarian jurnal interaktif
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
        
        drawOption(searchMenuOption, searchMenuOptionLength, menuChoice, 31);
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
                drawBorder(BORDER_ROW, BORDER_COLUMN, 25, 80);
                drawLifeLogLogoLogin(BORDER_ROW + 2, BORDER_COLUMN + 24);
            }

            switch (menuChoice)
            {
            case 0: 
            {
                moveCursor(BORDER_ROW + 11, 64);
                cout << "===== CARI BERDASARKAN TANGGAL =====\n";

                moveCursor(BORDER_ROW + 13, 53);
                cout << "Tanggal (YYYY-MM-DD):";
                moveCursor(BORDER_ROW + 14, 53);
                cout << "+~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+";
                moveCursor(BORDER_ROW + 15, 53);
                cout << "|                                                        |";
                moveCursor(BORDER_ROW + 16, 53);
                cout << "+~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+";

                SHOW_CURSOR;
                moveCursor(BORDER_ROW + 15, 54);
                string targetDate;
                getline(cin, targetDate);
                HIDE_CURSOR;

                matchedRecords = searchByDate(filename, targetDate, matchCount);
                break;
            }
           case 1: 
            {
                moveCursor(BORDER_ROW + 11, 66);
                cout << "===== CARI BERDASARKAN MOOD =====\n";
                moveCursor(BORDER_ROW + 13, 53);
                cout << "Pilih Kategori Mood:\n";
                moveCursor(BORDER_ROW + 14, 53); cout << "1. Mood Buruk (<= 3)";     
                moveCursor(BORDER_ROW + 15, 53); cout << "2. Mood Biasa (4-6)";       
                moveCursor(BORDER_ROW + 16, 53); cout << "3. Mood Bagus (>= 7)";       
                moveCursor(BORDER_ROW + 18, 53); cout << "Pilihan Kategori (1-3): ";
                
                string inputMood;
                cin >> inputMood;
                clearInputBuffer();

                if (inputMood.length() != 1 || inputMood[0] < '1' || inputMood[0] > '3') {
                    processSearch = false; 
                    moveCursor(BORDER_ROW + 20, 53);
                    cout << RED << "[!] Input tidak valid, masukkan angka bulat 1-3." << RESET_COLOR << "\n";
                    moveCursor(BORDER_ROW + 21, 53);
                    cout << "PRESS ANY KEY TO RETURN...\n";
                    getch();
                    break; 
                }
                
                int moodChoice = inputMood[0] - '0';
                matchedRecords = searchByMoodCategory(filename, moodChoice, matchCount);
                break;
            }
            case 2: 
            {
                moveCursor(BORDER_ROW + 11, 62);
                cout << "===== CARI BERDASARKAN PRODUKTIVITAS =====\n";
                moveCursor(BORDER_ROW + 13, 53);
                cout << "Masukkan produktivitas (1-10): ";
                
                string inputProd;
                cin >> inputProd;
                clearInputBuffer();

                int targetProd = 0;
                
                if (inputProd == "10") {
                    targetProd = 10;
                } else if (inputProd.length() == 1 && inputProd[0] >= '1' && inputProd[0] <= '9') {
                    targetProd = inputProd[0] - '0';
                } else {
                    processSearch = false; 
                    moveCursor(BORDER_ROW + 20, 53);
                    cout << RED << "[!] Input tidak valid, masukkan angka bulat 1-10." << RESET_COLOR << "\n";
                    moveCursor(BORDER_ROW + 21, 53);
                    cout << "PRESS ANY KEY TO RETURN...\n";
                    getch();
                    break;
                }
                
                matchedRecords = searchByProductivity(filename, targetProd, matchCount);
                break;
            }
            case 3: 
            {
                moveCursor(BORDER_ROW + 11, 66);
                cout << "===== CARI BERDASARKAN KATA =====\n";

                moveCursor(BORDER_ROW + 13, 53);
                cout << "Keyword:";
                moveCursor(BORDER_ROW + 14, 53);
                cout << "+~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+";
                moveCursor(BORDER_ROW + 15, 53);
                cout << "|                                                        |";
                moveCursor(BORDER_ROW + 16, 53);
                cout << "+~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+";

                SHOW_CURSOR;
                moveCursor(BORDER_ROW + 15, 54);
                string keyword;
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
                    moveCursor(BORDER_ROW + 20, 53);
                    cout << RED << "[!] Gagal membuka file atau fidfle tidak ditemukan." << RESET_COLOR;
                    moveCursor(BORDER_ROW + 22, 53);
                    cout << "PRESS ANY KEY TO RETURN...";
                    HIDE_CURSOR;
                    getch();  
                }
                else if (matchCount == 0)
                {
                    moveCursor(BORDER_ROW + 20, 53);
                    cout << RED << "Data tidak ditemukan." << RESET_COLOR;
                    moveCursor(BORDER_ROW + 22, 53);
                    cout << "PRESS ANY KEY TO RETURN...";
                    HIDE_CURSOR;
                    getch();

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
                        cout << "Note        : " << GREEN << matchedRecords[i].note << RESET_COLOR"\n";
                    }
                    cout << "\nPRESS ANY KEY TO RETURN...";
                    HIDE_CURSOR;
                    getch();
                }
                delete[] matchedRecords;
            }
            break;
        }
        }
    }
}

// Fungsi untuk mencetak seluruh riwayat jurnal milik user aktif ke layar
void printUserJournal(const string &filename)
{
    int matchCount = 0;
    userData *matchedRecords = searchByUsername(filename, currentUser, matchCount);

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
            cout << "Date        : " << matchedRecords[idx].date << "\n";
            cout << "Mood        : " << matchedRecords[idx].mood << "\n";
            cout << "Productivity: " << matchedRecords[idx].productivity << "\n";
            cout << "Note        : " << GREEN << matchedRecords[idx].note << RESET_COLOR"\n";
        }
    }

    delete[] matchedRecords;
}

// Fungsi untuk menulis satu entri jurnal baru dengan mempertahankan data lama
bool writeFile(const string &filename, const userData &data)
{
    int existingCount = 0;
    userData *existingRecords = readFile(filename, existingCount);
    if (existingCount < 0) existingCount = 0;

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

// Fungsi untuk menghapus nomor entri jurnal tertentu berdasarkan username
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

// Fungsi untuk mengontrol navigasi menu utama fitur jurnal harian
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
        drawOption(jurnalMenuOption, jurnalMenuOptionLength, menuChoice, 31);

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

// Fungsi untuk menampilkan form pengisian dan perekaman entri jurnal baru
void tulisJurnal()
{
    CLEAR_SCREEN;
    SHOW_CURSOR;

    cout << CYAN << "\n\n======== TULIS JURNAL ========" << RESET_COLOR << "\n\n";

    userData journalBuffer;
    journalBuffer.username = currentUser;
    journalBuffer.date = getCurrentDate();

    cout << "Username           : " << journalBuffer.username << "\n";
    cout << "Date               : " << journalBuffer.date << "\n";

    readInt(journalBuffer.mood, 1, 10, "Mood (1-10)        : "); // Batas max diubah ke 10, label disesuaikan
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

    bool ok = writeFile("data/userData.txt", journalBuffer);

    if (ok)
        cout << GREEN << "\nJurnal berhasil disimpan." << RESET_COLOR << "\n";
    else
        cout << RED << "\nGagal menyimpan jurnal" << RESET_COLOR << "\n";

    cout << "\nPRESS ANY KEY TO RETURN...";
    HIDE_CURSOR;
    getch();
}

// Fungsi untuk merender daftar riwayat semua jurnal milik pengguna ke layar
void tampilkanSemuaJurnal()
{
    CLEAR_SCREEN;
    SHOW_CURSOR;

    printUserJournal("data/userData.txt");

    cout << "\nPRESS ANY KEY TO RETURN...";
    HIDE_CURSOR;
    getch();
}

// Fungsi untuk memandu pengguna memilih entri jurnal yang ingin dihapus
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
        getch();
        return;
    }

    cout << CYAN << "===== HAPUS JURNAL =====" << RESET_COLOR << "\n";
    for (int i = 0; i < matchCount; i++)
    {
        cout << "\n--- Journal " << i + 1 << " ---\n";
        cout << "Date        : " << userJournals[i].date << "\n";
        cout << "Mood        : " << userJournals[i].mood << "\n";
        cout << "Productivity: " << userJournals[i].productivity << "\n";
        cout << "Note        : " << GREEN << userJournals[i].note << RESET_COLOR"\n";
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
    getch();
}