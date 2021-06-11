#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

#include <ESP8266WiFi.h>
#include <MQTT.h>
#include <Ticker.h>
#include <map>
#include <ArduinoJson.h>

#include "WiFiHandler.h"
#include "Singleton.h"
#include "Observer.h"
#include "debugPrint.h"
#include "parameters.h"
#include "webofthings/topicNames.h"

class MQTTHandler : Singleton, public ISubject {
private:
    Ticker mqttConnectTicker;
    
    const char* clientID;
    const char* username;
    const char* password;

public:
    static MQTTHandler& getInstance() {
        static MQTTHandler instance;
        return instance;
    }
private:
    std::map<IObserver*, std::map<String, int>> list_observer_;

    static void trimTopic(String& topic) {
        if(topic.startsWith("/")) {
            topic = topic.substring(1);
        }
        if(topic.endsWith("/")) {
            topic = topic.substring(0, topic.length() - 1);
        }
    }
    static int findNextDivider(const String& str, int currentPos) {
        static const char divider = '/';

        unsigned int i = currentPos;
        while(i < str.length()) {
            if(str.charAt(i) == divider){
                return i;
            }
            i++;
        }
        return i;
    }
    
public:
    static MQTTClient& getClient() {
        static MQTTClient client(1024);
        return client;
    }

    void init(WiFiClient& wifiClient, const char* brokerIP, int port, const char* _clientID, const char* _username, const char* _password) {
        clientID = _clientID;
        username = _username;
        password = _password;

        getClient().begin(brokerIP, port, wifiClient);
        getClient().onMessage(onMessageReceived);
        getClient().setKeepAlive(3);
        setLastWill();

        mqttConnectTicker.attach_ms_scheduled(500, connectToMQTTBroker);
    }

    static bool matchTopic(String messageTopic, String topic) {
        topic = attachTopicPrefix(topic);

        trimTopic(messageTopic);
        trimTopic(topic);

        unsigned int i = 0;
        unsigned int j = 0;

        char messageChar;
        char topicChar;
        while(i < messageTopic.length() && j < topic.length()) {
            messageChar = messageTopic.charAt(i);
            topicChar = topic.charAt(j);

            if(topicChar == '#'){
                return true;
            }
            else if(topicChar == '+') {
                i = findNextDivider(messageTopic, i) - 1;
            }
            else if(topicChar != messageChar){
                return false;
            }
            i++;
            j++;
        }

        if(i == messageTopic.length() && j == topic.length()) {
            return true;
        }
        return false;
    }

    void attachObserver(IObserver *observer, std::map<String, int>& topics) override {
        list_observer_.insert(std::pair<IObserver*, std::map<String, int>>(observer, topics));
    }
    void detachObserver(IObserver *observer) override {
        // TODO: unsuscribe
        // list_observer_[observer].clear();
        // list_observer_.erase(observer);
    }
    
    void notifyObservers(String& messageTopic, String& payload) override {
        std::map<IObserver *, std::map<String, int>>::iterator iterator;
        
        for (iterator = list_observer_.begin(); iterator != list_observer_.end(); ++iterator) {
            for(std::pair<String, int> subscribedTopic : iterator->second) {
                if(matchTopic(messageTopic, subscribedTopic.first)) {
                    iterator->first->onMessageReceived(messageTopic, payload);
                    break;
                }
            }
        }
    }

    void notifyObserversConnected() override {
        std::map<IObserver *, std::map<String, int>>::iterator iterator;
        
        for (iterator = list_observer_.begin(); iterator != list_observer_.end(); ++iterator) {
            iterator->first->onMQTTConnected();
        }
    }

    static void onMessageReceived(String& topic, String& payload) {
        println("Incoming MQTT message: " + topic + " - " + payload);
        getInstance().notifyObservers(topic, payload);
    }

    bool publish(const String& topic, const String& payload) {
        String fullTopic = attachTopicPrefix(topic);
        // println(topic);
        // println(payload);
        return getClient().publish(fullTopic, payload); 
    }

    bool publish(const String& topic, const String& payload, int qualityOfService) {
        String fullTopic = attachTopicPrefix(topic);
        return getClient().publish(fullTopic, payload, false, qualityOfService);
    }

    void updateSubscriptions() {
        std::map<IObserver *, std::map<String, int>>::iterator iterator;
        
        for (iterator = list_observer_.begin(); iterator != list_observer_.end(); ++iterator) {
            for(std::pair<String,int> topic : iterator->second) {
                getClient().subscribe(attachTopicPrefix(topic.first));
            }
        }
    }

    static String attachTopicPrefix(const String& topic) {
        String out;
        out += MQTT_TOPIC_PREFIX;
        out += "/";
        out += topic;

        return out;
    }
private:
    bool subscribe(const String& _topic) {
        return getClient().subscribe(attachTopicPrefix(_topic));
    }

    bool unsubscribe(const String& _topic) {std::map<IObserver *, std::map<String, int>>::iterator iterator;
        return getClient().unsubscribe(attachTopicPrefix(_topic));
    }

    const String buildLastWillPayload() {
        StaticJsonDocument<48> doc;

        doc["mac_address"] = WiFiHandler::getInstance().getMacAddress();

        String jsonString;
        serializeJsonPretty(doc, jsonString);
        return jsonString;
    }

    void setLastWill() {
        static String lastWillPayload = buildLastWillPayload();
        getClient().setWill(attachTopicPrefix(DEVICE_DISCONNECTED).c_str(), lastWillPayload.c_str(), false, LWMQTT_QOS2);
    }

    void onConnected() {
        notifyObserversConnected();
        updateSubscriptions();
    }

    static void connectToMQTTBroker() {
        if (WiFi.isConnected() && (!getClient().connected())) {
            print("\nConnecting to MQTT broker...");
            while (!getClient().connect(getInstance().clientID,
                    getInstance().username, 
                    getInstance().password)) {
                print(".");
                delay(1000);
            }
            getInstance().onConnected();

            println("\nMQTT Connected!");
        }
    }
};

#endif //MQTT_HANDLER_H