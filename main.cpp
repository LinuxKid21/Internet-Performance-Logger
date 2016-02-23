#include <iostream>
#include <vector>
#include <fstream>
#include <array>
#include <algorithm>
#include <iomanip>

using namespace std;

#include "NetworkData.hpp"
#include "HelperFunctions.hpp"





// we need this for both ints and floats
template <class T>
void PrintStats(std::string name, float average, std::vector<int> &Modes, std::vector<T> &UniqueValues, std::array<float,5> &FiveNumberSummary) {
    float total_total = 0;
    for(unsigned int j = 0;j < Modes.size();j++){
        total_total+= Modes[j];
    }
    std::cout << "Average " << name << " Speed: " << average << "\n\n";
    std::cout << "Five number summery: " << FiveNumberSummary[0] << ", " << FiveNumberSummary[1] << ", " << FiveNumberSummary[2] << ", "
              << FiveNumberSummary[3] << ", " << FiveNumberSummary[4] << "\n\n";

    std::cout   << std::setw(6)     << "speed"
                << std::setw(15)    << "occurrences"
                << std::setw(13)    << "special"
                << std::setw(13)    << "percent"
                << std::setw(22)    << "percent of total"
                << "\n";

    for(unsigned int i = 0;i < UniqueValues.size();i++){
        int total = 0;
        for(unsigned int j = i;j < Modes.size();j++){
            total+= Modes[j];
        }
        std::cout   << std::setw(6)     << UniqueValues[i]
                    << std::setw(15)    << Modes[i]
                    << std::setw(13)    << total
                    << std::setw(13)    << int(total/total_total*1000)/10.f
                    << std::setw(22)    << int(Modes[i]/total_total*1000)/10.f
                    << "\n";
    }
}



int main()
{
    std::vector<NetworkData> data;

    std::string file_name = "networklog.txt";

    while(!GetData(data, file_name)){}






    float AverageDownload = GetAverageDownload(data);
    float AverageUpload = GetAverageUpload(data);
    float AveragePing = GetAveragePing(data);



    int could_not_find_errors = 0;
    int parsing_errors = 0;
    GetErrors(could_not_find_errors,parsing_errors,data);



    std::vector<int> UniqueValuesDownload;
    std::vector<int> ModesDownload;
    GetModeDownload(UniqueValuesDownload, ModesDownload,data);

    std::vector<int> _UniqueValuesUpload;
    std::vector<int> ModesUpload;
    GetModeUpload(_UniqueValuesUpload, ModesUpload,data);
    std::vector<float> UniqueValuesUpload;
    for(unsigned int i = 0;i < _UniqueValuesUpload.size(); i++) UniqueValuesUpload.push_back(_UniqueValuesUpload[i]/10.f);
    _UniqueValuesUpload.clear();

    std::vector<int> UniqueValuesPing;
    std::vector<int> ModesPing;
    GetModePing(UniqueValuesPing, ModesPing,data);




    std::array<float,5> FiveNumberSummaryDownload = GetFiveNumberSummaryDownload(data);
    std::array<float,5> FiveNumberSummaryUpload = GetFiveNumberSummaryUpload(data);
    std::array<float,5> FiveNumberSummaryPing = GetFiveNumberSummaryPing(data);


    char command[128];
    while(cin.getline(command,128)){
        string str = command;

        if(str == "download" || str == "downloads"){
            PrintStats("Download", AverageDownload, ModesDownload, UniqueValuesDownload, FiveNumberSummaryDownload);
        }

        else if(str == "upload" || str == "uploads"){
            PrintStats("Upload", AverageUpload, ModesUpload, UniqueValuesUpload, FiveNumberSummaryUpload);
        }

        else if(str == "ping" || str == "pings"){
            PrintStats("Ping", AveragePing, ModesPing, UniqueValuesPing, FiveNumberSummaryPing);
        }

        else if(str == "time"){
            std::cout << "log up for: " << data.size() << " hours = " << int(data.size()/24) << " days.\n";
            //! add total time(including down time) here
        }

        else if(str == "errors" || str == "warnings"){
            std::cout << "could not find data for " << could_not_find_errors+parsing_errors << " hours.\n";
        }

        else if(str == "quit" || str == "exit"){
            return 0;
        }

        else{
            if(str != "help")
                std::cout << "invalid command, help printed below\n";
            std::cout << "commands are:\n";
            std::cout << "download,upload,ping,time,errors, and help\n";
            std::cout << "to quit simply enter quit or exit\n";
        }
    }

    return 0;
}
