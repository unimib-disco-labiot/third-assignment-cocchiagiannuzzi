#ifndef READING_BOOL_H
#define READING_BOOL_H

#include "ReadingTypeBase.h"

class ReadingBool : public ReadingTypeBase {
private:
    bool value;
public:
    ReadingBool(bool _value){
        value = _value;
    }
    ~ReadingBool() {}

    void setValue(bool _value){
        value = _value;
    }

    bool getValue(){
        return value;
    }

    void addToJson(JsonObject& doc) override {
        doc[READINGTYPEBASE_VALUENAME] = value;
    }
};

#endif //READING_BOOL_H