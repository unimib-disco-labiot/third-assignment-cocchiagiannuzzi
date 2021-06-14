#ifndef SERVO_MOTOR_ACTUATOR_H
#define SERVO_MOTOR_ACTUATOR_H

#include "webofthings/WoTActuator.h"
#include <Servo.h>

class ServoMotor : public WoTActuator {
public:
    ServoMotor(const String& _name, int _pin);
    ~ServoMotor();

protected:
    void init();
    void onStateChanged();

private:
    Servo* motor;
    int pin;
};

#endif //SERVO_MOTOR_ACTUATOR_H