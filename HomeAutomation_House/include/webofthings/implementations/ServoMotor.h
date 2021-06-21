#ifndef SERVO_MOTOR_ACTUATOR_H
#define SERVO_MOTOR_ACTUATOR_H

#include "webofthings/WoTActuator.h"
#include <Servo.h>
#include <Ticker.h>

class ServoMotor : public WoTActuator {
public:
    ServoMotor(const String& _name, int _pin, int initialValue);
    ~ServoMotor();

    void setActualPos(int state);
    int getActualPos();
    int getTargetPos();
    void stopMovement();
    bool isMotorMoving();
    Servo* getMotor();

protected:
    void init();
    void onStateChanged();

private:
    Ticker ticker;
    Servo motor;
    int pin;
    int initialValue;

    int actualState;
    bool isMoving = false;

    void startMoving();
    static void stepMotor(ServoMotor* motor);
};

#endif //SERVO_MOTOR_ACTUATOR_H