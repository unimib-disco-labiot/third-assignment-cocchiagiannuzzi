#include "webofthings/implementations/StepperMotorBoolState.h"

StepperMotorBoolState::StepperMotorBoolState(const String& _name, int drvPin1, int drvPin2, int drvPin3, int drvPin4, int _changeStateSteps, long speedRevsPerMin, int numberOfSteps, int gearRatio) : WoTActuator(_name, BOOL), changeStateSteps(_changeStateSteps) {
    motor = new Stepper(numberOfSteps, drvPin1, drvPin3, drvPin2, drvPin4);
    motor->setSpeed(speedRevsPerMin);
}

StepperMotorBoolState::~StepperMotorBoolState() {
    delete motor;
}

void StepperMotorBoolState::init() {}

void StepperMotorBoolState::onStateChanged() {
    bool state = *((bool*) getState());

    int multiplier = state ? 1 : -1;//TODO: This doesn't work
    motor->step(multiplier * changeStateSteps);
}