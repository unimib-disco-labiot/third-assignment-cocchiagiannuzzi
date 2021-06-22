#include "webofthings/implementations/ServoMotorOpener.h"
#include "debugPrint.h"

ServoMotorOpener::ServoMotorOpener(const String& _name, int _pin, bool _initialValue, int _openState, int _closedState)
 : WoTActuator(_name, BOOL, new bool(_initialValue)), pin(_pin), initialValue(_initialValue), openState(_openState), closedState(_closedState) {
    
}

ServoMotorOpener::~ServoMotorOpener() {}


void ServoMotorOpener::attachMotor() {
    motor.attach(pin, 500, 2500);
}

void ServoMotorOpener::detachMotor() {
    // digitalWrite(pin, LOW);
    // motor.detach();
}

void ServoMotorOpener::setOpen(bool isOpen) {
    setState(isOpen ? openState : closedState);
}

void ServoMotorOpener::init() {
    attachMotor();
    setOpen(initialValue);
}

void ServoMotorOpener::onStateChanged() {
    motor.write(getTargetPos());
    // int state = getTargetPos();

    // if(state != getActualPos() && state >= 0 && state <= 180) {
    //     startMoving();
    // }
    WoTActuator::onStateChanged();
}

bool ServoMotorOpener::isMotorMoving() {
    return isMoving;
}

Servo* ServoMotorOpener::getMotor() {
    return &motor;
}

int ServoMotorOpener::getActualPos() {
    return actualState;
}

int ServoMotorOpener::getTargetPos() {
    return *((bool*) getState()) ? openState : closedState;
}

void ServoMotorOpener::setActualPos(int _state){
    actualState = _state;
}

void ServoMotorOpener::startMoving() {
    if(!isMotorMoving()) {
        ticker.attach_ms(20, stepMotor, this);
        isMoving = true;
    }
}

void ServoMotorOpener::stopMovement() {
    ticker.detach();
    isMoving = false;
}

void ServoMotorOpener::stepMotor(ServoMotorOpener* motor) {   
    if(motor->getActualPos() == motor->getTargetPos()) {
        motor->stopMovement();
        motor->detachMotor();
    }
    else {
        // if(!motor->getMotor()->attached()) {
        //     motor->attachMotor();
        // }
        int direction = motor->getTargetPos() > motor->getActualPos() ? 1 : -1;
        motor->setActualPos(motor->getActualPos() + direction);
        motor->getMotor()->write(motor->getActualPos());
    }
}