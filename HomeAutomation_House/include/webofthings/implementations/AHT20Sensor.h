#ifndef AHT20_SENSOR_H
#define AHT20_SENSOR_H

#include "debugPrint.h"
#include <Adafruit_AHTX0.h>
#include "webofthings/WoTSensor.h"
#include "webofthings/Reading.h"

class AHT20Sensor : public WoTSensor {
private:
    Adafruit_AHTX0 aht;

    Reading* getTemperatureReading() {
        return readings[0];
    }

    Reading* getHumidityReading() {
        return readings[1];
    }

public:
    AHT20Sensor(unsigned long updatePeriod = 1000) : WoTSensor("AHT20", updatePeriod) {
        addReading(new Reading("Temperature", Reading::FLOAT, "thermometer-half", "C"));
        addReading(new Reading("Humidity", Reading::FLOAT, "tint", "%"));
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
    
    void sensorTick() override {
        sensors_event_t humidity, temp;
        aht.getEvent(&humidity, &temp);
        
        getTemperatureReading()->setValue(temp.temperature);
        getHumidityReading()->setValue(humidity.relative_humidity);

        notifyMQTTClient();
    }

    float getTemperatureValue() {
        return *((float*) getTemperatureReading()->getValue());
    }

    float getHumidityValue() {
        return *((float*) getHumidityReading()->getValue());
    }
};

#endif //AHT20_SENSOR_H