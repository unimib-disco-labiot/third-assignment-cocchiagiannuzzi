#ifndef ENTRANCE_HANDLER_H
#define ENTRANCE_HANDLER_H

#include <Arduino.h>
#include <Ticker.h>
#include "EnvironmentHandler.h"

#include "webofthings/implementations/WiFiSignalSensor.h"
#include "webofthings/implementations/AHT20Sensor.h"
#include "webofthings/implementations/WoTLed.h"
#include "webofthings/implementations/PIRSensor.h"

#define BUZZER_PIN D6
#define PIR_PIN D7
#define LIGHT_PIN1 D3
#define LIGHT_PIN2 D4

class EntranceHandler : public EnvironmentHandler {
private:
    WiFiSignalSensor* wifiSensor;
    AHT20Sensor* aht20;
    WoTLed* light1;
    WoTLed* light2;
    PIRSensor* pirSensor;

    Ticker ticker;

    void soundAlarm();

public: 
    EntranceHandler();
    ~EntranceHandler();

    void init();
};

#endif //ENTRANCE_HANDLER_H