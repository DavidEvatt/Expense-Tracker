#include "includes/Item.h"

Item::Item(string _occurs, int _day, int _month, double _amount, bool _inc)
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

    if(_occurs == "DAILY")
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
};