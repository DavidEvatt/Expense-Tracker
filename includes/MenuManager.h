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
#include <algorithm>
#include <fstream>
#include <cstdio>
#include <conio.h> //windows key library. also need linux
#include <iomanip>


#include "PrettyColors.h"
#include "Item.h"
#include "ActionValidator.h"

class MenuManager
{
    private:
        int intInput;
        string stringInput;
        double doubleInput;
        int year = getCurrentYear();

        bool* _RUNNING;
        map<int, vector<Item>>* _ITEMS;

        PrettyColors _COLORMANAGER;
        ActionValidator _VALIDATOR;

        //Key Values
        #define KEY_UP        72
        #define KEY_DOWN      80
        #define KEY_LEFT      75
        #define KEY_RIGHT     77
        #define KEY_ESCAPE    27
        #define KEY_ENTER     13
        #define KEY_BACKSPACE  8

    public:
        MenuManager(bool& running, map<int, vector<Item>>& items);
        void printMenu();
        Item addItemMenu();
        void populateOtherItems(Item _item);
        void selectViewMonths();
        void monthyView(int _months);
            /*  
                Splits into
                1. View by 1 Month
                2. View by 3 Month
                4. View by 6 Months
                5. View by Year

                each view should be seen by weeks and from their you can
                press enter on a week to open it into a daily view.
            */
           /*
           void viewByWeek(int _month, int _day);
           void viewByMonth(int _month);
           void viewByThreeMonths(int _month);
           void viewBySixMonths(int _month);
           void viewByYear(int _month);*/

        int getYear(){return year;};

        static int getCurrentYear()
        {
            const std::time_t now = std::time(nullptr);
            std::tm local_time{};

            if (localtime_s(&local_time, &now) != 0)
                return 0;

            return local_time.tm_year + 1900;
        };

        static int getCurrentMonth()
        {
            const std::time_t now = std::time(nullptr);
            std::tm local_time{};

            if (localtime_s(&local_time, &now) != 0)
                return 0;

            
            return local_time.tm_mon + 1;
        }

        static int getCurrentDay()
        {
            const std::time_t now = std::time(nullptr);
            std::tm local_time{};

            if (localtime_s(&local_time, &now) != 0)
                return 0;

            return local_time.tm_mday;
        }
};

#endif