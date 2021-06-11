#include "webofthings/WoTSensor.h"
#include "webofthings/WoTHandler.h"

std::set<WoTSensor*> WoTSensor::wotSensors;


WoTSensor::WoTSensor(const String& _name, unsigned long tickerPeriod) : Tickable(tickerPeriod), name(_name){
    if(tickerPeriod > 0) {
        wotSensors.insert(this);
    }
}

WoTSensor::~WoTSensor() {
    wotSensors.erase(this);
    
    for(Reading* reading : readings) {
        delete reading;
    }
    readings.clear();
}

void WoTSensor::addReading(Reading* reading) {
    readings.push_back(reading);
}

void WoTSensor::notifyMQTTClient(){
    WoTClient* client = WoTHandler::getInstance().getClient();
    if(client != nullptr) {
        client->onSensorReadingUpdate(this);
    }
}

const std::vector<Reading*>& WoTSensor::getReadings() const {
    return readings;
}

const String& WoTSensor::getName() const {
    return name;
}

void WoTSensor::buildJson(JsonObject* obj) const {
    (*obj)["name"] = getName();
    JsonArray readingsArray = obj->createNestedArray("readings");
    
    for(Reading* reading : readings) {
        if(reading != nullptr && reading->getValue() != nullptr) {
            JsonObject nested = readingsArray.createNestedObject();
            reading->addToJsonObject(nested);
        }
    }
}

void WoTSensor::buildDiscoveryJsonObject(JsonObject* obj) const {
    (*obj)["name"] = getName();
    JsonArray readingsArray = obj->createNestedArray("readings");

    for(Reading* reading : getReadings()) {
        JsonObject nested = readingsArray.createNestedObject();
        reading->addToJsonObject(nested, false);
    }
}

Reading* WoTSensor::findReading(const String& readingName) const {
    for(Reading* reading : getReadings()) {
        if(reading->getName().equals(readingName)) {
            return reading;
        }
    }
    return nullptr;
}

void WoTSensor::sensorTick() {}

void WoTSensor::tick() {
    sensorTick();
    //TODO: Handle Events
}

void WoTSensor::init() {}
