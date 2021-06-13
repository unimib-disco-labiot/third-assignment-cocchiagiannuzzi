#ifndef WOTSENSOR_H
#define WOTSENSOR_H

#include <vector>
#include <set>
#include "ScheduledTicker.h"
#include "webofthings/Reading.h"
#include "WString.h"
#include "WoTHandler.h"
#include "events/WoTEvent.h"

class WoTClient;
class WoTEvent;

class WoTSensor : public ScheduledTicker::Tickable {
private:
    static std::set<WoTSensor*> wotSensors;

    std::vector<WoTEvent*> events;

protected:
    String name;
    std::vector<Reading*> readings;

    void notifyMQTTClient();

public:
    WoTSensor(const String& _name, unsigned long tickerPeriod = 0);
    ~WoTSensor();

    void addReading(Reading* reading);
    void addEvent(WoTEvent* event);
    
    virtual void init();
    
    void tick() override;
    virtual void sensorTick();


    const std::vector<Reading*>& getReadings() const;
    const String& getName() const;
    void buildJson(JsonObject* obj) const;
    void buildDiscoveryJsonObject(JsonObject* obj) const;
    Reading* findReading(const String& readingName) const;


    static void initAllSensors() {
        for(WoTSensor* sensorToInit : wotSensors) {
            sensorToInit->init();
        }
    }

    static const std::set<WoTSensor*>& getSensorList() {
        return wotSensors;
    }
};


#endif //WOTSENSOR_H