#ifndef WIFI_SIGNAL_SENSOR_H
#define WIFI_SIGNAL_SENSOR_H

#include "webofthings/WoTSensor.h"
#include "webofthings/reading/ReadingInt.h"

class WiFiSignalSensor : public WoTSensor {
public:
    WiFiSignalSensor(unsigned long updatePeriod = 1000) : WoTSensor("WiFi_Sensor", updatePeriod) {
        addReading(new Reading("RSSI", INT, "wifi", "dBm"));
    }

    void init(){
        startTicker();
    }

    void tick(){
        if(WiFi.isConnected()) {
            int32_t rssi = WiFi.RSSI();
            //print("WiFi RSSI: ");
            //println(rssi);
            getRSSIReading()->setValue(new ReadingInt(rssi));
        }

        notifyMQTTClient();
    }
    
private:
    Reading* getRSSIReading() {
        return readings[0];
    }

    ReadingInt* getRSSIValue() {
        return (ReadingInt*) getRSSIReading()->getValue();
    }
};

#endif //WIFI_SIGNAL_SENSOR_H