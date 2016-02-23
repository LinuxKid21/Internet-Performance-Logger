#ifndef PARSED_INPUT
#define PARSED_INPUT
#include "FindNumber.hpp"

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
};


ParsedInput ParseInput(std::string str) {
    ParsedInput r = ParsedInput();
    unsigned int i = 0;
    r.starting_year = FindNextNumber(str, i);
    int command_index_last = i;
    r.starting_year = FindNextNumber(str, i);
    r.starting_month = FindNextNumber(str, i);
    r.starting_day_of_month = FindNextNumber(str, i);
    r.ending_year = FindNextNumber(str, i);
    r.ending_month = FindNextNumber(str, i);
    r.ending_day_of_month = FindNextNumber(str, i);
    r.command = str.substr(0,command_index_last);
    return r;
}
#endif // PARSED_INPUT
