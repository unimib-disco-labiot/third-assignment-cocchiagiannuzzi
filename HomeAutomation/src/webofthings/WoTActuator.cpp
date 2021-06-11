#include "webofthings/WoTActuator.h"
#include "MQTTHandler.h"
#include "webofthings/topicNames.h"
#include <ArduinoJson.h>
#include "WiFiHandler.h"
#include "webofthings/WoTHandler.h"


std::set<WoTActuator*> WoTActuator::wotActuators;

WoTActuator::WoTActuator(const String& _name, ActuatorStateType _stateType, void* _state, bool isMasterSide)
     : name(_name), stateType(_stateType), state(_state) {
    if(!isMasterSide) {
        wotActuators.insert(this);
        
        std::map<String, int> topics;
        topics.insert(std::pair<String, int>(SET_ACTUATOR_STATE, 1));
        MQTTHandler::getInstance().attachObserver(this, topics);
    }
}

void WoTActuator::freeState() {
    if(state != nullptr) {
        switch(stateType) {
            case INT:
                delete ((int*)state);
                break;
            case FLOAT:
                delete ((float*)state);
                break;
            case BOOL:
                delete ((bool*)state);
                break;
            case STRING:
                delete ((String*)state);
                break;
            default:
                break;
        }
    }
}

WoTActuator::~WoTActuator() {
    wotActuators.erase(this);
    freeState();
}

const String& WoTActuator::getName() const {
    return name;
}

WoTActuator::ActuatorStateType WoTActuator::getStateType() const {
    return stateType;
}

const void* WoTActuator::getState() const {
    return state;
}

void WoTActuator::setState(int _state) {
    freeState();
    int* s = new int(_state);
    state = (void*)s;
    onStateChanged();
}

void WoTActuator::setState(float _state) {
    freeState();
    float* s = new float(_state);
    state = (void*)s;
    onStateChanged();
}

void WoTActuator::setState(bool _state) {
    freeState();
    bool* s = new bool(_state);
    state = (void*)s;
    onStateChanged();
}

void WoTActuator::setState(const String& _state) {
    freeState();
    String* s = new String(_state);
    state = (void*)s;
    onStateChanged();
}

void WoTActuator::setState() {
    freeState();
    onStateChanged();
}

void WoTActuator::publishState() const {
    WoTClient* client = WoTHandler::getInstance().getClient();
    if(client != nullptr) {
        client->onActuatorStateChanged(this);
    }
}

void WoTActuator::onStateChanged() {
    publishState();
}

void WoTActuator::buildJson(JsonObject* obj) const {
    (*obj)["name"] = getName();
    (*obj)["type"] = getStateType();

    switch(stateType) {
        case INT:
            (*obj)["state"] = *((int*)state);
            break;
        case FLOAT:
            (*obj)["state"] = *((float*)state);
            break;
        case BOOL:
            (*obj)["state"] = *((bool*)state);
            break;
        case STRING:
            (*obj)["state"] = *((String*)state);
            break;
        default:
            break;
    }
}

void WoTActuator::parseSetStateJson(String& jsonString) {
    StaticJsonDocument<512> doc;
    deserializeJson(doc, jsonString);
    JsonObject rootObj = doc.as<JsonObject>();

    String macAddress = rootObj["mac_address"].as<String>();
    if(macAddress.equals(WiFiHandler::getInstance().getMacAddress())) {
        JsonObject actuatorObj = rootObj["actuator"].as<JsonObject>();
        if(actuatorObj["name"].as<String>().equals(getName())) {
            switch(getStateType()) {
                case INT:
                    setState(actuatorObj["state"].as<int>());
                    break;
                case FLOAT:
                    setState(actuatorObj["state"].as<float>());
                    break;
                case BOOL:
                    setState(actuatorObj["state"].as<bool>());
                    break;
                case STRING:
                    setState(actuatorObj["state"].as<String>());
                    break;
                case NONE:
                    setState();
                    break;
                default:
                    break;
            }
        }
    }
}

void WoTActuator::onMessageReceived(String& topic, String& payload) {
    if(MQTTHandler::matchTopic(topic, SET_ACTUATOR_STATE)) {
        parseSetStateJson(payload);
    }
}

void WoTActuator::onMQTTConnected() {
    publishState();
}


std::set<WoTActuator*>& WoTActuator::getAllActuators() {
    return wotActuators;
}

WoTActuator* WoTActuator::findActuator(const String& name) {
    for(WoTActuator* actuator : wotActuators) {
        if(actuator->getName().equals(name)) {
            return actuator;
        }
    }
    return nullptr;
}

void WoTActuator::buildDiscoveryJsonObject(JsonObject* obj) const {
    (*obj)["name"] = getName();
    (*obj)["type"] = getStateType();
    
    switch(stateType) {
        case INT:
            (*obj)["state"] = *((int*)state);
            break;
        case FLOAT:
            (*obj)["state"] = *((float*)state);
            break;
        case BOOL:
            (*obj)["state"] = *((bool*)state);
            break;
        case STRING:
            (*obj)["state"] = *((String*)state);
            break;
        default:
            break;
    }
}