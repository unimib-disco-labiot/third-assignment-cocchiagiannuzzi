#ifndef WOT_ACTUATOR_LED
#define WOT_ACTUATOR_LED

#include "webofthings/WoTActuator.h"

class WoTLed : public WoTActuator {
private:
    uint8_t pin;

protected:
    void init();
    void onStateChanged();
    
public:
    WoTLed(const String& _name, uint8_t _pin);
    ~WoTLed();

};

#endif //WOT_ACTUATOR_LED