#include <iostream>

using namespace std;

// Asumsi fungsi moveCursor sudah dideklarasikan sebelumnya

void moveCursor(int row, int col)
{
    cout << "\033[" << row << ";" << col << "H";
}

void drawLifeLogLogoLogin(int startRow, int startCol)
{
    moveCursor(5, 60);
    cout << R"(                                            
 _     _  __     _                 
| |   (_)/ _|   | |                
| |    _| |_ ___| |     ___   __ _ 
| |   | |  _/ _ \ |    / _ \ / _` |
| |___| | ||  __/ |___| (_) | (_| |
\_____/_|_| \___\_____/\___/ \__, |
                              __/ |
                             |___/ )";
  
}

int main(){
    //cout << 
    drawLifeLogLogoLogin(5, 30);
}