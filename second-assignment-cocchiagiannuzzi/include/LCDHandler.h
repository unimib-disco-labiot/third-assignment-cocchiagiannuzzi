#ifndef LCD_HANDLER_H
#define LCD_HANDLER_H

#include <LiquidCrystal_I2C.h>  

class LCDHandler{
private:
    LiquidCrystal_I2C* lcd;
    
public:
    LCDHandler(int address = 0x27, int characters = 16, int lines = 2){
        lcd = new LiquidCrystal_I2C(address, characters, lines); 
    }
    
    ~LCDHandler(){
        //delete lcd;
    }
};

#endif //LCD_HANDLER_H