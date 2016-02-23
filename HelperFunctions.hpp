#include <array>
#include <algorithm>

//!----------------------------------------------------------------------------
//-------------------averaging functions-------------------
//!----------------------------------------------------------------------------
float GetAverageDownload(const std::vector<NetworkData> &data){
    float AverageDownload = 0;
    int Count = 0;
    for(unsigned int i = 0;i < data.size();i++){
        if(data[i].Download != -1 && data[i].Download != -2){
            AverageDownload += data[i].Download;
            Count++;
        }
    }
    //! this will of course crash if any of these Counts are zero, but that should never be the case.
    return AverageDownload/Count;
}

float GetAverageUpload(const std::vector<NetworkData> &data){
    float AverageUpload = 0;
    int Count = 0;
    for(unsigned int i = 0;i < data.size();i++){
        if(data[i].Upload != -1 && data[i].Upload != -2){
            AverageUpload += data[i].Upload;
            Count++;
        }
    }
    //! this will of course crash if any of these Counts are zero, but that should never be the case.
    return AverageUpload/Count;
}


float GetAveragePing(const std::vector<NetworkData> &data){
    float AveragePing = 0;
    int Count = 0;
    for(unsigned int i = 0;i < data.size();i++){
        if(data[i].Ping != -1 && data[i].Ping != -2){
            AveragePing += data[i].Ping;
            Count++;
        }
    }
    //! this will of course crash if any of these Counts are zero, but that should never be the case.
    return AveragePing/Count;
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
//-------------------sort functions-------------------
//!----------------------------------------------------------------------------
template<class T>
void DoubleSort(std::vector<T> &SortFromAndTo, std::vector<T> &SortTo){
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

void GetModeDownload(std::vector<int> &UniqueValuesDownload, std::vector<int> &ModesDownload,const std::vector<NetworkData> &data){
    for(unsigned int i = 0;i < data.size();i++){
        if(data[i].Download != -1 && data[i].Download != -2){
            _HelperModes(UniqueValuesDownload, ModesDownload, i, int(data[i].Download+.5));
        }
    }
    // we get our data from _HelperModes in scrambled order so fix that!
    DoubleSort(UniqueValuesDownload,ModesDownload);

}

void GetModeUpload(std::vector<int> &UniqueValuesUpload, std::vector<int> &ModesUpload,const std::vector<NetworkData> &data){
    for(unsigned int i = 0;i < data.size();i++){
        if(data[i].Upload != -1 && data[i].Upload != -2){
            _HelperModes(UniqueValuesUpload, ModesUpload, i, int(data[i].Upload*10.f));
        }
    }
    // we get our data from _HelperModes in scrambled order so fix that!
    DoubleSort(UniqueValuesUpload,ModesUpload);

}


void GetModePing(std::vector<int> &UniqueValuesPing, std::vector<int> &ModesPing,const std::vector<NetworkData> &data){
    for(unsigned int i = 0;i < data.size();i++){
        if(data[i].Ping != -1 && data[i].Ping != -2){
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

std::array<float,5> GetFiveNumberSummaryDownload(const std::vector<NetworkData> &data){
    std::array<float,5> FiveNumber;
    std::vector<float> Downloads;
    for(unsigned int i = 0;i < data.size();i++){
        if(data[i].Download != -1 && data[i].Download != -2)
            Downloads.push_back(data[i].Download);
    }
    std::sort(Downloads.begin(),Downloads.end());

    //! numbers are too close to care about averaging median if even number
    FiveNumber[0] = Downloads[0];
    FiveNumber[1] = Downloads[Downloads.size()*.25-1];
    FiveNumber[2] = Downloads[Downloads.size()*.5 -1];
    FiveNumber[3] = Downloads[Downloads.size()*.75-1];
    FiveNumber[4] = Downloads.back();
    return FiveNumber;
}

std::array<float,5> GetFiveNumberSummaryUpload(const std::vector<NetworkData> &data){
    std::array<float,5> FiveNumber;
    std::vector<float> Uploads;
    for(unsigned int i = 0;i < data.size();i++){
        if(data[i].Upload != -1 && data[i].Upload != -2)
            Uploads.push_back(data[i].Upload);
    }
    std::sort(Uploads.begin(),Uploads.end());

    //! numbers are too close to care about averaging median if even number
    FiveNumber[0] = Uploads[0];
    FiveNumber[1] = Uploads[Uploads.size()*.25-1];
    FiveNumber[2] = Uploads[Uploads.size()*.5 -1];
    FiveNumber[3] = Uploads[Uploads.size()*.75-1];
    FiveNumber[4] = Uploads.back();
    return FiveNumber;
}

std::array<float,5> GetFiveNumberSummaryPing(const std::vector<NetworkData> &data){
    std::array<float,5> FiveNumber;
    std::vector<float> Pings;
    for(unsigned int i = 0;i < data.size();i++){
        if(data[i].Ping != -1 && data[i].Ping != -2)
            Pings.push_back(data[i].Ping);
    }
    std::sort(Pings.begin(),Pings.end());

    //! numbers are too close to care about averaging median if even number
    FiveNumber[0] = Pings[0];
    FiveNumber[1] = Pings[Pings.size()*.25-1];
    FiveNumber[2] = Pings[Pings.size()*.5 -1];
    FiveNumber[3] = Pings[Pings.size()*.75-1];
    FiveNumber[4] = Pings.back();
    return FiveNumber;
}
