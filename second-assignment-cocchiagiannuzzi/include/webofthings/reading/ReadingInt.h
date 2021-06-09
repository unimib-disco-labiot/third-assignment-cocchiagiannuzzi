#ifndef READING_INT_H
#define READING_INT_H

#include "ReadingTypeBase.h"


class ReadingInt : public ReadingTypeBase {
private:
    int value;
public:
    ReadingInt(int _value){
        value = _value;
    }
    // ~ReadingInt() {}

    void setValue(int _value){
        value = _value;
    }

    int getValue(){
        return value;
    }

    void addToJson(JsonObject& doc) override {
        doc[READINGTYPEBASE_VALUENAME] = value;
    }
};


#endif //READING_INT_H