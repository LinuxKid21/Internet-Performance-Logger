#ifndef NETWORK_DATA
#define NETWORK_DATA

#include "Enumerators.hpp"
#include "FindNumber.hpp"
#include <vector>
#include <fstream>



struct NetworkData{
    //! prob won't even parse this or use it...
    int year = -1;
    int hour = -1;
    int minute = -1;
    int day_of_month = -1;
    DAYNAMES day = UnkD;
    MONTHNAMES month = UnkM;
    float Download = -1;
    float Upload = -1;
    float Ping = -1;
};


bool GetData(std::vector<NetworkData> &data, std::string &file_name);

#endif // NETWORK_DATA
