#ifndef WOTEVENT_H
#define WOTEVENT_H

#include "webofthings/WoTSensor.h"

class WoTSensor;

class WoTEvent {
public:
    typedef bool (*triggerFunc)(WoTSensor*);
    typedef void (*outcomeFunc)(WoTSensor*);

    WoTEvent(triggerFunc _trigger, outcomeFunc _outcome);
    ~WoTEvent();

    void handleEvent(WoTSensor* sensor);

private:
    triggerFunc trigger;
    outcomeFunc outcome;

    bool checkTrigger(WoTSensor* sensor);
    void execOutcome(WoTSensor* sensor);

    void setTrigger(triggerFunc func);
    void setOutcome(outcomeFunc func);
};

#endif //WOTEVENT_BASE_H