#ifndef PIR_SENSOR_H
#define PIR_SENSOR_H

#include "webofthings/WoTSensor.h"
#include "webofthings/reading/ReadingBool.h"


class PIRSensor : public WoTSensor {
public:
    PIRSensor(uint8_t _pin, unsigned long updatePeriod = 2000) : WoTSensor("PIR_sensor", updatePeriod) {
        addReading(new Reading("PIR Sensor", BOOL, "running", ""));
        pin = _pin;
    }

    ~PIRSensor(){}

    void init(){
        pinMode(pin, INPUT_PULLUP);
        startTicker();
    }

    void tick(){
        bool b;
        if(digitalRead(pin)==HIGH){
            b = true;
            getMovementReading()->setValue(new ReadingBool(b));
        }
        else{
            b = false;
            getMovementReading()->setValue(new ReadingBool(b));
        }
        notifyMQTTClient();
    }
    
private:
    uint8_t pin;

    Reading* getMovementReading() {
        return readings[0];
    }

    ReadingBool* getMovementValue() {
        return (ReadingBool*) getMovementReading()->getValue();
    }

    
};

#endif //PIR_SENSOR_H