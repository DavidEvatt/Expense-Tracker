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
map<int, vector<Item>> _ITEMS; //map of month to vector of items
sqlite3* db;

PrettyColors _COLORMANAGER;

int _DAY_CREATED = 0;
int _MONTH_CREATED = 0;
int _YEAR_CREATED = 0;
int _TIMES_OPENED = 0;
string _NAME = "";
string _PASS_HASH = "";
string _SECURITY_HASH = "";

//Prototypes
void loadSQLDataBase();
void saveToSQLDataBase();
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
    //create files on loading
    std::filesystem::path filePath = "Data/myDataBase.db";
    std::filesystem::path statusPath = "Data/status.json";
    if (!std::filesystem::exists(filePath)) 
    {
        // Create Directory
        std::filesystem::create_directories(filePath.parent_path());

        // Create sqlite database
        std::ofstream outFile(filePath);
        if (outFile.is_open())
        {
            outFile.close();
        }
    }

    if (!std::filesystem::exists(statusPath)) 
    {
        // Create directory
        std::filesystem::create_directories(statusPath.parent_path());

        // create userInformation
        std::ofstream outFile(statusPath);
        if (outFile.is_open())
        {
            outFile << "{}";
            outFile.close();
        }
    }

    //Attempt to open data base
    int exit = sqlite3_open("Data/myDataBase.db", &db);

    if (exit) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return (1);
    } else {
        std::cout << "Opened database successfully!" << std::endl;
    }
    
    if(!loadStats())
    {
        //either file was not created or there was no data in the file
        firstTimeUser();
        saveUser();
    }

    if(login())
    {
        loadSQLDataBase();
        _RUNNING = true;
        MenuManager manager(_RUNNING, _ITEMS);

        while(_RUNNING)
        {
            _COLORMANAGER.clearScreen();
            manager.printMenu();
        }

        _COLORMANAGER.clearScreen();
        saveToSQLDataBase();
        saveUser();
    }

    else
    {
        cout << _COLORMANAGER.RED << "!! INVALID PASSWORD !!" << _COLORMANAGER.CLEAR_TEXT;
    }

    /*
    for(int k = 1; k <= 12; k++)
    {
        if(_ITEMS.find(k) != _ITEMS.end())
        {
            for(size_t i = 0; i < _ITEMS[k].size(); i++)
            {
                cout << _ITEMS[k].at(i) << "\n";
            }
        }
    }

    */
   
    sqlite3_close(db);
    db = nullptr;

    cout << "Exiting program...\n";
    _COLORMANAGER.pauseTerminal(1);

    return 0;
};

void saveToSQLDataBase()
{
    bool conti = true;

    //Error message variable
    char* errMsg = nullptr;

    //Create Table if it doesnt exist
    const char* createTableSQL = "CREATE TABLE IF NOT EXISTS items ("
                             "occurs TEXT, "
                             "month INTEGER, "
                             "day INTEGER, "
                             "amount DOUBLE, "
                             "inc INTEGER, "
                             "name TEXT, "
                             "year INTEGER);";

    int rc = sqlite3_exec(db, createTableSQL, nullptr, nullptr, &errMsg);
    if(rc != SQLITE_OK)
    {
        std::cerr << "SQL error creating table: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        conti = false;
    }
    
    /*
        You have to prepare the vakues to be ready
        which will be seperated by commas
    */

    //write everything at once instead of file by file
    sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, &errMsg);

    //how to write the thing
    const char* sql = "INSERT INTO items (occurs, month, day, amount, inc, name, year) VALUES (?, ?, ?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Prepare Failed: " << sqlite3_errmsg(db) << "\n";
        conti = false;
    }

    if(conti)
    {
        //bind object into data
        for(int k = 1; k <= 12; k++)
        {
            if(_ITEMS.find(k) != _ITEMS.end())
            {
                for(size_t i = 0; i < _ITEMS[k].size(); i++)
                {
                     /**
                     *  (occurs, month, day, amount, inc,  name,   year)
                     *   String, int,   int, double, bool, string, int
                     */
                    Item objectToBind = _ITEMS[k].at(i);

                    sqlite3_bind_text(stmt, 1, objectToBind.getOccurance().c_str(), -1, SQLITE_TRANSIENT);
                    sqlite3_bind_int(stmt, 2, objectToBind.getMonth());
                    sqlite3_bind_int(stmt, 3, objectToBind.getDay());
                    sqlite3_bind_double(stmt, 4, objectToBind.getAmt());
                    sqlite3_bind_int(stmt, 5, objectToBind.getInc()); // returns 0 or 1
                    sqlite3_bind_text(stmt, 6, objectToBind.getName().c_str(), -1, SQLITE_TRANSIENT);
                    sqlite3_bind_int(stmt, 7, objectToBind.getYear());

                    //execute the thing
                    sqlite3_step(stmt);

                    //reset the thing
                    sqlite3_reset(stmt);
                }
            }
        }

        //finalize the thing
        sqlite3_finalize(stmt);

        //commit the thing to save
        sqlite3_exec(db, "COMMIT;", nullptr, nullptr, &errMsg);
    }
};

void loadSQLDataBase()
{
    bool conti = true;
    // Loading a data base
        //clear the map
        _ITEMS.clear();
    
    const char* dbSQL = "SELECT occurs, month, day, amount, inc, name, year FROM items;";
    sqlite3_stmt* stmt2;

    if(sqlite3_prepare_v2(db, dbSQL, -1, &stmt2, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare select statement: " << sqlite3_errmsg(db) << std::endl;
        conti = false;
    }

    if(conti)
    {
        while (sqlite3_step(stmt2) == SQLITE_ROW) {
            std::string occurs = reinterpret_cast<const char*>(sqlite3_column_text(stmt2, 0));
            int month          = sqlite3_column_int(stmt2, 1);
            int day            = sqlite3_column_int(stmt2, 2);
            double amount      = sqlite3_column_double(stmt2, 3);
            bool inc           = sqlite3_column_int(stmt2, 4) == 1;
            std::string name   = reinterpret_cast<const char*>(sqlite3_column_text(stmt2, 5));
            int year           = sqlite3_column_int(stmt2, 6);

            Item loadedItem(occurs, month, day, amount, inc, name, year);
            
            // Use the actual database ID as the map key
            _ITEMS[month].push_back(loadedItem);
            cout << "Added item to the " << std::to_string(month) << " month\n";
            cout << loadedItem;
            //sorts the alogroithm by days after adding it in
            std::sort(_ITEMS[month].begin(), _ITEMS[month].end());
        }

        sqlite3_finalize(stmt2);
    }


    //End loading data base
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

        