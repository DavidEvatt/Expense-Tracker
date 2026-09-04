#ifndef ITEM_h
#define ITEM_h
#include <string>
    using std::string;
#include <iostream>
    using std::cout;

class Item
{
    private:
        int occurs;
        double amount;
        int dayOfWeek;
        int month;
        string itemName;
        int year;
    public:
        enum OCCURANCE
        {
            ONCE,
            DAILY,
            WEEKLY,
            BIWEEKLY,
            MONTHLY,
            QUARTERLY,
            SEMIANNUALLY,
            ANNUALLY
        };
    

        Item(string _occurs, int _month, int _day, double _amount, bool _inc, string _name, int _year);

        //SETTERS
        void setOccurance(string _occurs);
        void setAmt(double _amount, bool _inc);
        void setDay(int _day) {dayOfWeek = _day;};
        void setMonth(int _month) {month = _month;};
        void setName(string _name) {itemName = _name;};

        //GETTERS
        string getOccurance() const;
        double getAmt() const {return amount;};
        int getDay() const {return dayOfWeek;}; 
        int getMonth() const {return month;};
        string getName() const {return itemName;};
        int getYear() const {return year;};

        //Operators
        friend std::ostream& operator<<(std::ostream& os, const Item& _item);
        friend bool operator<(const Item& lhs, const Item& rhs);
};


#endif