#include "webofthings/implementations/WoTLed.h"

#include <Arduino.h>
#include "debugPrint.h"

WoTLed::WoTLed(const String& _name, uint8_t _pin) : WoTActuator(_name, BOOL), pin(_pin) {}

WoTLed::~WoTLed() {}

void WoTLed::onStateChanged() {
    bool* state = (bool*) getState();
    digitalWrite(pin, *state ? HIGH : LOW);
    println(*((int*)getState()));
    println(String("LED STATE CHANGED ") + (*state ? "TRUE" : "FALSE"));

    WoTActuator::onStateChanged();
}

void WoTLed::init() {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
    setState(digitalRead(pin));
}