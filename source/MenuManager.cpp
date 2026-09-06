#include "MenuManager.h"
#include <algorithm>

MenuManager::MenuManager(bool& running, map<int, vector<Item>>& items)
{
    _RUNNING = &running;
    _ITEMS = &items;
};

/**
 * @brief Displays the main menu and handles user input for menu options.
 */
void MenuManager::printMenu()
{
    if(_RUNNING)
    {
        int maxChoices = 3;
        cout << "-----------------------------\n";
        cout << "1. Add Item\n";
        cout << "2. View Items\n";
        cout << "3. Exit\n";
        cout << "-----------------------------\n";
        cout << "Please select an option: ";

        int option;
        cin >> option;

        while(!_VALIDATOR.checkValidInput(option, 1, maxChoices))
        {
            cout << _COLORMANAGER.RED << "Invalid option. Please select an option between 1 and " << maxChoices << ": " << _COLORMANAGER.DEFAULT;
            cin >> option;
        }

        switch(option)
        {
            case 1:
                addItemMenu();
                break;

            case 2:
                _COLORMANAGER.clearScreen();
                viewItemsMenu();
                break;

            case 3:
                *_RUNNING = false;
                _RUNNING = nullptr;
                break;
        }
    }
};

/**
 * @brief Displays the menu for adding a new item and handles user input for item details. it also adds it to the _ITEMS map.
 * @return The newly created Item object.
 */
Item MenuManager::addItemMenu()
{
    cout << "Lets make a new item!\n-----------------------------\n";
    cout << "Name of item: ";
    string name;
    cin >> name;

    cout << "Price of item: ";
    double amount; 
    cin >> amount;
    
    while(cin.fail())
    {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        cout << _COLORMANAGER.RED << "Invalid input. Please enter a real number: " << _COLORMANAGER.DEFAULT;
        cin >> amount;
    }
    //needs input validation to make sure its a string or double

    cout << "Is this an income or expense? (1 for income, 0 for expense): ";
    int inc;
    cin >> inc;

    while(!_VALIDATOR.checkValidInput(inc, 0, 1))
    {
        cout << _COLORMANAGER.RED << "Invalid input. Please enter 1 for income or 0 for expense: " << _COLORMANAGER.DEFAULT;
        cin >> inc;
    }

    cout << "What month does this item occur? (1-12): ";
    int month;
    cin >> month;

    while(!_VALIDATOR.checkMonths(month))
    {
        cout << _COLORMANAGER.RED << "Invalid month. Please enter a month between 1 and 12: " << _COLORMANAGER.DEFAULT;
        cin >> month;
    }

    int maxDays = _VALIDATOR.getMaxDays(month);

    cout << "What day does this item occur? (1- " << std::to_string(maxDays) << "): ";
    int day;
    cin >> day;

    while(!_VALIDATOR.checkDays(day, month))
    {
        cout << _COLORMANAGER.RED << "Invalid day. Please enter a day between 1 and " << std::to_string(maxDays) << ": " << _COLORMANAGER.DEFAULT;
        cin >> day;
    }

    cout << "How often does this item occur? (ONCE, DAILY, WEEKLY, BIWEEKLY, MONTHLY, QUARTERLY, SEMIANNUALLY, ANNUALLY): ";
    string occurs;
    cin >> occurs;

    cout << _VALIDATOR.checkOccurance(occurs) << "\n";
    while(!_VALIDATOR.checkOccurance(occurs))
    {
        cout << "Inside loop" + _VALIDATOR.checkOccurance(occurs) << "\n";      
        cout << _COLORMANAGER.RED << "Invalid occurance. Please enter one of the following: ONCE, DAILY, WEEKLY, BIWEEKLY, MONTHLY, QUARTERLY, SEMIANNUALLY, ANNUALLY: " << _COLORMANAGER.DEFAULT;
        cin >> occurs;
    }

    Item newItem = Item(occurs, month, day, amount, inc, name, year);
    cout << _COLORMANAGER.GREEN << "Item added!\n" << _COLORMANAGER.DEFAULT;
    _COLORMANAGER.pauseTerminal(1);

    (*_ITEMS)[month].push_back(newItem);
    //sorts the alogroithm by days after adding it in
    std::sort((*_ITEMS)[month].begin(), (*_ITEMS)[month].end());
    populateOtherItems(newItem);
    cout << "Number of items: " << (*_ITEMS)[8].size() << "\n";
    return newItem;
};

/**
 * @brief Shows the different sub-menus / scales for viewing items and handling user input.
 */
void MenuManager::viewItemsMenu()
{
    _COLORMANAGER.clearScreen();
    cout << "What scale would you like to see on?\n-----------------------------\n";
    cout << "[1] Weekly\n";
    cout << "[2] Monthly\n";
    cout << "[3] Three Monthly\n";
    cout << "[4] Semi Annually\n";
    cout << "[5] Annually\n";
};

void MenuManager::populateOtherItems(Item _item)
{
    int curMonth = _item.getMonth();
    int curDay = _item.getDay();
    int curYear = year;

    if(_item.getOccurance() == "DAILY")
    {
        for(int i = 0; i < 365; i++)
        {
            curDay++;
            //check the ammount of days in our moth
            int daysInMonth = _VALIDATOR.getMaxDays(curMonth);
            if(curDay > daysInMonth)
            {
                //if my day im on is greater than the amount of days
                //set curDay to 1 and move to the next month
                curDay = 1;
                curMonth++;

                if(curMonth > 12)
                {
                    //if we go over twelve the fo back to 1 and increase the year by 1
                    curMonth = 1;
                    curYear++;
                }
            }

            Item newItem = Item(_item.getOccurance(), curMonth, curDay, _item.getAmt(), _item.getInc(), _item.getName(), curYear);
            (*_ITEMS)[newItem.getMonth()].push_back(newItem);
            //sorts the alogroithm by days after adding it in
            std::sort((*_ITEMS)[newItem.getMonth()].begin(), (*_ITEMS)[newItem.getMonth()].end());
        }
    }

    else if(_item.getOccurance() == "WEEKLY")
    {
        // 52 weeks in a year
        for(int i = 0; i < 52; i++)
        {
            curDay += 7;
            int daysInMonth = _VALIDATOR.getMaxDays(curMonth);

            if(curDay > daysInMonth)
            {
                curDay -= daysInMonth;
                curMonth++;

                if(curMonth > 12)
                {
                    curMonth = 1;
                    curYear++;
                }
            }

             Item newItem = Item(_item.getOccurance(), curMonth, curDay, _item.getAmt(), _item.getInc(), _item.getName(), curYear);
            (*_ITEMS)[newItem.getMonth()].push_back(newItem);
            //sorts the alogroithm by days after adding it in
            std::sort((*_ITEMS)[newItem.getMonth()].begin(), (*_ITEMS)[newItem.getMonth()].end());
        }
    }

    else if(_item.getOccurance() == "BIWEEKLY")
    {
        // 26 bi weeks in a year
        for(int i = 0; i < 26; i++)
        {
            curDay += 14;
            int daysInMonth = _VALIDATOR.getMaxDays(curMonth);

            if(curDay > daysInMonth)
            {
                curDay -= daysInMonth;
                curMonth++;

                if(curMonth > 12)
                {
                    curMonth = 1;
                    curYear++;
                }
            }

             Item newItem = Item(_item.getOccurance(), curMonth, curDay, _item.getAmt(), _item.getInc(), _item.getName(), curYear);
            (*_ITEMS)[newItem.getMonth()].push_back(newItem);
            //sorts the alogroithm by days after adding it in
            std::sort((*_ITEMS)[newItem.getMonth()].begin(), (*_ITEMS)[newItem.getMonth()].end());
        }
    }

    else if(_item.getOccurance() == "MONTHLY")
    {
        // 12 months in a year
        for(int i = 0; i < 12; i++)
        {
            curMonth++;

            if(curMonth > 12)
            {
                curMonth = 1;
                curYear++;
            }
            

             Item newItem = Item(_item.getOccurance(), curMonth, curDay, _item.getAmt(), _item.getInc(), _item.getName(), curYear);
            (*_ITEMS)[newItem.getMonth()].push_back(newItem);
            //sorts the alogroithm by days after adding it in
            std::sort((*_ITEMS)[newItem.getMonth()].begin(), (*_ITEMS)[newItem.getMonth()].end());
        }
    }
    
    else if(_item.getOccurance() == "QUARTERLY")
    {
        //4 quarters in a year
        for(int i = 0; i < 4; i++)
        {
            curMonth += 3;

            if(curMonth > 12)
            {
                curMonth -= 12;
                curYear++;
            }
            

             Item newItem = Item(_item.getOccurance(), curMonth, curDay, _item.getAmt(), _item.getInc(), _item.getName(), curYear);
            (*_ITEMS)[newItem.getMonth()].push_back(newItem);
            //sorts the alogroithm by days after adding it in
            std::sort((*_ITEMS)[newItem.getMonth()].begin(), (*_ITEMS)[newItem.getMonth()].end());
        }
    }

    else if(_item.getOccurance() == "SEMIANNUALLY")
    {
        //2 Halfs in a year
        for(int i = 0; i < 2; i++)
        {
            curMonth += 6;

            if(curMonth > 12)
            {
                curMonth -= 12;
                curYear++;
            }
            

             Item newItem = Item(_item.getOccurance(), curMonth, curDay, _item.getAmt(), _item.getInc(), _item.getName(), curYear);
            (*_ITEMS)[newItem.getMonth()].push_back(newItem);
            //sorts the alogroithm by days after adding it in
            std::sort((*_ITEMS)[newItem.getMonth()].begin(), (*_ITEMS)[newItem.getMonth()].end());
        }
    }

    else if(_item.getOccurance() == "ANNUALLY")
    {
        curYear++;
            
        Item newItem = Item(_item.getOccurance(), curMonth, curDay, _item.getAmt(), _item.getInc(), _item.getName(), curYear);
        (*_ITEMS)[newItem.getMonth()].push_back(newItem);
        //sorts the alogroithm by days after adding it in
        std::sort((*_ITEMS)[newItem.getMonth()].begin(), (*_ITEMS)[newItem.getMonth()].end());
    }

    
}