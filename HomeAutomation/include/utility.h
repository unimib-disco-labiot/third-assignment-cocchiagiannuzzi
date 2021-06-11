#ifndef UTILITY_H
#define UTILITY_H

#include <WString.h>

char* wStringToCharPtr(String string){
    size_t buf_size = (string.length() + 1) * sizeof(char);
    char* buffer = new char[buf_size];
    string.toCharArray(buffer, buf_size);
    return buffer;
}

#endif //UTILITY_H