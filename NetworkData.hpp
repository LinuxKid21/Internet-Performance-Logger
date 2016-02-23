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


bool GetData(std::vector<NetworkData> &data, std::string &file_name){
    NetworkData _data;
    std::ifstream myfile (file_name);
    std::string line;
    std::string last_line;
    if (myfile.is_open()){
        while ( getline (myfile,line) ){
            if(line.find("Download:") != std::string::npos){
                unsigned int i = 0;
                float num = FindNextNumber(line, i);
                _data.Download = num;
            }
            if(line.find("Ping:") != std::string::npos){
                unsigned int i = 0;
                float num = FindNextNumber(line, i);
                _data.Ping = num;
            }

            // Upload is always last so here push _data to data
            if(line.find("Upload:") != std::string::npos){
                unsigned int i = 0;
                float num = FindNextNumber(line, i);
                _data.Upload = num;
                data.push_back(_data);

                _data = NetworkData();
            }
            //! if the last line is a seperator and this line is not, then this one is the date
            if(line != "----------------" && last_line == "----------------"){
                _data.day = find_day(line);
                _data.month = find_month(line);

                unsigned int i = 0;
                _data.day_of_month = FindNextNumber(line, i);
                _data.hour = FindNextNumber(line, i);
                _data.minute = FindNextNumber(line, i);
                FindNextNumber(line, i); // skip seconds
                _data.year = FindNextNumber(line, i);
            }

            last_line = line;
        }
        myfile.close();
    }else{
        std::cout << "please input a file:\n";
        std::cin >> file_name;
        return false;
    }
    return true;
}
