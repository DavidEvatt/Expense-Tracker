#ifndef MENUMANGER_H
#define MENUMANAGER_H
#include <string>
    using std::string;
#include <iostream>
    using std::cout;
    using std::cin;
#include <map>
    using std::map;
#include <vector>
    using std::vector;
#include <ctime>


#include "PrettyColors.h"
#include "Item.h"
#include "ActionValidator.h"

class MenuManager
{
    private:
        int intInput;
        string stringInput;
        double doubleInput;

        static int getCurrentYear()
        {
            const std::time_t now = std::time(nullptr);
            std::tm local_time{};

            if (localtime_s(&local_time, &now) != 0)
                return 0;

            return local_time.tm_year + 1900;
        }

        int year = getCurrentYear();

        bool* _RUNNING;
        map<int, vector<Item>>* _ITEMS;

        PrettyColors _COLORMANAGER;
        ActionValidator _VALIDATOR;



    public:
        MenuManager(bool& running, map<int, vector<Item>>& items);
        void printMenu();
        Item addItemMenu();
        void populateOtherItems(Item _item);
        void viewItemsMenu();
            /*  
                Splits into
                1. View by Week
                2. View by Month
                3. View by 3 Months
                4. View by 6 Months
                5. View by Year
            */
           /*
           void viewByWeek(int _month, int _day);
           void viewByMonth(int _month);
           void viewByThreeMonths(int _month);
           void viewBySixMonths(int _month);
           void viewByYear(int _month);*/

        int getYear(){return year;};
};

#endif