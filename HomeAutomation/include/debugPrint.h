#ifndef DEBUG_PRINT_H
#define DEBUG_PRINT_H

#define DEBUG

void debugPrintBegin(int baud_rate);

void printf(const char* string);


void println(const char* string);

void println(int integer);

void println(float floatingPoint);

void print(const char* string);

void print(float floatingPoint);

void print(int integer);

void println(String str);

void print(String str);

void print(bool b);

void println(bool b);

void print(char c);

void println(char c);

#endif //DEBUG_PRINT_H