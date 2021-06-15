#include "webofthings/implementations/ServoMotor.h"
#include "debugPrint.h"

ServoMotor::ServoMotor(const String& _name, int _pin, int _initialValue) : WoTActuator(_name, INT, new int(0)), pin(_pin), initialValue(_initialValue) {
    // motor = new Servo();
}

ServoMotor::~ServoMotor() {
    // delete motor;
}


void ServoMotor::init() {
    motor.attach(pin, 500, 2500);
    setState(initialValue);
}

void ServoMotor::onStateChanged() {
    int state = getTargetPos();

    if(state != getActualPos() && state >= 0 && state <= 180) {
        startMoving();
    }
}

bool ServoMotor::isMotorMoving() {
    return isMoving;
}

Servo* ServoMotor::getMotor() {
    return &motor;
}

int ServoMotor::getActualPos() {
    return actualState;
}

int ServoMotor::getTargetPos() {
    return *((int*) getState());
}

void ServoMotor::setActualPos(int _state){
    actualState = _state;
}

void ServoMotor::startMoving() {
    if(!isMotorMoving()) {
        ticker.attach_ms(20, stepMotor, this);
        isMoving = true;
    }
}

void ServoMotor::stopMovement() {
    ticker.detach();
    isMoving = false;
}

void ServoMotor::stepMotor(ServoMotor* motor) {   
    if(motor->getActualPos() == motor->getTargetPos()) {
        motor->stopMovement();
    }
    else {
        int direction = motor->getTargetPos() > motor->getActualPos() ? 1 : -1;
        motor->setActualPos(motor->getActualPos() + direction);
        motor->getMotor()->write(motor->getActualPos());
    }
}