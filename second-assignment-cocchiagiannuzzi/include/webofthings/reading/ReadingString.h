#ifndef READING_STRING_H
#define READING_STRING_H

#include<WString.h>

#include "ReadingTypeBase.h"

class ReadingString : public ReadingTypeBase {
private:
    String value;
public:
    ReadingString(String _value){
        value = _value;
    }
    ~ReadingString() {}

    void setValue(String _value){
        value = _value;
    }

    String getValue(){
        return value;
    }

    void addToJson(JsonObject& doc) override {
        doc[READINGTYPEBASE_VALUENAME] = value;
    }
};

#endif //READING_INT_H