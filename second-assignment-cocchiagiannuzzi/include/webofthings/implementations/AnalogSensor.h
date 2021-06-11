#ifndef ANALOG_SENSOR
#define ANALOG_SENSOR

#include "webofthings/WoTSensor.h"
#include "webofthings/Reading.h"
 

class AnalogSensor : public WoTSensor {
private:
    uint8_t analogPin;


    Reading* getAnalogReading() {
        return readings[0];
    }

public:
    AnalogSensor(uint8_t pin, const String& name, const String& read_name, 
                        const String& iconName, const String& unitOfMeasure = "%", 
                        unsigned long updatePeriod = 2000) : WoTSensor(name, updatePeriod) {
        analogPin = pin;
        addReading(new Reading(read_name, Reading::INT, iconName, unitOfMeasure));
    }

    void init(){
        startTicker();
    }

    void tick(){
        int value = analogRead(analogPin);
        int percentage = value * 100 / 1024; 
        getAnalogReading()->setValue(percentage);
        notifyMQTTClient();
    }

    int getAnalogValue() {
        return *((int*) getAnalogReading()->getValue());
    }

};

#endif //ANALOG_SENSOR