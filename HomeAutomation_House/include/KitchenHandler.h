#ifndef KITCHEN_HANDLER_H
#define KITCHEN_HANDLER_H

#include <Arduino.h>
#include <Ticker.h>
#include "EnvironmentHandler.h"

#include "webofthings/implementations/MQ4Sensor.h"
#include "webofthings/implementations/WoTLed.h"
#include "webofthings/implementations/ServoMotorOpener.h"


#define MQ4_PIN A0
#define LIGHT_PIN1 D3
#define LIGHT_PIN2 D4
#define WINDOW_SERVO_PIN D5

#define METHANE_THRESHOLD 1e4

class KitchenHandler : public EnvironmentHandler {
private:
    Ticker ticker;

    MQ4Sensor* mq4;
    WoTLed* light1;
    WoTLed* light2;
    ServoMotorOpener* servo;

    
    void soundAlarm();

public: 
    KitchenHandler();
    ~KitchenHandler();

    void init();
};

#endif //KITCHEN_HANDLER_H