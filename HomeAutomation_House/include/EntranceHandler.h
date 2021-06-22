#ifndef ENTRANCE_HANDLER_H
#define ENTRANCE_HANDLER_H

#include <Arduino.h>
#include <Ticker.h>
#include <ESP8266TelegramBOT.h>
#include "EnvironmentHandler.h"
#include "secrets.h"

#include "webofthings/implementations/WiFiSignalSensor.h"
#include "webofthings/implementations/AHT20Sensor.h"
#include "webofthings/implementations/WoTLed.h"
#include "webofthings/implementations/PIRSensor.h"

#define BUZZER_PIN D6
#define PIR_PIN D7
#define LIGHT_PIN1 D3
#define LIGHT_PIN2 D4

#define BOT_MTBS 1000

class EntranceHandler : public EnvironmentHandler {
private:
    WiFiSignalSensor* wifiSensor;
    AHT20Sensor* aht20;
    WoTLed* light1;
    WoTLed* light2;
    PIRSensor* pirSensor;

    Ticker ticker;
    Ticker telegramTicker;
    TelegramBOT bot{BOT_TOKEN, BOT_NAME, BOT_USERNAME};

    void soundAlarm();
    void botExecMessages();

public: 
    EntranceHandler();
    ~EntranceHandler();

    void init();
};

#endif //ENTRANCE_HANDLER_H