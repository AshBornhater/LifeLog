#include "../include/utils.hpp"
#include <iostream>

using namespace std;

#define SHOW_CURSOR cout << "\033[?25h"

int main()
{
    bool stop = false;

    loginInterface(stop);

    if (!stop)
        mainMenuInterface(stop);

    SHOW_CURSOR;
    return 0;
}
