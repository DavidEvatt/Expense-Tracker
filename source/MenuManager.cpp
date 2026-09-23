#include "MenuManager.h"


MenuManager::MenuManager(bool& running)
{
    _RUNNING = &running;

    //Attempt to open data base
    int exit = sqlite3_open("Data/myDataBase.db", &_DB);

    if (exit) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(_DB) << std::endl;
    } else {
        std::cout << "Opened database successfully!" << std::endl;
    }
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

                //Create Table if it doesnt exist
                char* errMsg = nullptr;
                const char* createTableSQL = "CREATE TABLE IF NOT EXISTS items ("
                                        "occurs TEXT, "
                                        "month INTEGER, "
                                        "day INTEGER, "
                                        "amount DOUBLE, "
                                        "inc INTEGER, "
                                        "name TEXT, "
                                        "year INTEGER);";

                int rc = sqlite3_exec(_DB, createTableSQL, nullptr, nullptr, &errMsg);
                if(rc != SQLITE_OK)
                {
                    std::cerr << "SQL error creating table: " << errMsg << std::endl;
                    sqlite3_free(errMsg);
                    sqlite3_close(_DB);
                }

                errMsg = nullptr;
                createTableSQL = nullptr;

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
    
    populateOtherItems(newItem);
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
    int itemIndex = 0;
    int spacing = 10;

    int monthOn = 0;
    int addedItemNum = 0;

    vector<Item> listOfWeek;

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

        addedItemNum = 0;
    //ADDING ITEMS from data base
        // 1. The act i want to perforn
        const char* sql = "SELECT occurs, month, day, amount, inc, name, year "
                        "FROM items "
                        "WHERE month = ? AND day BETWEEN ? AND ?;";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(_DB, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(_DB) << std::endl;
            //clean nenory and stio running
        }

        // 2. Bind the 3 variables to the '?' placeholders (1-indexed)
        sqlite3_bind_int(stmt, 1, monthOn); // First ?: month
        //grab days variable
        int startDay = 0;
        int endDay = 0;
        
        if(dates.at(optionIndex) == 22 || optionIndex == dates.size() -1)
        {
            startDay = dates.at(optionIndex);
            endDay = _VALIDATOR.getMaxDays(monthOn);
        }

        else
        {
            startDay = dates.at(optionIndex);
            endDay = dates.at(optionIndex + 1) -1;
        }
        
        sqlite3_bind_int(stmt, 2, startDay);    // Second ?: start of day range (e.g., 7)
        sqlite3_bind_int(stmt, 3, endDay);      // Third ?: end of day range (e.g., 14)

        // 3. Step through the results row by row
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::string occurs = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            int month          = sqlite3_column_int(stmt, 1);
            int day            = sqlite3_column_int(stmt, 2);
            double amount      = sqlite3_column_double(stmt, 3);
            bool inc           = sqlite3_column_int(stmt, 4) != 0;
            std::string name   = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
            int _year           = sqlite3_column_int(stmt, 6);

            listOfWeek.emplace_back(occurs, month, day, amount, inc, name, _year);
        }

        sqlite3_finalize(stmt);
    //END SECTION

        //print all of the events out
        for(size_t i = 0; i < listOfWeek.size(); i++)
        {
            if(itemIndex == i)
            {
                cout << "\n" << _COLORMANAGER.BLUE_BKG << listOfWeek.at(i) << _COLORMANAGER.CLEAR_FORMAT;
            }

            else
            {
                cout << "\n" << listOfWeek.at(i);
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
                        itemIndex = 0;
                        listOfWeek.clear();
                    }
                    
                    break;
                }

                case KEY_LEFT:
                {
                    if(optionIndex > 0)
                    {
                        optionIndex--;
                        itemIndex = 0;
                        listOfWeek.clear();

                    }
                    break;
                }

                case KEY_UP:
                {
                    cout << _COLORMANAGER.MAGENTA << "Key Up Pressed | ItemIndex is | " << std::to_string(itemIndex) 
                         <<  " | Size of list is " << std::to_string(listOfWeek.size()) << _COLORMANAGER.CLEAR_FORMAT << "\n";
                    if(itemIndex > 0)
                    {
                        itemIndex--;
                        cout << _COLORMANAGER.MAGENTA << "ItemIndex is should update | " << std::to_string(itemIndex) << _COLORMANAGER.CLEAR_FORMAT << "\n";
                    }
                    break;
                }

                case KEY_DOWN:
                {
                    cout << _COLORMANAGER.MAGENTA << "Key Down Pressed | ItemIndex is | " << std::to_string(itemIndex) 
                         <<  " | Size of list is " << std::to_string(listOfWeek.size()) << _COLORMANAGER.CLEAR_FORMAT << "\n";
                    if(itemIndex < listOfWeek.size() -1)
                    {
                        itemIndex++;
                        cout << _COLORMANAGER.MAGENTA << "ItemIndex is should update | " << std::to_string(itemIndex) << _COLORMANAGER.CLEAR_FORMAT << "\n";
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
        listOfWeek.clear();
        addedItemNum = 0;
    }

}

/**
 * @brief uses the items occurance to automatically populate the sqldata base with duplicates of itself with different days / months / years
 */
void MenuManager::populateOtherItems(Item _item)
{
    int numInstances = 0; //the number of instances we want to create
    int timeChangeRate = 0; //how many (days / months / years) do we move forward at a time.
    bool atEnd = false; // a varibale that tells us if we are at the end of a month or not.

    int curMonth = _item.getMonth();
    int curDay = _item.getDay();
    int curYear = year;

    if(curDay == _VALIDATOR.getMaxDays(curMonth))
    {
        atEnd = true;
    }

    if(_item.getOccurance() == "DAILY")
    {
        numInstances = 365;
        timeChangeRate = 1;
    }

    else if(_item.getOccurance() == "WEEKLY")
    {
        numInstances = 52;
        timeChangeRate = 7;
    }

    else if(_item.getOccurance() == "BIWEEKLY")
    {
        numInstances = 26;
        timeChangeRate = 14;
    }

    else if(_item.getOccurance() == "MONTHLY")
    {
        numInstances = 12;
        timeChangeRate = _VALIDATOR.getMaxDays(_item.getMonth());
    }
    
    else if(_item.getOccurance() == "QUARTERLY")
    {
        numInstances = 4;
        timeChangeRate = 3; // months
    }

    else if(_item.getOccurance() == "SEMIANNUALLY")
    {
        numInstances = 2;
        timeChangeRate = 6; //months
    }

    else if(_item.getOccurance() == "ANNUALLY")
    {
        numInstances = 2;
        timeChangeRate = 1; //year
    }

    for(int i = 0; i < numInstances; i++)
    {
    //STEP ! | ADD THE ITEM
        //Error message variable
        char* errMsg = nullptr;
        //using char* to prepare the data base to take in a statment to perform
        sqlite3_exec(_DB, "BEGIN TRANSACTION;", nullptr, nullptr, &errMsg);
        const char* sql = "INSERT INTO items (occurs, month, day, amount, inc, name, year) VALUES (?, ?, ?, ?, ?, ?, ?);";
        sqlite3_stmt* stmt;

        //failure
        if(sqlite3_prepare_v2(_DB, sql, -1, &stmt, nullptr) != SQLITE_OK)
        {
            std::cerr << "Prepare Failed: " << sqlite3_errmsg(_DB) << "\n";
            //close pointers to pervent memory leak
            sqlite3_close(_DB);
            sql = nullptr;
            stmt = nullptr;
            _DB = nullptr;
            *_RUNNING = false; //kills the program
        }

        //the item jsut created
        Item objectToBind = Item(_item.getOccurance(), curMonth, curDay, _item.getAmt(), _item.getInc(), _item.getName(), curYear);

        //taking tge value and assigning it to the sql variable. They start at 0 not 1 from some odd reason
        sqlite3_bind_text(stmt, 1, objectToBind.getOccurance().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 2, objectToBind.getMonth());
        sqlite3_bind_int(stmt, 3, objectToBind.getDay());
        sqlite3_bind_double(stmt, 4, objectToBind.getAmt());
        sqlite3_bind_int(stmt, 5, objectToBind.getInc()); // returns 0 or 1
        sqlite3_bind_text(stmt, 6, objectToBind.getName().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 7, objectToBind.getYear());

        //execute the stmt variable
        sqlite3_step(stmt);

        //reset it back to being empty
        sqlite3_reset(stmt);

        //finalize the thing (not 100% sure what this even does
        sqlite3_finalize(stmt);

        //Update the data base
        sqlite3_exec(_DB, "COMMIT;", nullptr, nullptr, &errMsg);

        //prevent memory leaks
        sql = nullptr;
        stmt = nullptr;
        errMsg = nullptr;

    //STEP 2 | USE DAYCHANGE VARIABLE
        if(_item.getOccurance() == "MONTHLY")
        {
            curDay += timeChangeRate;
            if(curDay > _VALIDATOR.getMaxDays(curMonth))
            {
                // this should always return a minimum value of 1 and 
                // should always show how many days we are into the next month
                curDay -= _VALIDATOR.getMaxDays(curMonth);
                curMonth++;
                
                //after we increase our month we check if we are at the end of the year
                if(curMonth > 12)
                {
                    curMonth = 1;
                    curYear++;
                }

                timeChangeRate = _VALIDATOR.getMaxDays(curMonth);
            }
        }

        else if(_item.getOccurance() == "QUARTERLY")
        {
            curMonth += timeChangeRate;
            if(curMonth > 12)
            {
                curMonth -= 12;
                curYear++;
            }

            if(atEnd)
            {
                if(curDay != _VALIDATOR.getMaxDays(curMonth))
                {
                    curDay = _VALIDATOR.getMaxDays(curMonth);
                }
            }
        }

        else if( _item.getOccurance() == "SEMIANNUAL")
        {
            curMonth += timeChangeRate;
            if(curMonth > 12)
            {
                curMonth -= 12;
                curYear++;
            }

            if(atEnd)
            {
                if(curDay != _VALIDATOR.getMaxDays(curMonth))
                {
                    curDay = _VALIDATOR.getMaxDays(curMonth);
                }
            }
        }

        else if( _item.getOccurance() == "ANNUALLY")
        {
            curYear += timeChangeRate;
        }

        else
        {
            curDay += timeChangeRate;
            if(curDay > _VALIDATOR.getMaxDays(curMonth))
            {
                // this should always return a minimum value of 1 and 
                // should always show how many days we are into the next month
                curDay -= _VALIDATOR.getMaxDays(curMonth);
                curMonth++;
                
                //after we increase our month we check if we are at the end of the year
                if(curMonth > 12)
                {
                    curMonth = 1;
                    curYear++;
                }
            }
        }
        

    //STEP 3 | CELEBRATE IF IT WORKS
    }
};

MenuManager::~MenuManager()
{ 
    sqlite3_close(_DB);
    _DB = nullptr;
}