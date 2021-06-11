#ifndef WIFI_SIGNAL_SENSOR_H
#define WIFI_SIGNAL_SENSOR_H

#include "webofthings/WoTSensor.h"
#include "webofthings/Reading.h"

class WiFiSignalSensor : public WoTSensor {
private:
    Reading* getRSSIReading() {
        return readings[0];
    }

public:
    WiFiSignalSensor(unsigned long updatePeriod = 1000) : WoTSensor("WiFi_Sensor", updatePeriod) {
        addReading(new Reading("RSSI", Reading::INT, "wifi", "dBm"));
    }

    void init(){
        startTicker();
    }

    void sensorTick() override{
        if(WiFi.isConnected()) {
            int rssi = WiFi.RSSI();
            // print("WiFi RSSI: ");
            // println(rssi);
            getRSSIReading()->setValue(rssi);
        }

        notifyMQTTClient();
    }

    int getRSSIValue() {
        return *((int*) getRSSIReading()->getValue());
    }
};

#endif //WIFI_SIGNAL_SENSOR_H