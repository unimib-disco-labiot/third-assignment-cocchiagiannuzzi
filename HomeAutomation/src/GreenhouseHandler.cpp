#include "GreenhouseHandler.h"
#include <functional>

GreenhouseHandler::GreenhouseHandler() {
    
    dht22Sensor = new DHTSensor(DHT_PIN, DHT22, "DHT_22");
    soilMoistureSensor = new AnalogSensor(SOIL_SENSOR_PIN, "soil_moisture", "Soil moisture", "leaf", "%");
    windowsMotor = new ServoMotorOpener("Windows Opener", WINDOWS_MOTOR_PIN, false, WINDOW_OPEN_VALUE, WINDOW_CLOSED_VALUE);
}

GreenhouseHandler::~GreenhouseHandler() {
    delete dht22Sensor;
    delete soilMoistureSensor;
    delete windowsMotor;
}

void GreenhouseHandler::init() {
    digitalWrite(PUMP_RELAY_PIN, HIGH);
    pinMode(PUMP_RELAY_PIN, OUTPUT);
    digitalWrite(PUMP_RELAY_PIN, LOW);

    // Events setup
    dht22Sensor->addEvent(new WoTEvent(
        [](WoTSensor* sensor) {
        DHTSensor* dht22 = (DHTSensor*) sensor;
            return dht22->hasValue() && dht22->getTemperatureValue() > 28.0f;
        },
        [this](WoTSensor* sensor) {
            // println(sensor->getName());
            // DHTSensor* dht22 = static_cast<DHTSensor*>(sensor);

            // Serial.println(String("TEMP: ") + dht22->getTemperatureValue());
            windowsMotor->setOpen(true);
        }
    ));
    dht22Sensor->addEvent(new WoTEvent(
        [](WoTSensor* sensor) {
            DHTSensor* dht22 = (DHTSensor*) sensor;
            return dht22->hasValue() && dht22->getTemperatureValue() < 24.0f;
        },
        [this](WoTSensor* sensor) {
            // DHTSensor* dht22 = (DHTSensor*) sensor;
            // Serial.println(String("TEMP2: ") + dht22->getTemperatureValue());
            windowsMotor->setOpen(false);
        }
    ));
}
