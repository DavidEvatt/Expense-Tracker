#ifndef ACTIONVALIDATOR_H
#define ACTIONVALIDATOR_H
#include <string>
    using std::string;

class ActionValidator
{
    private:
        string _OCCURANCES[8] = {"ONCE", "DAILY", "WEEKLY", "BIWEEKLY", "MONTHLY", "QUARTERLY", "SEMIANNUALLY", "ANNUALLY"};
        int _30DAYS[4] = {4, 6, 9, 11};

    public:
        bool checkOccurance(string _occurs)
        {
            bool returnVal = false;
            for(int i = 0; i < sizeof(_OCCURANCES); i++)
            {
                if(_OCCURANCES[i] == _occurs)
                {
                    returnVal = true;
                }
            }

            return returnVal;
        };

        bool checkMonths(int _month)
        {
            bool returnVal = false;
            if(_month > 0 && _month < 13)
            {
                returnVal = true;
            }

            return returnVal;
        };

        bool checkDays(int _day, int _month)
        {
            int maxDays = 31;
            bool correctDay = false;

            if(_month == 2)
            {
                maxDays = 29;
            }
            else
            {
                for(size_t i = 0; i < sizeof(_30DAYS); i++)
                {
                    if(_month == _30DAYS[i])
                    {
                        maxDays = 30;
                    }
                }
            }

            if(_day > 0 && _day <= maxDays)
            {
                correctDay = true;
            }

            return correctDay;
        };

        int getMaxDays(int _month)
        {
            int maxDays = 31;

            if(_month == 2)
            {
                maxDays = 29;
            }
            else
            {
                for(size_t i = 0; i < sizeof(_30DAYS); i++)
                {
                    if(_month == _30DAYS[i])
                    {
                        maxDays = 30;
                    }
                }
            }

            return maxDays;
        };
};

#endif