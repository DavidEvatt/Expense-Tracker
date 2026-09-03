#ifndef ITEM_h
#define ITEM_h
#include <string>
    using std::string;

class Item
{
    private:
        int occurs;
        double amount;
        int dayOfWeek;
        int month;
    public:
        enum OCCURANCE
        {
            DAILY,
            WEEKLY,
            BIWEEKLY,
            MONTHLY,
            QUARTERLY,
            SEMIANNUALLY,
            ANNUALLY
        };

        Item(string _occurs, int _day, int _month, double _amount, bool _inc);

        //SETTERS

        //GETTERS
        string getOccurance()
        {
            switch(occurs)
            {
                case OCCURANCE.DAILY:
                    return "DAILY";
                    break;

                case OCCURANCE.WEEKLY:
                    return "WEEKLY";
                    break;

                case OCCURANCE.BIWEEKLY:
                    return "BIWEEKLY";
                    break;

                case OCCURANCE.MONTHLY:
                    return "MONTHLY";
                    break;

                case OCCURANCE.QUARTERLY:
                    return "QUARTERLY";
                    break;

                case OCCURANCE.SEMIANNUALLY:
                    return "SEMIANNUALLY";
                    break;

                case OCCURANCE.ANNUALLY:
                    return "ANNUALLY";
                    break;
            }
        };

        double getAmt() {return amount;};
        int getDay() {return dayOfWeek;}; 
        int getMonth() {return month;};
};


#endif