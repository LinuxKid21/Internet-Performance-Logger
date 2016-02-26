#ifndef ENUMERATORS
#define ENUMERATORS
#include <iostream>


enum DAYNAMES{
    UnkD=0,Mon=1,Tue,Wed,
    Thu,Fri,Sat,Sun
};

enum MONTHNAMES{
    UnkM=0,Jan=1,Feb,Mar,Apr,May,
    Jun,Jul,Aug,Sep,Oct,Nov,Dec
};

DAYNAMES find_day(std::string date_from_line);
std::string day_to_string(DAYNAMES day);
MONTHNAMES find_month(std::string date_from_line);
std::string month_to_string(MONTHNAMES month);


#endif // ENUMERATORS
