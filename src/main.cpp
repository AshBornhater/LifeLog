#include <iostream>
#include <string>
#include <fstream>
#include "../include/util.hpp"

int main()
{
    bool stop = false;
    while (!stop)
    {
        const std::string mainMenuOption[] = {};
        loginInterface(stop);
        mainMenuInterface(stop);
        moveCursor(50, 1);
    }
    SHOW_CURSOR;
    return 0;
}
