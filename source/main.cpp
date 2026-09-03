#include <iostream>
    using std::cout;
    using std::cin;
#include <vector>
    using std::vector;
#include <map>
    using std::map;
#include "MenuManager.h"

//globals

bool _RUNNING = true;
map<int, vector<Item>> _ITEMS; //map of month to vector of items

PrettyColors _COLORMANAGER;

int main()
{
    _RUNNING = true;
    MenuManager manager(_RUNNING, _ITEMS);

    while(_RUNNING)
    {
        _COLORMANAGER.clearScreen();
        manager.printMenu();
    }

    _COLORMANAGER.clearScreen();
    cout << "Exiting program...\n";
    _COLORMANAGER.pauseTerminal(1);

    return 0;
};
