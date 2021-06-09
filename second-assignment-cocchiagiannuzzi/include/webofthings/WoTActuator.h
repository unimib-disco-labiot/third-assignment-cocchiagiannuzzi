#ifndef WOT_ACTUATOR_H
#define WOT_ACTUATOR_H

#include <set>
#include "WString.h"
#include "Observer.h"
#include "ArduinoJson.h"


class WoTActuator : IObserver {
public:
    enum ActuatorStateType {
        NONE,
        INT,
        FLOAT,
        BOOL,
        STRING
    };
private:
    static std::set<WoTActuator*> wotActuators;
    String name;
    ActuatorStateType stateType;
    void* state;
    
    void freeState();

protected:
    void publishState() const;
    
    virtual void onStateChanged() = 0;
    virtual void init() = 0;
    
public:
    WoTActuator(const String& _name, ActuatorStateType _stateType = NONE, void* _state = 0, bool isMasterSide = false);
    ~WoTActuator();

    void onMessageReceived(String& topic, String& payload);
    void onMQTTConnected();

    void setState(int _state);
    void setState(float _state);
    void setState(bool _state);
    void setState(const String& _state);
    void setState();

    const String& getName() const;
    ActuatorStateType getStateType() const;
    const void* getState() const;

    void buildJson(JsonObject* obj) const;
    void buildDiscoveryJsonObject(JsonObject* obj) const;
    void parseSetStateJson(String& jsonString);

    static std::set<WoTActuator*>& getAllActuators();
    static WoTActuator* findActuator(const String& name);

    
    static void initAllActuators() {
        for(WoTActuator* actuatorToInit : wotActuators) {
            actuatorToInit->init();
        }
    }
};

#endif //WOT_ACTUATOR_H