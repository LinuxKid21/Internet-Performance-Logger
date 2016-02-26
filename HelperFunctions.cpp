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
float GetAverage(const std::vector<NetworkData> &data, ParsedInput &input, DATA d) {
    float Average = 0;
    int Count = 0;
    IterateThroughData(data, input, [&](NetworkData _data){Average += _data.Get(d); Count++;});
    if(Count == 0) return -1;
    return Average/Count;
}


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
Mode GetMode(const std::vector<NetworkData> &data, ParsedInput &input, float multiplier, DATA d) {
    std::vector<float> UniqueValues;
    std::vector<float> Modes;
    IterateThroughData(data, input,
        [&](NetworkData _data){
            float num = int(_data.Get(d)*multiplier+.5)/multiplier;
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
       });
    DoubleSort(UniqueValues,Modes);
    return Mode(UniqueValues, Modes);
}



//!----------------------------------------------------------------------------
//-------------------five number summary-------------------
//!----------------------------------------------------------------------------

std::array<float,5> GetFiveNumberSummary(const std::vector<NetworkData> &data, ParsedInput &input, DATA d) {
    std::vector<float> Speeds;
    IterateThroughData(data, input, [&](NetworkData data){Speeds.push_back(data.Get(d));});
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
