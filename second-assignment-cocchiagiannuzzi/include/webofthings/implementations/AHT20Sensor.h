#ifndef AHT20_SENSOR_H
#define AHT20_SENSOR_H

#include "debugPrint.h"
#include <Adafruit_AHTX0.h>
#include "webofthings/WoTSensor.h"
#include "webofthings/reading/ReadingFloat.h"

class AHT20Sensor : public WoTSensor {
private:
    Adafruit_AHTX0 aht;

    Reading* getTemperatureReading() {
        return readings[0];
    }

    Reading* getHumidityReading() {
        return readings[1];
    }

    ReadingFloat* getTemperatureValue() {
        return (ReadingFloat*) getTemperatureReading()->getValue();
    }

    ReadingFloat* getHumidityValue() {
        return (ReadingFloat*) getHumidityReading()->getValue();
    }

public:
    AHT20Sensor(unsigned long updatePeriod = 1000) : WoTSensor("AHT20", updatePeriod) {
        addReading(new Reading("Temperature", FLOAT, "thermometer-half", "C"));
        addReading(new Reading("Humidity", FLOAT, "tint", "%"));
    }

    ~AHT20Sensor() {}

    void init() override {
        println("Checking for AHt20");
        if (!aht.begin()) {
            println("Could not find AHT20? Check wiring. Reset to continue.");
            while (1)
                delay(10);
        }
        println("AHT20 found");

        startTicker();
    }
    
    void tick() override {
        sensors_event_t humidity, temp;
        aht.getEvent(&humidity, &temp);
        
        getTemperatureReading()->setValue(new ReadingFloat(temp.temperature));
        getHumidityReading()->setValue(new ReadingFloat(humidity.relative_humidity));

        notifyMQTTClient();
    }


    float getTemperature() {
        return getTemperatureValue()->getValue();
    }

    float getHumidity() {
        return getHumidityValue()->getValue();
    }
};

#endif //AHT20_SENSOR_H