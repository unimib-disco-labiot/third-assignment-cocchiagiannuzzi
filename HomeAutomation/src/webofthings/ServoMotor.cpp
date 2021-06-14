#include "webofthings/implementations/ServoMotor.h"

#define SERVO_PWM_MIN 500
#define SERVO_PWM_MAX 2500

ServoMotor::ServoMotor(const String& _name, int _pin) : WoTActuator(_name, INT), pin(_pin) {
    motor = new Servo();
}

ServoMotor::~ServoMotor() {
    delete motor;
}


void ServoMotor::init() {
    motor->attach(pin, SERVO_PWM_MIN, SERVO_PWM_MAX);
    motor->write(0);
}

void ServoMotor::onStateChanged() {
    int state = *((int*) getState());

    if(state >= 0 && state <= 180) {
        motor->write(state);
    }
}