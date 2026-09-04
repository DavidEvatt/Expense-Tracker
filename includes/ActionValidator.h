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
        /**
         * @brief Checks if the given occurrence string is valid.
         */
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

        /**
         * @brief Checks if the given month is valid.
         * @param _month The month to check.
         * @return true if the month is valid, false otherwise.
         */
        bool checkMonths(int _month)
        {
            bool returnVal = false;
            if(_month > 0 && _month < 13)
            {
                returnVal = true;
            }

            return returnVal;
        };

        /**
         * @brief Checks if the given day is valid for the specified month.
         * @param _day The day to check.
         * @param _month The month to check against.
         * @return true if the day is valid, false otherwise.
         */
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

        /**
         * @brief Gets the maximum number of days in the specified month.
         * @param _month The month for which to get the maximum days.
         * @return The maximum number of days in the specified month.
         */
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