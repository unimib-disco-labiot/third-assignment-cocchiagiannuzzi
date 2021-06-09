#ifndef READING_H
#define READING_H

#include <memory>
#include<WString.h>

#include "reading/ReadingTypeBase.h"

enum ReadingType {
    INT,
    FLOAT,
    BOOL,
    STRING,
    EVENT
};

class Reading {
private:
    ReadingType type;
    String name;
    String iconName;
    String unitOfMeasure;
    ReadingTypeBase* value;
    
public:
    Reading(const String& _name, const ReadingType& _type, const String& _iconName, const String& _unitOfMeasure = "")
            : type(_type), name(_name), iconName(_iconName), unitOfMeasure(_unitOfMeasure) {}

    const String& getUnitOfMeasure() const {
        return unitOfMeasure;
    }

    const String& getName() const {
        return name;
    }

    const String& getIconName() const {
        return iconName;
    }

    ReadingTypeBase* getValue() const {
        return value;
    }

    ReadingType getType() const{
        return type;
    }

    void setValue(ReadingTypeBase* _value) {
        if(value != nullptr) {
            delete value;
        }
        value = _value;
    }

    void addToJsonObject(JsonObject& obj, bool addValue = true) {
        obj["name"] = getName();
        obj["type"] = getType();
        obj["icon"] = getIconName();
        obj["unit"] = getUnitOfMeasure();
        if(addValue) {
            getValue()->addToJson(obj);
        }
    }

};

#endif //READING_H