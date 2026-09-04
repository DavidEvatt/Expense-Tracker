#include <iostream>
    using std::cout;
    using std::cin;
#include <vector>
    using std::vector;
#include <map>
    using std::map;
#include "MenuManager.h"

//globals

bool _RUNNING = true; //what allows everything to run in a loop until the user exits
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

    for(size_t i = 0; i < _ITEMS[8].size(); i++)
    {
        cout << _ITEMS[8].at(i) << "\n";
    }

    for(size_t i = 0; i < _ITEMS[9].size(); i++)
    {
        cout << _ITEMS[9].at(i) << "\n";
    }

    _COLORMANAGER.clearScreen();
    cout << "Exiting program...\n";
    _COLORMANAGER.pauseTerminal(1);

    return 0;
};
