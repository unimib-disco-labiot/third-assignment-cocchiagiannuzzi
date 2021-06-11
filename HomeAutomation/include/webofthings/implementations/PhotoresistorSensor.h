#ifndef PHOTORESISTOR_SENSOR
#define PHOTORESISTOR_SENSOR

#include "webofthings/WoTSensor.h"
#include "webofthings/reading/ReadingInt.h"
 

class PhotoresistorSensor : public WoTSensor {
public:
    PhotoresistorSensor(uint8_t pin, unsigned long updatePeriod = 2000) : WoTSensor("photoresistor", updatePeriod) {
        photoresistor = pin;
        addReading(new Reading("Light", INT, "lightbulb", ""));
    }

    void init(){
        startTicker();
    }

    void tick(){
        int lightSensorValue = analogRead(photoresistor);
        getLightReading()->setValue(new ReadingInt(lightSensorValue));
        notifyMQTTClient();
    }
    
private:
    uint8_t photoresistor;

    Reading* getLightReading() {
        return readings[0];
    }

    ReadingInt* getLightValue() {
        return (ReadingInt*) getLightReading()->getValue();
    }

};

#endif //PHOTORESISTOR_SENSOR