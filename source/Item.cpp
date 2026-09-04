#include "Item.h"
#include <iomanip>

/**
 * @brief Constructs an Item object with the given parameters.
 * @param _occurs A string representation of the enum value for occurs
 * @param _month The month the item occurs (1-12)
 * @param _day The day the item occurs (1-31)
 * @param _amount The amount of the item
 * @param _inc A boolean indicating whether the amount is an income (true) or an expense (false)
 * @example "DAILY", "WEEKLY", "BIWEEKLY", "MONTHLY", "QUARTERLY", "SEMIANNUALLY", "ANNUALLY"
 * @param _name The name of the item
 */
Item::Item(string _occurs, int _month, int _day, double _amount, bool _inc, string _name, int _year)
{
    //Checks if the item price/amount needs to be positive or negative
    if(_inc)
    {
        if(_amount < 0)
        {
            amount = _amount * -1;
        }

        else
        {
            amount = _amount;
        }
    }

    else
    {
        if(amount > 0)
        {
            amount = _amount * -1;
        }

        else
        {
            amount = _amount;
        }
    }

    //When do we need to set our enum to based on _occurs
    if(_occurs == "ONCE")
    {
        occurs = ONCE;
    }

    else if(_occurs == "DAILY")
    {
        occurs = DAILY;
    }

    else if(_occurs == "WEEKLY")
    {
        occurs = WEEKLY;
    }

    else if(_occurs == "BIWEEKLY")
    {
        occurs = BIWEEKLY;
    }

    else if(_occurs == "MONTHLY")
    {
        occurs = MONTHLY;
    }

    else if(_occurs == "QUARTERLY")
    {
        occurs = QUARTERLY;
    }

    else if(_occurs == "SEMIANNUALLY")
    {
        occurs = SEMIANNUALLY;
    }

    else if(_occurs == "ANNUALLY")
    {
        occurs = ANNUALLY;
    }

    dayOfWeek = _day;
    month = _month;
    itemName = _name;
    year = _year;
    inc = _inc;
};

/**
 * @brief Sets how often an item occurs
 * @param _occurs A string representation of the enum value for occurs
 * @example "DAILY", "WEEKLY", "BIWEEKLY", "MONTHLY", "QUARTERLY", "SEMIANNUALLY", "ANNUALLY"
 */
void Item::setOccurance(string _occurs)
{
    if(_occurs == "ONCE")
    {
        occurs = ONCE;
    }
    else if(_occurs == "DAILY")
    {
        occurs = DAILY;
    }

    else if(_occurs == "WEEKLY")
    {
        occurs = WEEKLY;
    }

    else if(_occurs == "BIWEEKLY")
    {
        occurs = BIWEEKLY;
    }

    else if(_occurs == "MONTHLY")
    {
        occurs = MONTHLY;
    }

    else if(_occurs == "QUARTERLY")
    {
        occurs = QUARTERLY;
    }

    else if(_occurs == "SEMIANNUALLY")
    {
        occurs = SEMIANNUALLY;
    }

    else if(_occurs == "ANNUALLY")
    {
        occurs = ANNUALLY;
    }
};

/**
 * @brief Sets the amount of the item, and ensures that the amount is always positive if _inc is true, and negative if _inc is false
 * @param _amount The amount to set the item to
 * @param _inc A boolean indicating whether the amount is an income (true) or an expense (false)
 */
void Item::setAmt(double _amount, bool _inc)
{
    if(_inc)
    {
        if(_amount < 0)
        {
            amount = _amount * -1;
        }

        else
        {
            amount = _amount;
        }
    }

    else
    {
        if(amount > 0)
        {
            amount = _amount * -1;
        }

        else
        {
            amount = _amount;
        }
    }
};

/**
 * @brief Returns a string representation of the enum value for occurs
 * @return string representation of occurs
 */
string Item::getOccurance() const
{
    string returnVal = "";
    switch(occurs)
    {
        case ONCE:
            returnVal = "ONCE";
            break;
    
        case DAILY:
            returnVal = "DAILY";
            break;

        case WEEKLY:
            returnVal = "WEEKLY";
            break;

        case BIWEEKLY:
            returnVal = "BIWEEKLY";
            break;

        case MONTHLY:
            returnVal = "MONTHLY";
            break;

        case QUARTERLY:
            returnVal = "QUARTERLY";
            break;

        case SEMIANNUALLY:
            returnVal = "SEMIANNUALLY";
            break;

        case ANNUALLY:
            returnVal = "ANNUALLY";
            break;
    }

    return returnVal;
};

//Operators
/**
 * @brief Overloading the << operator to print the item in a formatted way
 */
std::ostream& operator<< (std::ostream& os, const Item& _item)
{
    // |    Item Name      |      Pricing      |  Occurance  |     Date Happens     |
    string toPrint = "|\t" + _item.getName() + "\t|\t" + std::to_string(_item.getAmt()) + "\t|\t" + _item.getOccurance() 
                           + "\t|\t" + std::to_string(_item.getMonth()) + "/" + std::to_string(_item.getDay()) + "\t|\t" 
                           + std::to_string(_item.getYear()) + "\t|";
    os << std::fixed << std::setprecision(2) << toPrint << "\n";
    return os;
};

/**
 * @brief Overloading the < operator to compare two items based on their days
 */
bool operator<(const Item& lhs, const Item& rhs)
{
    bool returnVal = false;
    if(lhs.getYear() < rhs.getYear())
    {
        if(lhs.getDay() < rhs.getDay())
        {
            returnVal = true;
        }
    }

    return returnVal;
}

