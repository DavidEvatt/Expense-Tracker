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

#include "MenuManager.h"
#include "sqlite3.h"

//globals
bool _RUNNING = true; //what allows everything to run in a loop until the user exits
map<int, vector<Item>> _ITEMS; //map of month to vector of items
sqlite3* db;

PrettyColors _COLORMANAGER;

//Prototypes
void loadSQLDataBase();
void saveToSQLDataBase();

int main()
{
    cout << std::fixed << std::setprecision(2);

    //Attempt to open data base
    int exit = sqlite3_open("Data/myDataBase.db", &db);

    if (exit) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return (1);
    } else {
        std::cout << "Opened database successfully!" << std::endl;
    }

    loadSQLDataBase();

    _RUNNING = true;
    MenuManager manager(_RUNNING, _ITEMS);

    while(_RUNNING)
    {
        _COLORMANAGER.clearScreen();
        manager.printMenu();
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
   
   
    saveToSQLDataBase();

    sqlite3_close(db);

    _COLORMANAGER.clearScreen();
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

