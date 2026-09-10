#include "MenuManager.h"


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
        int maxChoices = 4;
        cout << "-----------------------------\n";
        cout << "1. Add Item\n";
        cout << "2. View Items\n";
        cout << "3. Delete Database\n";
        cout << "4. Exit\n";
        cout << "-----------------------------\n";
        cout << "Please select an option: ";

        int option;
        cin >> option;
    
        while(cin.fail() || !_VALIDATOR.checkValidInput(option, 1, maxChoices))
        {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            cout << _COLORMANAGER.RED << "Invalid option. Please select an option between 1 and " << maxChoices << ": " << _COLORMANAGER.CLEAR_FORMAT;
            cin >> option;
        }

        switch(option)
        {
            case 1:
            {
                addItemMenu();
                break;
            }

            case 2:
            {
                _COLORMANAGER.clearScreen();
                selectViewMonths();
                break;
            }

            case 3:
            {
                std::remove("Data/myDataBase.db");
                std::ofstream outFile("Data/myDataBase.db");
                if(outFile.is_open())
                {
                    outFile.close();
                }

                _ITEMS->clear();
                break;
            }

            case 4:
            {
                *_RUNNING = false;
                _RUNNING = nullptr;
                break;
            }
        }
    }
};

/**
 * @brief Displays the menu for adding a new item and handles user input for item details. it also adds it to the _ITEMS map.
 * @return The newly created Item object.
 */
Item MenuManager::addItemMenu()
{
    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    cout << "Lets make a new item!\n-----------------------------\n";
    cout << "Name of item: ";
    string name;
    std::getline(cin, name);

    cout << "Price of item: ";
    double amount; 
    cin >> amount;
    cin.ignore();
    
    while(cin.fail())
    {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        cout << _COLORMANAGER.RED << "Invalid input. Please enter a real number: " << _COLORMANAGER.CLEAR_FORMAT;
        cin >> amount;
    }

    cout << "Is this an income or expense? (1 for income, 0 for expense): ";
    int inc;
    cin >> inc;
    cin.ignore();


    while(!_VALIDATOR.checkValidInput(inc, 0, 1))
    {
        cout << _COLORMANAGER.RED << "Invalid input. Please enter 1 for income or 0 for expense: " << _COLORMANAGER.CLEAR_FORMAT;
        cin >> inc;
    }

    cout << "What month does this item occur? (1-12): ";
    int month;
    cin >> month;
    cin.ignore();

    while(!_VALIDATOR.checkMonths(month))
    {
        cout << _COLORMANAGER.RED << "Invalid month. Please enter a month between 1 and 12: " << _COLORMANAGER.CLEAR_FORMAT;
        cin >> month;
    }

    int maxDays = _VALIDATOR.getMaxDays(month);

    cout << "What day does this item occur? (1- " << std::to_string(maxDays) << "): ";
    int day;
    cin >> day;
    cin.ignore();

    while(!_VALIDATOR.checkDays(day, month))
    {
        cout << _COLORMANAGER.RED << "Invalid day. Please enter a day between 1 and " << std::to_string(maxDays) << ": " << _COLORMANAGER.CLEAR_FORMAT;
        cin >> day;
    }

    cout << "How often does this item occur? (ONCE, DAILY, WEEKLY, BIWEEKLY, MONTHLY, QUARTERLY, SEMIANNUALLY, ANNUALLY): ";
    string occurs;
    cin >> occurs;
    cin.ignore();

    cout << _VALIDATOR.checkOccurance(occurs) << "\n";
    while(!_VALIDATOR.checkOccurance(occurs))
    {
        cout << "Inside loop" + _VALIDATOR.checkOccurance(occurs) << "\n";      
        cout << _COLORMANAGER.RED << "Invalid occurance. Please enter one of the following: ONCE, DAILY, WEEKLY, BIWEEKLY, MONTHLY, QUARTERLY, SEMIANNUALLY, ANNUALLY: " << _COLORMANAGER.CLEAR_FORMAT;
        cin >> occurs;
    }

    Item newItem = Item(occurs, month, day, amount, inc, name, year);
    cout << _COLORMANAGER.GREEN << "Item added!\n" << _COLORMANAGER.CLEAR_FORMAT;
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
void MenuManager::selectViewMonths()
{
    cout << std::left;

    bool here = true;
    _COLORMANAGER.clearScreen();
    string ary[4] = {"One", "Three", "Six", "Twelve"};
    int optionIndex = 0;
    int spacing = 10;
    while(here)
    {
        cout << "How many months would you like to see?\n(Esc to return to main menu)\n-----------------------------\n";
        
        for(int i = 0; i < 4; i++)
        {
            // Calculate how many regular trailing spaces this word needs to hit 15 chars
            size_t spacesNeeded = spacing - ary[i].length();
            string padding(spacesNeeded, ' ');

            if(optionIndex == i)
            {
                cout << _COLORMANAGER.BLUE_BKG << ary[i] << _COLORMANAGER.CLEAR_FORMAT << padding;
            }
            else
            {
                // Normal text followed by regular padding spaces
                cout << ary[i] << padding;
            }
        }

        int ch = _getch(); //first key press

        //is it a special character
        if(ch == 0 || ch == 224)
        {
            //grab keycode
            ch = _getch();

            switch(ch)
            {
                case KEY_RIGHT:
                {
                    if(optionIndex < 4)
                    {
                        optionIndex++;
                    }
                    
                    break;
                }

                case KEY_LEFT:
                {
                    if(optionIndex > 0)
                    {
                        optionIndex--;
                    }
                    break;
                }
            }
        } 

        //check for enter key. use the optionIndex to load the proper view
        else if (ch == '\n' || ch == '\r' || ch == KEY_ENTER) 
        {
            if(ary[optionIndex] == "One")
            {
                monthyView(1);
            }  
            
            else if(ary[optionIndex] == "Three")
            {
                monthyView(3);
            }  

            else if(ary[optionIndex] == "Six")
            {
                monthyView(6);
            }  

            else if(ary[optionIndex] == "Twelve")
            {
                monthyView(12);
            }  

            here = false;
            break;
        }

        //check for backspace or esc
        else if(ch == KEY_BACKSPACE || ch == KEY_ESCAPE || ch == '\b')
        {
            here = false;
        }

        _COLORMANAGER.clearScreen();
    }


    /*
    for(int k = 1; k <= 12; k++)
    {
        if(_ITEMS->find(k) != _ITEMS->end())
        {
            for(size_t i = 0; i < (*_ITEMS)[k].size(); i++)
            {
                cout << (*_ITEMS)[k].at(i) << "\n";
            }
        }
    }*/
};

void MenuManager::monthyView(int _months)
{
    int monthIn = getCurrentMonth();
    cout << "MONTH IS " << std::to_string(monthIn);

    cout << std::left;

    vector<int> dates;

    bool here = true;
    _COLORMANAGER.clearScreen();
    int optionIndex = 0;
    int spacing = 10;

    int monthOn = 0;

    for(int i = 0; i < _months; i++)
    {
        dates.push_back(1);
        dates.push_back(8);
        dates.push_back(15);
        dates.push_back(22);
    }

    while(here)
    {
        cout << "Hover over a week to see purchases?\n(Esc to return to main menu)\n-----------------------------\n";
        
        //printing out your week options
        for(size_t i = 0; i < dates.size(); i++)
        {
            // Calculate how many regular trailing spaces this word needs to hit 15 chars
            size_t spacesNeeded = spacing - 4;
            string padding(spacesNeeded, ' ');


            if(optionIndex == i)
            {
                cout << _COLORMANAGER.BLUE_BKG << std::to_string(monthIn) << "/" << std::to_string(dates.at(i)) << _COLORMANAGER.CLEAR_FORMAT << padding;
                if(monthOn != monthIn){monthOn = monthIn;}
            }
            else
            {
                // Normal text followed by regular padding spaces
                cout << std::to_string(monthIn) << "/" << std::to_string(dates.at(i)) << padding;
            }

            if(dates.at(i) == 22)
            {
                monthIn++;
                if(monthIn >= 12){monthIn = 1;}
            }

            if(i % 12 == 0 && i != 0)
            {
                cout << "\n";
            }
        }
        
        monthIn = getCurrentMonth();

        //print out the events
       
        for(size_t i = 0; i < (*_ITEMS)[monthOn].size(); i++)
        {
            if((*_ITEMS)[monthOn].at(i).getMonth() == monthOn)
            {
                if(dates.at(optionIndex) == 22 || optionIndex == dates.size() -1)
                {
                    //grab until the end of the month
                    if((*_ITEMS)[monthOn].at(i).getDay() >= dates.at(optionIndex) && (*_ITEMS)[monthOn].at(i).getDay() < _VALIDATOR.getMaxDays(monthOn))
                    {
                        cout << "\n" << (*_ITEMS)[monthOn].at(i);
                    }
                }

                else
                {
                    //grab a normal week
                    if((*_ITEMS)[monthOn].at(i).getDay() >= dates.at(optionIndex) && (*_ITEMS)[monthOn].at(i).getDay() < dates.at(optionIndex + 1))
                    {
                        cout << "\n" << (*_ITEMS)[monthOn].at(i);
                    }
                }
            }
        }
        

        int ch = _getch(); //first key press

        //is it a special character
        if(ch == 0 || ch == 224)
        {
            //grab keycode
            ch = _getch();

            switch(ch)
            {
                case KEY_RIGHT:
                {
                    if(optionIndex < dates.size() -1)
                    {
                        optionIndex++;
                    }
                    
                    break;
                }

                case KEY_LEFT:
                {
                    if(optionIndex > 0)
                    {
                        optionIndex--;
                    }
                    break;
                }
            }
        } 

        //check for enter key. use the optionIndex to load the proper view
        else if (ch == '\n' || ch == '\r' || ch == KEY_ENTER) {
            std::cout << "Enter detected every time!\n";
            break;
        }

        //check for backspace or esc
        else if(ch == KEY_BACKSPACE || ch == KEY_ESCAPE || ch == '\b')
        {
            here = false;
        }

        _COLORMANAGER.clearScreen();
    }

}


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

    
};