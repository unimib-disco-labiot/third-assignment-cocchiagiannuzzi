#ifndef DHT22_SENSOR_H
#define DHT22_SENSOR_H

#include "webofthings/WoTSensor.h"
#include "webofthings/Reading.h"

#include <DHT.h>
#include <Adafruit_Sensor.h>


class DHTSensor : public WoTSensor {
public:
    DHTSensor(uint8_t pin, uint8_t type, const String& name, unsigned long updatePeriod = 2000) : WoTSensor(name, updatePeriod) {
        dht = new DHT(pin, type);
        addReading(new Reading("Temperature", Reading::FLOAT, "thermometer-half", "C"));
        addReading(new Reading("Humidity", Reading::FLOAT, "tint", "%"));
        addReading(new Reading("Hic", Reading::FLOAT, "temperature-low", "C"));
    }

    ~DHTSensor(){
        delete dht;
    }

    void init(){
        dht->begin();
        startTicker();
    }

    void tick(){
        float temperature = dht->readTemperature();
        float humidity = dht->readHumidity();  
        

        if (!isnan(humidity) && !isnan(temperature)) {
            getTemperatureReading()->setValue(temperature);
            getHumidityReading()->setValue(humidity);

            float hic = dht->computeHeatIndex(temperature, humidity, false);
            getHicReading()->setValue(hic);
            notifyMQTTClient();
        }
    }
    
private:
    DHT* dht;

    Reading* getTemperatureReading() {
        return readings[0];
    }

    Reading* getHumidityReading() {
        return readings[1];
    }

    Reading* getHicReading() {
        return readings[2];
    }

public:
    float getTemperatureValue() {
        return *((float*) getTemperatureReading()->getValue());
    }

    float getHumidityValue() {
        return *((float*) getHumidityReading()->getValue());
    }

    float getHic() {
        return *((float*) getHicReading()->getValue());
    }
};

#endif //DHT22_SENSOR_H