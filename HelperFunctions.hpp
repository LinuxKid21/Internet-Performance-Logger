#ifndef HELPER_FUNCTIONS
#define HELPER_FUNCTIONS
#include "NetworkData.hpp"
#include "ParsedInput.hpp"
#include <array>
#include <algorithm>


//-------------------averaging functions-------------------
float GetAverageDownload(const std::vector<NetworkData> &data, ParsedInput &input);
float GetAverageUpload(const std::vector<NetworkData> &data, ParsedInput &input);
float GetAveragePing(const std::vector<NetworkData> &data, ParsedInput &input);

//-------------------get error-------------------
void GetErrors(int &could_not_find_errors, int &parsing_errors,const std::vector<NetworkData> &data);



//-------------------mode functions-------------------
void GetModeDownload(std::vector<int> &UniqueValuesDownload, std::vector<int> &ModesDownload,const std::vector<NetworkData> &data, ParsedInput &input);
void GetModeUpload(std::vector<int> &UniqueValuesUpload, std::vector<int> &ModesUpload,const std::vector<NetworkData> &data, ParsedInput &input);
void GetModePing(std::vector<int> &UniqueValuesPing, std::vector<int> &ModesPing,const std::vector<NetworkData> &data, ParsedInput &input);



//-------------------five number summary-------------------
std::array<float,5> GetFiveNumberSummaryDownload(const std::vector<NetworkData> &data, ParsedInput &input);
std::array<float,5> GetFiveNumberSummaryUpload(const std::vector<NetworkData> &data, ParsedInput &input);
std::array<float,5> GetFiveNumberSummaryPing(const std::vector<NetworkData> &data, ParsedInput &input);


#endif // HELPER_FUNCTIONS
