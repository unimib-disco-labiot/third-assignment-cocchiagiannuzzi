#include "webofthings/WoTHandler.h"

WoTHandler& WoTHandler::getInstance() {
    static WoTHandler instance;
    return instance;
}

WoTHandler::WoTHandler() {
    //Subscription topics in init() method
}

WoTHandler::~WoTHandler()  {
    delete master;
    delete client;
}

void WoTHandler::checkMasterPresence() {
    if(maxAttempts == 0) {
        isLookingForMaster = false;
        onMasterChecked(false);
        return;
    }

    isLookingForMaster = true;
    MQTTHandler::getInstance().publish(MASTER_INFO_TRIGGER, "");
    ticker.once_ms_scheduled(delayAttempts_ms, []() {
            if(WoTHandler::getInstance().masterFound) {
                WoTHandler::getInstance().onMasterChecked(true);
            }
            else{
                WoTHandler::getInstance().checkMasterPresence((WoTHandler::getInstance().maxAttempts - 1), WoTHandler::getInstance().delayAttempts_ms);
            }
        }
    );
}

void WoTHandler::onMasterChecked(bool isMasterFound) {
    if(masterFound) {
        print("Master found with ");
        print(maxAttempts);
        println(" remaining");
    }
    else {
        println("Master not found. Instancing new master.");
        if(master == nullptr) {
            master = new WoTMaster();
        }
    }
    
    if(client == nullptr) {
        println("Instancing new client.");
        client = new WoTClient();
    }

    MQTTHandler::getInstance().updateSubscriptions();
    
    isLookingForMaster = false;
    masterFound = false;
    maxAttempts = 0;
    delayAttempts_ms = 0;
}

void WoTHandler::init() {
    std::map<String, int> topics;
    topics.insert(std::pair<String, int>(MASTER_INFO, LWMQTT_QOS1));
    topics.insert(std::pair<String, int>(DEVICE_DISCONNECTED, LWMQTT_QOS2));
    MQTTHandler::getInstance().attachObserver(this, topics);
}

void WoTHandler::onMessageReceived(String& topic, String& payload) {
    if(MQTTHandler::matchTopic(topic, MASTER_INFO)) {
        if(isLookingForMaster) {
            masterFound = true;
        }
    }
    else if(MQTTHandler::matchTopic(topic, DEVICE_DISCONNECTED)){
        StaticJsonDocument<48> doc;
        deserializeJson(doc, payload);
        const char* mac_address = doc["mac_address"];
        onDeviceDisconnected(mac_address);
    }
}

void WoTHandler::onMQTTConnected() {
    println("\nChecking master presence");
    checkMasterPresence(5, 200);
}

bool WoTHandler::isInitialized(){
    return getMaster() != nullptr || getClient() != nullptr;
}

bool WoTHandler::isMaster() {
    return master != nullptr;
}

WoTMaster* WoTHandler::getMaster() {
    return master;
}

WoTClient* WoTHandler::getClient(){
    return client;
}

void WoTHandler::onDeviceDisconnected(const String& mac_address) {
    static const int MAX_WAIT_TIME_SEC = 15;

    if(!isMaster()) {
        int waitTime = random(MAX_WAIT_TIME_SEC+1);

        ticker.once_scheduled(waitTime, [this]() {
                checkMasterPresence(5, 200);
            }
        );
    }
}