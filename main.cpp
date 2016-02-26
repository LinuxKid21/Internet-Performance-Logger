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






void PrintStats(std::string name, std::vector<NetworkData> &data, ParsedInput &input, float mode_mult, DATA d) {
    float average = GetAverage(data, input, d);
    Mode mode = GetMode(data, input, mode_mult, d);
    std::array<float,5> fiveNumberSummary = GetFiveNumberSummary(data, input, d);
    float total_total = 0;
    for(unsigned int j = 0;j < mode.Values.size();j++){
        total_total+= mode.Values[j];
    }
    std::cout << "Average " << name << " Speed: " << average << "\n\n";
    std::cout << "Five number summery: " << fiveNumberSummary[0] << ", " << fiveNumberSummary[1] << ", " << fiveNumberSummary[2] << ", "
              << fiveNumberSummary[3] << ", " << fiveNumberSummary[4] << "\n\n";

    std::cout   << std::setw(6)     << "speed"
                << std::setw(15)    << "occurrences"
                << std::setw(13)    << "special"
                << std::setw(13)    << "percent"
                << std::setw(22)    << "percent of total"
                << "\n";

    for(unsigned int i = 0;i < mode.UniqueValues.size();i++){
        int total = 0;
        for(unsigned int j = i;j < mode.Values.size();j++){
            total+= mode.Values[j];
        }
        std::cout   << std::setw(6)     << mode.UniqueValues[i]
                    << std::setw(15)    << mode.Values[i]
                    << std::setw(13)    << total
                    << std::setw(13)    << int(total/total_total*1000)/10.f
                    << std::setw(22)    << int(mode.Values[i]/total_total*1000)/10.f
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
            PrintStats("Download", data, input, 1, DATA_DOWNLOAD);
        }

        else if(input.command == "upload" || input.command == "uploads"){
            PrintStats("Upload", data, input, 10, DATA_UPLOAD);
        }

        else if(input.command == "ping" || input.command == "pings"){
            PrintStats("Ping", data, input, .1, DATA_PING);
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
