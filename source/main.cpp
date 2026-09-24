#include <iostream>
    using std::cout;
    using std::cin;
#include <vector>
    using std::vector;
#include <map>
    using std::map;
#include <algorithm>
#include <string>
#include <iomanip>
#include <filesystem>
#include <fstream>

#include "MenuManager.h"
#include "sqlite3.h"
#include "json.hpp"

//globals
bool _RUNNING = true; //what allows everything to run in a loop until the user exits

PrettyColors _COLORMANAGER;

int _DAY_CREATED = 0;
int _MONTH_CREATED = 0;
int _YEAR_CREATED = 0;
int _TIMES_OPENED = 0;
string _NAME = "";
string _PASS_HASH = "";
string _SECURITY_HASH = "";

//Prototypes
bool loadStats();

void firstTimeUser();
bool login();
void resetPassword();
void saveUser();

static int getCurrentYear();
static int getCurrentMonth();
static int getCurrentDay();

int main()
{
    if(!loadStats())
    {
        //add database creation to this as well
        //either file was not created or there was no data in the file
        firstTimeUser();
        saveUser();
    }

    if(login())
    {
        _RUNNING = true;
        MenuManager manager(_RUNNING);

        while(_RUNNING)
        {
            _COLORMANAGER.clearScreen();
            manager.printMenu();
        }

        _COLORMANAGER.clearScreen();
        saveUser();
    }

    else
    {
        cout << _COLORMANAGER.RED << "!! INVALID PASSWORD !!" << _COLORMANAGER.CLEAR_TEXT;
    }
   
    cout << "Exiting program...\n";
    _COLORMANAGER.pauseTerminal(1);

    return 0;
};

bool loadStats()
{   
    bool returnVal = true;
    const string fileName = "Data/status.json";
    std::ifstream inFile;

    inFile.open(fileName);
    std::ifstream userInfo;

    if(!inFile)
    {
        cout << _COLORMANAGER.RED << "Error opening file.\n" << _COLORMANAGER.CLEAR_TEXT;
        returnVal = false;
        return returnVal;
    }

    nlohmann::json user;
    inFile >> user;

    if(!user["dayCreated"].is_null())
    {
        _DAY_CREATED   = user["dayCreated"];
        _MONTH_CREATED = user["monthCreated"];
        _YEAR_CREATED  = user["yearCreated"];
        _TIMES_OPENED  = user["timesOpened"] + 1;
        _NAME          = user["username"];
        _PASS_HASH     = user["password"];
        _SECURITY_HASH = user["securityAnswer"];
    }

    else
    {
        returnVal = false;
    }

    inFile.close();

    return returnVal;
};

void firstTimeUser()
{
    _DAY_CREATED = getCurrentDay();
    _MONTH_CREATED = getCurrentMonth();
    _YEAR_CREATED = getCurrentYear();
    _TIMES_OPENED = 1;


    cout << "Enter your name\n  -> ";
    cin >> _NAME;

    cout << "Enter a password\n  -> ";
    cin >> _PASS_HASH;

    cout << "Enter a security word\n(This will be used to reset your password)\n  -> ";
    cin >> _SECURITY_HASH;
};

bool login()
{
    bool loggedIn = false;
    int maxAttempts = 3;
    int curAttempt = 0;

    string input = "";

    cout << "Username: " << _NAME << "\n";
    while(curAttempt < maxAttempts)
    {
        if(loggedIn){curAttempt = 4;}
        cout << "Password: ";
        cin >> input;

        if(input == _PASS_HASH)
        {
            loggedIn = true;
            break;
        }

        else
        {
            curAttempt++;
            cout << _COLORMANAGER.RED << "\n!! INVALID PASSWORD!!\n" <<
                 std::to_string(curAttempt) << " of " << std::to_string(maxAttempts)
                 << " attempts!\n\n" << _COLORMANAGER.CLEAR_TEXT;

            if(!loggedIn)
            {
                loggedIn = false;
            }
        }
    }

    if(!loggedIn)
    {
        _COLORMANAGER.clearScreen();
        cout << "Reset password with security phrase (y/n)\n  -> ";
        cin >> input;

        if(input == "y")
        {
            curAttempt = 0;

            while(curAttempt < maxAttempts)
            {
                if(loggedIn){curAttempt = 4;}
                cout << "\nSecurity Phrase: ";
                cin >> input;

                if(input == _SECURITY_HASH)
                {
                    loggedIn = true;
                    resetPassword();
                    break;
                }

                else
                {
                    curAttempt++;
                    cout << _COLORMANAGER.RED << "\n!! INVALID Security Phrase!!\n" <<
                        std::to_string(curAttempt) << " of " << std::to_string(maxAttempts)
                        << " attempts!\n" << _COLORMANAGER.CLEAR_TEXT;

                    if(!loggedIn)
                    {
                        loggedIn = false;
                    }
                }
            }
        }

    }

    return loggedIn;
};

void resetPassword()
{
    cout << "Enter a password\n  -> ";
    cin >> _PASS_HASH;
};

void saveUser()
{
    std::ofstream userInfo;
    if(!userInfo)
    {
        cout << _COLORMANAGER.RED << "Error opening file.\n" << _COLORMANAGER.CLEAR_TEXT;
        exit(1);
    }

    userInfo.open("Data/status.json");
    nlohmann::ordered_json user;

    user["dayCreated"]     = _DAY_CREATED;
    user["monthCreated"]   = _MONTH_CREATED;
    user["yearCreated"]    = _YEAR_CREATED;
    user["timesOpened"]    = _TIMES_OPENED;
    user["username"]       = _NAME;
    user["password"]       = _PASS_HASH;
    user["securityAnswer"] = _SECURITY_HASH;

    userInfo << user;
    std::ofstream outFile("Data/status.json");
    outFile << user.dump(3);

    userInfo.close();
};

static int getCurrentYear()
{
    const std::time_t now = std::time(nullptr);
    std::tm local_time{};

    if (localtime_s(&local_time, &now) != 0)
        return 0;

    return local_time.tm_year + 1900;
}

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

        