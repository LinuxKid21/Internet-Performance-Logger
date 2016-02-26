#include "HelperFunctions.hpp"

static bool IsDataValid(NetworkData d) {
    return d.Download != -1 && d.Upload != -1 && d.Ping != -1
           && d.Download != -2 && d.Upload != -2 && d.Ping != -2;
}

// this might be better replaced with a date-to-integer conversion
// and comparing integers
static bool DoesDataFitInParsedInput(NetworkData d, ParsedInput &input){
    if(d.year < input.ending_year && d.year > input.starting_year) {
        return true;
    }
    else if(d.year == input.starting_year && d.year == input.ending_year) {
        if(d.month > input.starting_month && d.month < input.ending_month) {
            return true;
        }else if(d.month == input.starting_month && d.month == input.ending_month) {
            return d.day_of_month >= input.starting_day_of_month && d.day_of_month <= input.ending_day_of_month;
        }
        else if(d.month == input.starting_month) {
            return d.day_of_month >= input.starting_day_of_month;
        }
        else if(d.month == input.ending_month) {
            return d.day_of_month <= input.ending_day_of_month;
        }
        return false;
    }


    else if(d.year == input.starting_year) {
        if(d.month > input.starting_month) {
            return true;
        }else if(d.month == input.starting_month) {
            return d.day_of_month >= input.starting_day_of_month;
        }
        return false;
    }


    else if(d.year == input.ending_year) {
        if(d.month < input.ending_month) {
            return true;
        }else if(d.month == input.ending_month) {
            return d.day_of_month <= input.starting_day_of_month;
        }
        return false;
    }
    return false;
}


template<typename Func, typename... Args>
static void IterateThroughData(const std::vector<NetworkData> &data, ParsedInput &input, Func func, Args&&... args) {
    for(unsigned int i = 0;i < data.size();i++){
        if(IsDataValid(data[i]) && DoesDataFitInParsedInput(data[i], input)){
            func(data[i], args...);
        }
    }
}



template<class T>
static void DoubleSort(std::vector<T> &SortFromAndTo, std::vector<T> &SortTo){
    //! sort modes here. cannot use standard function because we need to sort one, and sort the other one according to it.
    //! we need to loop twice; the inner loop sorts only one letter
    for(unsigned int j = 0;j < SortFromAndTo.size();j++){
        for(unsigned int i = 0;i < SortFromAndTo.size()-1;i++){
            if(SortFromAndTo[i] < SortFromAndTo[i+1]){
                std::swap(SortFromAndTo[i],SortFromAndTo[i+1]);
                std::swap(SortTo[i],SortTo[i+1]);
            }
        }
    }
}


//!----------------------------------------------------------------------------
//-------------------averaging functions-------------------
//!----------------------------------------------------------------------------


float GetAverageDownload(const std::vector<NetworkData> &data, ParsedInput &input){
    float AverageDownload = 0;
    int Count = 0;
    IterateThroughData(data, input, [&](NetworkData _data){AverageDownload += _data.Download; Count++;});
    if(Count == 0) return -1;
    return AverageDownload/Count;
}

float GetAverageUpload(const std::vector<NetworkData> &data, ParsedInput &input){
    float AverageUpload = 0;
    int Count = 0;
    IterateThroughData(data, input, [&](NetworkData _data){AverageUpload += _data.Upload; Count++;});
    if(Count == 0) return -1;
    return AverageUpload/Count;
}


float GetAveragePing(const std::vector<NetworkData> &data, ParsedInput &input){
    float AveragePing = 0;
    int Count = 0;
    IterateThroughData(data, input, [&](NetworkData _data){AveragePing += _data.Ping; Count++;});
    if(Count == 0) return -1;
    return AveragePing/Count;
}

#undef GetAverage


//!----------------------------------------------------------------------------
//-------------------get error-------------------
//!----------------------------------------------------------------------------
void GetErrors(int &could_not_find_errors, int &parsing_errors,const std::vector<NetworkData> &data){
    for(unsigned int i = 0;i < data.size();i++){
        if(data[i].Download == -1 || data[i].Upload == -1 || data[i].Ping == -1){
            could_not_find_errors++;
        }
        if(data[i].Download == -2 || data[i].Upload == -2 || data[i].Ping == -2){
            parsing_errors++;
        }
    }
}




//!----------------------------------------------------------------------------
//-------------------mode functions-------------------
//!----------------------------------------------------------------------------
/* _HelperModes takes in two empty vectors, an index, and a number. */
/* _HelperModes checks to see if num is inside UniqueValues, and if
   not adds it and initalizes Modes at that index with 1. If it already
   exists in UniqueValues, then increment Modes at that index. */
void _HelperModes(std::vector<int> &UniqueValues, std::vector<int> &Modes, int i, int num) {

    if(UniqueValues.empty()){
        UniqueValues.push_back(num);
        Modes.push_back(1);
    }else{
        bool new_mode = true;
        for(unsigned int i = 0;i < UniqueValues.size();i++){
            if(num == UniqueValues[i]){
                new_mode = false;
                Modes[i]++;
            }
        }
        if(new_mode){
            UniqueValues.push_back(num);
            Modes.push_back(1);
        }
    }
}


void GetModeDownload(std::vector<int> &UniqueValuesDownload, std::vector<int> &ModesDownload,const std::vector<NetworkData> &data, ParsedInput &input){
    for(unsigned int i = 0;i < data.size();i++){
        if(IsDataValid(data[i]) && DoesDataFitInParsedInput(data[i], input)){
            _HelperModes(UniqueValuesDownload, ModesDownload, i, int(data[i].Download+.5));
        }
    }
    // we get our data from _HelperModes in scrambled order so fix that!
    DoubleSort(UniqueValuesDownload,ModesDownload);

}

void GetModeUpload(std::vector<int> &UniqueValuesUpload, std::vector<int> &ModesUpload,const std::vector<NetworkData> &data, ParsedInput &input){
    for(unsigned int i = 0;i < data.size();i++){
        if(IsDataValid(data[i]) && DoesDataFitInParsedInput(data[i], input)){
            _HelperModes(UniqueValuesUpload, ModesUpload, i, int(data[i].Upload*10.f));
        }
    }
    // we get our data from _HelperModes in scrambled order so fix that!
    DoubleSort(UniqueValuesUpload,ModesUpload);

}


void GetModePing(std::vector<int> &UniqueValuesPing, std::vector<int> &ModesPing,const std::vector<NetworkData> &data, ParsedInput &input){
    for(unsigned int i = 0;i < data.size();i++){
        if(IsDataValid(data[i]) && DoesDataFitInParsedInput(data[i], input)){
            int download_int = 0;
            if(data[i].Ping <= 20)
                download_int = 20;
            else if(data[i].Ping <= 30)
                download_int = 30;
            else
                download_int = 40;

            _HelperModes(UniqueValuesPing, ModesPing, i, download_int);
        }
    }
    // we get our data from _HelperModes in scrambled order so fix that!
    DoubleSort(UniqueValuesPing,ModesPing);

}



//!----------------------------------------------------------------------------
//-------------------five number summary-------------------
//!----------------------------------------------------------------------------

static std::array<float,5> GetFiveNumberSummery(std::vector<float> &Speeds) {
    std::array<float,5> FiveNumber;
    std::sort(Speeds.begin(),Speeds.end());
    if(Speeds.size() >= 5) {
        //! numbers are too close to care about averaging median if even number
        FiveNumber[0] = Speeds[0];
        FiveNumber[1] = Speeds[Speeds.size()*.25-1];
        FiveNumber[2] = Speeds[Speeds.size()*.5 -1];
        FiveNumber[3] = Speeds[Speeds.size()*.75-1];
        FiveNumber[4] = Speeds.back();
    } else {
        for(int i = 0; i < 5; i++) {
            FiveNumber[i] = -1;
        }
    }
    return FiveNumber;
}

std::array<float,5> GetFiveNumberSummaryDownload(const std::vector<NetworkData> &data, ParsedInput &input){
    std::vector<float> Downloads;
    IterateThroughData(data, input, [&](NetworkData data){Downloads.push_back(data.Download);});
    return GetFiveNumberSummery(Downloads);
}

std::array<float,5> GetFiveNumberSummaryUpload(const std::vector<NetworkData> &data, ParsedInput &input){
    std::vector<float> Uploads;
    IterateThroughData(data, input, [&](NetworkData data){Uploads.push_back(data.Upload);});
    return GetFiveNumberSummery(Uploads);
}

std::array<float,5> GetFiveNumberSummaryPing(const std::vector<NetworkData> &data, ParsedInput &input){
    std::vector<float> Pings;
    IterateThroughData(data, input, [&](NetworkData data){Pings.push_back(data.Ping);});
    return GetFiveNumberSummery(Pings);
}
