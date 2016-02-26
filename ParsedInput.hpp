#ifndef PARSED_INPUT
#define PARSED_INPUT
#include <vector>
#include "Enumerators.hpp"

// this should get passed to any statistics functions so they can evaluate if the dates are correct
struct ParsedInput {
    std::string command;
    // initialize with default values
    // that will include all parsed data
    int starting_year = -5000;
    int ending_year = 5000;
    int starting_month = 0;
    int ending_month = 12;
    int starting_day_of_month = 0;
    int ending_day_of_month = 0;
    std::string error = ""; // empty string = no errors
};




// input should be in one of these formats:
// (command)
// (command) from Jan 1 2015 to Feb 28 2016     <- whole days  in calculation
ParsedInput ParseInput(std::string str);
#endif // PARSED_INPUT
