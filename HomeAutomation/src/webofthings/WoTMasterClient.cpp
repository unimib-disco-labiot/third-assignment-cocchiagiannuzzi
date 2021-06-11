#include "webofthings/WoTMasterClient.h"
#include "webofthings/Reading.h"

 WoTMasterClient::~WoTMasterClient() {
    for(WoTSensor* sensorPtr : wotSensors) {
        delete sensorPtr;
    }
    wotSensors.clear();
 }

void WoTMasterClient::addSensor(WoTSensor* sensor) {
    wotSensors.push_back(sensor);
}

void WoTMasterClient::parseNewClientJson(const String& jsonString) {
    StaticJsonDocument<512> doc;
    deserializeJson(doc, jsonString);

    JsonObject clientObj = doc.as<JsonObject>();
    macAddress = clientObj["mac_address"].as<String>();

    JsonArray sensorsArray = clientObj["sensors"].as<JsonArray>();
    for(JsonVariant v : sensorsArray) {
        JsonObject sensorObj = v.as<JsonObject>();
        String sensorName = sensorObj["name"];

        WoTSensor* sensor = new WoTSensor(sensorName);

        JsonArray readingsArray = sensorObj["readings"].as<JsonArray>();
        for(JsonVariant r : readingsArray) {
            JsonObject readingObj = r.as<JsonObject>();
            Reading::ReadingType type = readingObj["type"];
            Reading* reading = new Reading(readingObj["name"], type, readingObj["icon"], readingObj["unit"]);
            sensor->addReading(reading);
        }

        addSensor(sensor);
    }
}

WoTSensor* WoTMasterClient::findSensor(const String& sensorName) const {
    for(WoTSensor* sensor : wotSensors) {
        if(sensor->getName().equals(sensorName)) {
            return sensor;
        }
    }
    return nullptr;
}

const String& WoTMasterClient::getMacAddress() {
    return macAddress;
}