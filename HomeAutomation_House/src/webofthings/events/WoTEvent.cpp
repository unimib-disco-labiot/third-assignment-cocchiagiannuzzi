#include "webofthings/events/WoTEvent.h"

WoTEvent::WoTEvent(triggerFunc _trigger, outcomeFunc _outcome) {
    setTrigger(_trigger);
    setOutcome(_outcome);
}

WoTEvent::~WoTEvent() {}


bool WoTEvent::checkTrigger(WoTSensor* sensor) {
    return trigger(sensor);
}

void WoTEvent::execOutcome(WoTSensor* sensor) {
    outcome(sensor);
}

void WoTEvent::setTrigger(triggerFunc func) {
    trigger = func;
}

void WoTEvent::setOutcome(outcomeFunc func) {
    outcome = func;
}

void WoTEvent::handleEvent(WoTSensor* sensor) {
    if(checkTrigger(sensor)) {
        execOutcome(sensor);
    }
}