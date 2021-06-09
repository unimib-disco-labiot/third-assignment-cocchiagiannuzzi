#ifndef READING_EVENT_H
#define READING_EVENT_H

#include "ReadingTypeBase.h"

class ReadingEvent : public ReadingTypeBase {
private:
    
public:
    ReadingEvent(){}
    ~ReadingEvent() {}
    
    void addToJson(JsonObject& doc) override {
        // doc[VALUE_NAME] = "event";
    }
};


#endif //READING_EVENT_H