#ifndef DHT22_SENSOR_H
#define DHT22_SENSOR_H

#include "webofthings/WoTSensor.h"
#include "webofthings/reading/ReadingFloat.h"

#include <DHT.h>
#include <Adafruit_Sensor.h>


class DHTSensor : public WoTSensor {
public:
    DHTSensor(uint8_t pin, uint8_t type, const String& name, unsigned long updatePeriod = 2000) : WoTSensor(name, updatePeriod) {
        dht = new DHT(pin, type);
        addReading(new Reading("Temperature", FLOAT, "thermometer-half", "C"));
        addReading(new Reading("Humidity", FLOAT, "tint", "%"));
        addReading(new Reading("Hic", FLOAT, "temperature-low", "C"));
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
            getTemperatureReading()->setValue(new ReadingFloat(temperature));
            getHumidityReading()->setValue(new ReadingFloat(humidity));

            float hic = dht->computeHeatIndex(temperature, humidity, false);
            getHicReading()->setValue(new ReadingFloat(hic));
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

    ReadingFloat* getTemperatureValue() {
        return (ReadingFloat*) getTemperatureReading()->getValue();
    }

    ReadingFloat* getHumidityValue() {
        return (ReadingFloat*) getHumidityReading()->getValue();
    }

    ReadingFloat* getHic() {
        return (ReadingFloat*) getHicReading()->getValue();
    }
};

#endif //DHT22_SENSOR_H