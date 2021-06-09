#ifndef READING_TYPEBASE_H
#define READING_TYPEBASE_H

#include <ArduinoJson.h>
#include "WString.h"
#include "webofthings/topicNames.h"

class ReadingTypeBase {
public:
    virtual void addToJson(JsonObject& doc) = 0;
    virtual ~ReadingTypeBase() {}
};

#endif //READING_TYPEBASE_H