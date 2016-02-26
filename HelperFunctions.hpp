#ifndef HELPER_FUNCTIONS
#define HELPER_FUNCTIONS
#include "NetworkData.hpp"
#include "ParsedInput.hpp"
#include <array>
#include <algorithm>


//-------------------averaging functions-------------------
float GetAverage(const std::vector<NetworkData> &data, ParsedInput &input, DATA d);

//-------------------get error-------------------
void GetErrors(int &could_not_find_errors, int &parsing_errors,const std::vector<NetworkData> &data);



//-------------------mode functions-------------------
Mode GetMode(const std::vector<NetworkData> &data, ParsedInput &input, float multiplier, DATA d);


//-------------------five number summary-------------------
std::array<float,5> GetFiveNumberSummary(const std::vector<NetworkData> &data, ParsedInput &input, DATA d);


#endif // HELPER_FUNCTIONS
