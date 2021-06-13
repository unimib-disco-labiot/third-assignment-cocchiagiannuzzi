#ifndef STEPPER_MOTOR_ACTUATOR_H
#define STEPPER_MOTOR_ACTUATOR_H

#include "webofthings/WoTActuator.h"
#include <Stepper.h>

class StepperMotorBoolState : public WoTActuator {
public:
    StepperMotorBoolState(const String& _name, int drvPin1, int drvPin2, int drvPin3, int drvPin4, int _changeStateSteps, long speedRevsPerMin = 320, int numberOfSteps = 64, int gearRatio = 32);
    ~StepperMotorBoolState();

protected:
    void init();
    void onStateChanged();

private:
    Stepper* motor;
    int changeStateSteps;
    
};

#endif //STEPPER_MOTOR_ACTUATOR_H