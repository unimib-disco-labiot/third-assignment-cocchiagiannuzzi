
#include <Arduino.h>
#include "debugPrint.h"

void debugPrintBegin(int baud_rate){    
  #ifdef DEBUG
    Serial.begin(baud_rate);
  #endif
}

void printf(const char* string){
  #ifdef DEBUG
    Serial.printf(string);
  #endif
}


void println(const char* string){
  #ifdef DEBUG
    Serial.println(string);
  #endif
}

void println(int integer){
  #ifdef DEBUG
    Serial.println(integer);
  #endif
}

void println(float floatingPoint){
  #ifdef DEBUG
    Serial.println(floatingPoint);
  #endif
}

void print(const char* string){
  #ifdef DEBUG
    Serial.print(string);
  #endif
}

void print(float floatingPoint){
  #ifdef DEBUG
    Serial.print(floatingPoint);
  #endif
}

void print(int integer){
  #ifdef DEBUG
    Serial.print(integer);
  #endif
}

void println(String str) {
  #ifdef DEBUG
    Serial.println(str);
  #endif
}

void print(String str) {
  #ifdef DEBUG
    Serial.print(str);
  #endif
}

void print(bool b) {
  #ifdef DEBUG
    Serial.print(b);
  #endif
}

void println(bool b) {
  #ifdef DEBUG
    Serial.println(b);
  #endif
}

void print(char c) {
  #ifdef DEBUG
    Serial.print(c);
  #endif
}

void println(char c) {
  #ifdef DEBUG
    Serial.println(c);
  #endif
}