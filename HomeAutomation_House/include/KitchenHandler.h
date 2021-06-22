#ifndef KITCHEN_HANDLER_H
#define KITCHEN_HANDLER_H

#include <Arduino.h>
#include <Ticker.h>
#include <ESP8266TelegramBOT.h>
#include "EnvironmentHandler.h"
#include "secrets.h"

#include "webofthings/implementations/MQ4Sensor.h"
#include "webofthings/implementations/WoTLed.h"
#include "webofthings/implementations/ServoMotorOpener.h"


#define MQ4_PIN A0
#define LIGHT_PIN1 D3
#define LIGHT_PIN2 D4
#define WINDOW_SERVO_PIN D5

#define METHANE_THRESHOLD 300

#define BOT_MTBS 1000

class KitchenHandler : public EnvironmentHandler {
private:
    Ticker ticker;
    Ticker telegramTicker;
    TelegramBOT bot{BOT_TOKEN, BOT_NAME, BOT_USERNAME};

    MQ4Sensor* mq4;
    WoTLed* light1;
    WoTLed* light2;
    ServoMotorOpener* servo;

    void soundAlarm();
    void botExecMessages();

public: 
    KitchenHandler();
    ~KitchenHandler();

    void init();
};

#endif //KITCHEN_HANDLER_H