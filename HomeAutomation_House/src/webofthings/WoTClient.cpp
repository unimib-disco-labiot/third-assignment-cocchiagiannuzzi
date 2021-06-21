#include "webofthings/WoTClient.h"
#include "webofthings/topicNames.h"
#include "WiFiHandler.h"
#include "MQTTHandler.h"
#include <map>

WoTClient::WoTClient() {
    std::map<String, int> topics;
    topics.insert(std::pair<String, int>(TRIGGER_ALL_CLIENT_INFO, LWMQTT_QOS2));
    topics.insert(std::pair<String, int>(DEVICE_DISCONNECTED, LWMQTT_QOS2));
    //TODO: Subscribe actuator
    MQTTHandler::getInstance().attachObserver(this, topics);

    if(MQTTHandler::getInstance().getClient().connected()) {
        publishClientInfo();
    }
}

WoTClient::~WoTClient() {}

String WoTClient::buildTopic(const String& _topic, const String& _name) const {
    String topic;
    topic += _topic;
    topic += '/';
    topic += WiFiHandler::getInstance().getMacAddress();
    topic += '/';
    topic += _name;

    return topic;
}

void WoTClient::onSensorReadingUpdate(WoTSensor* sensor) {
    StaticJsonDocument<768> doc;
    JsonObject obj = doc.to<JsonObject>();
    String topic = buildTopic(SENSOR_DATA_UPDATE, sensor->getName());
    obj["mac_address"] = WiFiHandler::getInstance().getMacAddress();
    
    sensor->buildJson(&obj);
    String payload;
    serializeJson(doc, payload);

    println(payload);

    bool noError = MQTTHandler::getInstance().publish(topic, payload);
    
    if(noError && WoTHandler::getInstance().isMaster()) {
        String fullTopic = MQTTHandler::attachTopicPrefix(topic);
        WoTHandler::getInstance().getMaster()->onMessageReceived(fullTopic, payload);
    }
}

void WoTClient::onActuatorStateChanged(const WoTActuator* actuator) {
    StaticJsonDocument<256> doc;

    JsonObject obj = doc.to<JsonObject>();
    obj["mac_address"] = WiFiHandler::getInstance().getMacAddress();

    JsonObject actuatorObj = obj.createNestedObject("actuator");
    actuator->buildJson(&actuatorObj);
    String payload;
    serializeJson(doc, payload);

    String topic = buildTopic(ACTUATOR_STATE_CHANGED, actuator->getName());

    bool noError = MQTTHandler::getInstance().publish(topic, payload);
    
    if(noError && WoTHandler::getInstance().isMaster()) {
        String fullTopic = MQTTHandler::attachTopicPrefix(topic);
        WoTHandler::getInstance().getMaster()->onMessageReceived(fullTopic, payload);
    }
}

void WoTClient::publishClientInfo() {
    println("PUBLISHING CLIENT INFO");
    DynamicJsonDocument doc(2048);

    JsonObject obj = doc.to<JsonObject>();
    obj["mac_address"] = WiFiHandler::getInstance().getMacAddress();
    
    JsonArray sensorsArray = obj.createNestedArray("sensors"); 
    for(WoTSensor* sensor : WoTSensor::getSensorList()) {
        JsonObject sensorObj = sensorsArray.createNestedObject();
        sensor->buildDiscoveryJsonObject(&sensorObj);
    }

    JsonArray actuatorsArray = obj.createNestedArray("actuators"); 
    for(WoTActuator* actuator : WoTActuator::getAllActuators()) {
        JsonObject actuatorObj = actuatorsArray.createNestedObject();
        actuator->buildDiscoveryJsonObject(&actuatorObj);
    }

    String jsonString;
    serializeJson(doc, jsonString);

    println(jsonString);

    MQTTHandler::getInstance().publish(NEW_CLIENT_INFO, jsonString);

    if(WoTHandler::getInstance().isMaster()) {
        String fullTopic = MQTTHandler::attachTopicPrefix(NEW_CLIENT_INFO);
        WoTHandler::getInstance().getMaster()->onMessageReceived(fullTopic, jsonString);
    }
}

void WoTClient::onMessageReceived(String& topic, String& payload) {
    if(MQTTHandler::matchTopic(topic, TRIGGER_ALL_CLIENT_INFO)) {
        publishClientInfo();
    }
}

void WoTClient::onMQTTConnected() {
    publishClientInfo();
}