#ifndef GREENHOUSE_HANDLER_H
#define GREENHOUSE_HANDLER

#include <Arduino.h>
#include "EnvironmentHandler.h"

#include "webofthings/implementations/DHTSensor.h"
#include "webofthings/implementations/AnalogSensor.h"
#include "webofthings/implementations/ServoMotorOpener.h"

//Parameters
#define SOIL_SENSOR_PIN A0
#define PUMP_RELAY_PIN D5
#define DHT_PIN D6
#define WINDOWS_MOTOR_PIN D7

#define WINDOW_OPEN_VALUE 140
#define WINDOW_CLOSED_VALUE 40


class GreenhouseHandler : public EnvironmentHandler {
private:
    DHTSensor* dht22Sensor;
    AnalogSensor* soilMoistureSensor;
    ServoMotorOpener* windowsMotor;

public: 
    GreenhouseHandler();
    ~GreenhouseHandler();

    void init();
};


#endif //GREENHOUSE_HANDLER_H