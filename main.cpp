#include "ParsedInput.hpp"
#include "Enumerators.hpp"
#include "NetworkData.hpp"
#include "HelperFunctions.hpp"

#include <iostream>
#include <vector>
#include <array>
#include <iomanip>
#include <thread>
#include <chrono>
#include <stdlib.h>


#include <plplot/plplot.h>
#include <plplot/plstream.h>






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

void LogPeriodically(int interval) {

    while(true) {
        std::cout << "getting another set of data...\n";
        std::cout << "for now this must be stopped by hitting ctrl-C...\n";
        system("./get_ping.sh &");
        std::this_thread::sleep_for(std::chrono::minutes(interval));
    }
}

void Reader() {

    std::vector<NetworkData> data;
    std::string file_name = "networklog.txt";

    // if "networklog.txt" does not exist
    // GetData will return true and prompt
    // for an input file name until it
    // recieves something valid
    while(!GetData(data, file_name)){}





    int could_not_find_errors = 0;
    int parsing_errors = 0;
    GetErrors(could_not_find_errors,parsing_errors,data);


    char command[128];
    while(std::cin.getline(command,128)){
        ParsedInput input = ParseInput(command);



        if(input.error != "") {
            std::cout << "input error: " << input.error << "\n";
            continue;
        }

        if(input.command == "download" || input.command == "downloads"){
            std::vector<int> UniqueValuesDownload;
            std::vector<int> ModesDownload;
            GetModeDownload(UniqueValuesDownload, ModesDownload,data, input);
            std::array<float,5> FiveNumberSummaryDownload = GetFiveNumberSummaryDownload(data, input);


            PrintStats("Download", GetAverageDownload(data, input), ModesDownload, UniqueValuesDownload, FiveNumberSummaryDownload);
        }

        else if(input.command == "upload" || input.command == "uploads"){
            std::vector<int> _UniqueValuesUpload;
            std::vector<int> ModesUpload;
            GetModeUpload(_UniqueValuesUpload, ModesUpload,data, input);
            std::vector<float> UniqueValuesUpload;
            for(unsigned int i = 0;i < _UniqueValuesUpload.size(); i++) UniqueValuesUpload.push_back(_UniqueValuesUpload[i]/10.f);
            _UniqueValuesUpload.clear();
            std::array<float,5> FiveNumberSummaryUpload = GetFiveNumberSummaryUpload(data, input);


            PrintStats("Upload", GetAverageUpload(data, input), ModesUpload, UniqueValuesUpload, FiveNumberSummaryUpload);
        }

        else if(input.command == "ping" || input.command == "pings"){
            std::vector<int> UniqueValuesPing;
            std::vector<int> ModesPing;
            GetModePing(UniqueValuesPing, ModesPing,data, input);
            std::array<float,5> FiveNumberSummaryPing = GetFiveNumberSummaryPing(data, input);


            PrintStats("Ping", GetAveragePing(data, input), ModesPing, UniqueValuesPing, FiveNumberSummaryPing);
        }

        else if(input.command == "time"){
            std::cout << "log up for: " << data.size() << " hours = " << int(data.size()/24) << " days.\n";
            //! add total time(including down time) here
        }

        else if(input.command == "errors" || input.command == "warnings"){
            std::cout << "could not find data for " << could_not_find_errors+parsing_errors << " hours.\n";
        }

        else if(input.command == "quit" || input.command == "exit"){
            exit(0);
        }

        else{
            if(input.command != "help")
                std::cout << "invalid command, help printed below\n";
            std::cout << "commands are:\n";
            std::cout << "download,upload,ping,time,errors, and help\n";
            std::cout << "to quit simply enter quit or exit\n";
        }
    }
}


int main(int argc, char **argv)
{
    int interval = 60;
    bool reader_or_logger = true;
    for(int i = 1;i < argc; i++) {
        std::string arg = argv[i];
        if(arg == "-h" || arg == "--help") {
            std::cout << "--help     | " << "displays this help\n";
            std::cout << "--interval | " << "default is 60(1 hour). sets how often to log the data.\n";
            std::cout << "--logger   | " << "logs data, and has no user interaction except quit.\n";
            std::cout << "--reader   | " << "does not log data, but allows commands like \"ping\".\n";
            return 0;
        }
        else if(arg == "--interval" || arg == "-i") {
            interval = -1;
        }
        else if(interval == -1) {
            interval = std::stoi(arg);
        }
        else if(arg == "--logger" || arg == "-l") {
            reader_or_logger = false;
        }
        else if(arg == "--reader" || arg == "-r") {
            reader_or_logger = true;
        } else {
            std::cout << "unkown command \"" << arg << "\"\n";
            return 0;
        }
    }
    if(!reader_or_logger)
        LogPeriodically(interval);
    else
        Reader();


    return 0;
}
