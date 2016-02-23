#ifndef FIND_NUMBER
#define FIND_NUMBER
#include <iostream>

bool IsNum(char c){
    if(c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9'){
        return true;
    }else{
        return false;
    }
}

// searches through a string and returns the first number it finds.
// i is passed in by reference so this can be called again with the
// same i passed in to find the number after it
float FindNextNumber(std::string line, unsigned int &i) {
    bool found_first = false;
    std::string num_as_str = "";
    for(;i < line.size();i++){
        if(IsNum(line[i]) || line[i] == '.'){
            num_as_str.push_back(line[i]);
            found_first = true;
        }
        else if(found_first){
            break;
        }
    }
    if(!num_as_str.empty()){
        return std::stof(num_as_str);
    }
    return -2;
}

#endif // FIND_NUMBER
