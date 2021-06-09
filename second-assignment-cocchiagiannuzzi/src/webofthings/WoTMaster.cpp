#include "webofthings/WoTMaster.h"

#include "MQTTHandler.h"
#include "webofthings/topicNames.h"
#include "WiFiHandler.h"
#include <ArduinoJson.h>
#include "webofthings/topicNames.h"
#include "webofthings/Reading.h"
#include "webofthings/reading/ReadingBool.h"
#include "webofthings/reading/ReadingEvent.h"
#include "webofthings/reading/ReadingFloat.h"
#include "webofthings/reading/ReadingInt.h"
#include "webofthings/reading/ReadingString.h"
#include "InfluxDBHandler.h"

WoTMaster::WoTMaster() {
    std::map<String, int> topics;
    topics.insert(std::pair<String, int>(SENSOR_DATA_UPDATE, LWMQTT_QOS0));
    topics.insert(std::pair<String, int>(MASTER_INFO_TRIGGER, LWMQTT_QOS2));
    topics.insert(std::pair<String, int>(DEVICE_DISCONNECTED, LWMQTT_QOS2));
    topics.insert(std::pair<String, int>(MASTER_INFLUXDB, LWMQTT_QOS2));
    MQTTHandler::getInstance().attachObserver(this, topics);
}

WoTMaster::~WoTMaster(){
    for(WoTMasterClient* clientPtr : clients) {
        delete clientPtr;
    }
    clients.clear();
}

void WoTMaster::sendMasterInfo(){
    StaticJsonDocument<256> doc;
    doc["mac_address"] = WiFiHandler::getInstance().getMacAddress();

    char jsonMasterInfo[512];
    serializeJson(doc, jsonMasterInfo);

    print("JSON with master info: ");
    println(jsonMasterInfo);

    MQTTHandler::getInstance().publish(MASTER_INFO, jsonMasterInfo, 1);
}

WoTMasterClient* WoTMaster::findClient(const String& _macAddress) const {
    for(WoTMasterClient* client : clients) {
        if(client->getMacAddress().equals(_macAddress)) {
            return client;
        }
    }
    return nullptr;
}

void WoTMaster::onMQTTConnected() {
    MQTTHandler::getInstance().publish(TRIGGER_ALL_CLIENT_INFO, "");
}

void WoTMaster::onMessageReceived(String& topic, String& payload) {
    if(MQTTHandler::matchTopic(topic, MASTER_INFO_TRIGGER)) {
        sendMasterInfo();
    }
    else if(MQTTHandler::matchTopic(topic, String(SENSOR_DATA_UPDATE) + "/#")) {
        onSensorDataUpdate(payload);
        //TODO: Handle: MySQL
    }
    else if(MQTTHandler::matchTopic(topic, String(NEW_CLIENT_INFO))) {
        onNewClientConnected(payload);
    }
    else if(MQTTHandler::matchTopic(topic, DEVICE_DISCONNECTED)){
        StaticJsonDocument<48> doc;
        deserializeJson(doc, payload);
        const char* mac_address = doc["mac_address"];
        onClientDisconnected(mac_address);
    }
    else if(MQTTHandler::matchTopic(topic, MASTER_INFLUXDB)){
        StaticJsonDocument<48> doc;
        deserializeJson(doc, payload);
        bool influxActive = doc["active"].as<bool>();
        InfluxDBHandler::getInstance().setEnabled(influxActive);
    }

}
 
void WoTMaster::onClientDisconnected(const String& _macAddress) {
    WoTMasterClient* disconnectedClient = findClient(_macAddress);
    clients.erase(std::remove(clients.begin(), clients.end(), disconnectedClient), clients.end());
}

void WoTMaster::onSensorDataUpdate(const String& jsonString) {
    StaticJsonDocument<256> doc;
    deserializeJson(doc, jsonString);
    JsonObject rootObj = doc.as<JsonObject>();

    String device_mac_address = rootObj["mac_address"].as<String>();
    WoTMasterClient* client = findClient(device_mac_address);
    if(client != nullptr) {        
        WoTSensor* sensor = client->findSensor(rootObj["name"].as<String>());
        if(sensor != nullptr) {
            JsonArray readingsArray = rootObj["readings"].as<JsonArray>();

            for(JsonVariant r : readingsArray) {
                JsonObject readingObj = r.as<JsonObject>();
                Reading* reading = sensor->findReading(readingObj["name"]);
                if(reading != nullptr) {
                    ReadingType type = readingObj["type"];
                    switch(type) {
                        case INT:
                            reading->setValue(new ReadingInt(readingObj["value"].as<int>()));
                            break;
                        case FLOAT:
                            reading->setValue(new ReadingFloat(readingObj["value"].as<float>()));
                            break;
                        case BOOL:
                            reading->setValue(new ReadingBool(readingObj["value"].as<bool>()));
                            break;
                        case STRING:
                            reading->setValue(new ReadingString(readingObj["value"].as<String>()));
                            break;
                        case EVENT:
                            reading->setValue(new ReadingEvent());
                            break;
                        default:
                            break;
                    }
                    
                    InfluxDBHandler::getInstance().writeReadingsToInfluxDB(device_mac_address, *sensor, *reading);
                }
            }
        }
    }
}

void WoTMaster::onNewClientConnected(const String& jsonString) {
    WoTMasterClient* newClient = new WoTMasterClient();
    newClient->parseNewClientJson(jsonString);

    if(findClient(newClient->getMacAddress()) != nullptr) {
        delete newClient;
        return;
    }
    else {
        clients.push_back(newClient);
    }
}