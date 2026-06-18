#include <iostream>
#include <string>
#include <fstream>
#include "../include/util.hpp"

int main()
{
    bool stop = false;

    while (!stop)
    {
        loginInterface(stop);

        if (!stop)
            mainMenuInterface(stop);
    }

    SHOW_CURSOR;
    return 0;
}
