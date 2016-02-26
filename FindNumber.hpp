#ifndef FIND_NUMBER
#define FIND_NUMBER
#include <iostream>


// searches through a string and returns the first number it finds.
// i is passed in by reference so this can be called again with the
// same i passed in to find the number after it
float FindNextNumber(std::string line, unsigned int &i);

#endif // FIND_NUMBER
