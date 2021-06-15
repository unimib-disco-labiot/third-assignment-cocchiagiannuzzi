#ifndef READING_H
#define READING_H

#include<WString.h>
#include <ArduinoJson.h>
#include "webofthings/topicNames.h"


class Reading {
public: 
    enum ReadingType {
        INT,
        FLOAT,
        BOOL,
        STRING,
        EVENT
    };
    
private:
    ReadingType type;
    String name;
    String iconName;
    String unitOfMeasure;
    void* value = nullptr;
    
public:
    Reading(const String& _name, const ReadingType& _type, const String& _iconName, const String& _unitOfMeasure = "")
            : type(_type), name(_name), iconName(_iconName), unitOfMeasure(_unitOfMeasure) {
        // setValue(0);
    }

    ~Reading() {
        freeValue();
    }

    const String& getUnitOfMeasure() const {
        return unitOfMeasure;
    }

    const String& getName() const {
        return name;
    }

    const String& getIconName() const {
        return iconName;
    }

    void* getValue() const {
        return value;
    }

    ReadingType getType() const {
        return type;
    }

    void setValue(int _value) {
        setValue((void*)new int(_value));
    }

    void setValue(float _value) {
        setValue((void*)new float(_value));
    }

    void setValue(bool _value) {
        setValue((void*)new bool(_value));
    }

    void setValue(const String& _value) {
        setValue((void*)new String(_value));
    }

    void freeValue() {
        if(value != nullptr) {
            switch(type) {
                case INT:
                    delete ((int*)value);
                    break;
                case FLOAT:
                    delete ((float*)value);
                    break;
                case BOOL:
                    delete ((bool*)value);
                    break;
                case STRING:
                    delete ((String*)value);
                    break;
                case EVENT:
                    break;
            }
        }
    }

    void setValue(void* _value) {
        freeValue();
        value = _value;
    }

    void addToJsonObject(JsonObject& obj, bool addValue = true) {
        obj["name"] = getName();
        obj["type"] = getType();
        obj["icon"] = getIconName();
        obj["unit"] = getUnitOfMeasure();
        if(addValue && value != nullptr) {
            switch(type) {
                case INT:
                    obj[READINGTYPEBASE_VALUENAME] = *((int*)value);
                    break;
                case FLOAT:
                    obj[READINGTYPEBASE_VALUENAME] = *((float*)value);
                    break;
                case BOOL:
                    obj[READINGTYPEBASE_VALUENAME] = *((bool*)value);
                    break;
                case STRING:
                    obj[READINGTYPEBASE_VALUENAME] = *((String*)value);
                    break;
                case EVENT:
                    break;
            }
        }
    }

};

#endif //READING_H