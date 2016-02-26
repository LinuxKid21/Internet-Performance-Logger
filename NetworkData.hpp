#ifndef NETWORK_DATA
#define NETWORK_DATA

#include "Enumerators.hpp"
#include "FindNumber.hpp"
#include <vector>
#include <fstream>

enum DATA {
    DATA_DOWNLOAD = 0,
    DATA_UPLOAD,
    DATA_PING,
};

struct NetworkData{
    //! prob won't even parse this or use it...
    int year = -1;
    int hour = -1;
    int minute = -1;
    int day_of_month = -1;
    DAYNAMES day = UnkD;
    MONTHNAMES month = UnkM;
    float Get(DATA d) {
        if(d == DATA_DOWNLOAD)
            return Download;
        if(d == DATA_UPLOAD)
            return Upload;
        if(d == DATA_PING)
            return Ping;
        return -1;
    }


    float Download = -1;
    float Upload = -1;
    float Ping = -1;
};

struct Mode {
    Mode(std::vector<float> _UniqueValues,std::vector<float> _Values) {
        UniqueValues = _UniqueValues;
        Values = _Values;
    }
    std::vector<float> UniqueValues;
    std::vector<float> Values;
};


bool GetData(std::vector<NetworkData> &data, std::string &file_name);

#endif // NETWORK_DATA
