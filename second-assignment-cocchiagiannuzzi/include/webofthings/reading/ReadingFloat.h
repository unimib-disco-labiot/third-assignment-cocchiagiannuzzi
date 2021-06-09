#ifndef READING_FLOAT_H
#define READING_FLOAT_H

#include "ReadingTypeBase.h"

class ReadingFloat : public ReadingTypeBase {
private:
    float value;
public:
    ReadingFloat(float _value){
        value = _value;
    }
    ~ReadingFloat() {}

    void setValue(float _value){
        value = _value;
    }

    float getValue(){
        return value;
    }
    
    void addToJson(JsonObject& doc) override {
        doc[READINGTYPEBASE_VALUENAME] = value;
    }
};


#endif //READING_FLOAT_H