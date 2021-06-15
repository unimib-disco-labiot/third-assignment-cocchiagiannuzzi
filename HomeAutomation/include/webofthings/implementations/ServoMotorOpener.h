#ifndef SERVO_MOTOR_OPENER_ACTUATOR_H
#define SERVO_MOTOR_OPENER_ACTUATOR_H

#include "webofthings/WoTActuator.h"
#include <Servo.h>
#include <Ticker.h>

class ServoMotorOpener : public WoTActuator {
public:
    ServoMotorOpener(const String& _name, int _pin, bool _initialValue, int _openState, int _closedState);
    ~ServoMotorOpener();

    void setActualPos(int state);
    int getActualPos();
    int getTargetPos();
    void stopMovement();
    bool isMotorMoving();
    Servo* getMotor();

    void setOpen(bool isOpen);

protected:
    void init();
    void onStateChanged();

    void attachMotor();
    void detachMotor();

private:
    Ticker ticker;
    Servo motor;
    int pin;
    int initialValue;

    int actualState;
    bool isMoving = false;

    int openState;
    int closedState;

    void startMoving();
    static void stepMotor(ServoMotorOpener* motor);
};

#endif //SERVO_MOTOR_OPENER_ACTUATOR_H