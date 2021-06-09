#ifndef ANALOG_SENSOR
#define ANALOG_SENSOR

#include "webofthings/WoTSensor.h"
#include "webofthings/reading/ReadingInt.h"
 

class AnalogSensor : public WoTSensor {
public:
    AnalogSensor(uint8_t pin, const String& name, const String& read_name, 
                        const String& iconName, const String& unitOfMeasure = "%", 
                        unsigned long updatePeriod = 2000) : WoTSensor(name, updatePeriod) {
        analogPin = pin;
        addReading(new Reading(read_name, INT, iconName, unitOfMeasure));
    }

    void init(){
        startTicker();
    }

    void tick(){
        int value = analogRead(analogPin);
        int percentage = value * 100 / 1024; 
        getAnalogReading()->setValue(new ReadingInt(percentage));
        notifyMQTTClient();
    }
    
private:
    uint8_t analogPin;


    Reading* getAnalogReading() {
        return readings[0];
    }

    ReadingInt* getAnalogValue() {
        return (ReadingInt*) getAnalogReading()->getValue();
    }

};

#endif //ANALOG_SENSOR