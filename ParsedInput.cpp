#include "ParsedInput.hpp"


ParsedInput ParseInput(std::string str) {

    enum FINDING{
        FINDING_COMMAND = 0,
        FINDING_FROM,
        FINDING_S_MONTH,
        FINDING_S_DAY,
        FINDING_S_YEAR,
        FINDING_TO,
        FINDING_E_MONTH,
        FINDING_E_DAY,
        FINDING_E_YEAR,
        FINDING_FINISHED
    };
    std::vector<std::string> split;

    unsigned int j = 0;
    unsigned int i = 0;
    for(;i < str.size(); i++) {
        if(str[i] == ' ') {
            split.push_back(str.substr(j, i-j));
            j = i+1;
        }
    }
    split.push_back(str.substr(j, i-j));






    ParsedInput r = ParsedInput();

    FINDING stage = FINDING_COMMAND;


    for(unsigned int k = 0;k < split.size(); k++) {
        if(!split[k].empty()) {
            switch(stage) {
            case FINDING_COMMAND: {
                r.command = split[k];
                stage = FINDING_FROM;
                break;
            }
            case FINDING_FROM: {
                if(split[k] != "from") {
                    r.error = std::string("Expected to find from, but found: ") + split[k];
                    return r;
                }
                stage = FINDING_S_MONTH;
                break;
            }

            case FINDING_S_MONTH: {
                MONTHNAMES m = find_month(split[k]);
                if(m == UnkM) {
                    r.error = std::string("Expected to find one of {Jan, Feb Mar, Apr, May, Jun, Jul, Aug Sep, Oct, Nov, Dec} but found: ") + split[k];
                    return r;
                }
                r.starting_month = m;
                stage = FINDING_S_DAY;
                break;
            }

            case FINDING_S_DAY: {
                int m = std::stoi(split[k]);
                if(m < 1 || m > 31) {
                    r.error = std::string("Expected value in the range [1, 31] but found: ") + split[k];
                    return r;
                }
                r.starting_day_of_month = m;
                stage = FINDING_S_YEAR;
                break;
            }

            case FINDING_S_YEAR: {
                int m = std::stoi(split[k]);
                r.starting_year = m;
                stage = FINDING_TO;
                break;
            }


            case FINDING_TO: {
                if(split[k] != "to") {
                    r.error = std::string("Expected to find to, but found: ") + split[k];
                    return r;
                }
                stage = FINDING_E_MONTH;
                break;
            }



            case FINDING_E_MONTH: {
                MONTHNAMES m = find_month(split[k]);
                if(m == UnkM) {
                    r.error = std::string("Expected to find one of {Jan, Feb Mar, Apr, May, Jun, Jul, Aug Sep, Oct, Nov, Dec} but found: ") + split[k];
                    return r;
                }
                r.ending_month = m;
                stage = FINDING_E_DAY;
                break;
            }
            case FINDING_E_DAY: {
                int m = std::stoi(split[k]);
                if(m < 1 || m > 31) {
                    r.error = std::string("Expected value in the range [1, 31] but found: ") + split[k];
                    return r;
                }
                r.ending_day_of_month = m;
                stage = FINDING_E_YEAR;
                break;
            }
            case FINDING_E_YEAR: {
                int m = std::stoi(split[k]);
                r.ending_year = m;
                stage = FINDING_FINISHED;
                break;
            }

            default:
                r.error = std::string("Too many dates or other error: ") + split[k];
                return r;
                break;
            }
        }
    }

    if(stage != FINDING_FROM && stage != FINDING_FINISHED) {
        r.error = std::string("Expected more or less arguments. Please use the format (command) or the foramt (command) from Jan 1 2015 to Feb 28 2016");
    }

    return r;
}
