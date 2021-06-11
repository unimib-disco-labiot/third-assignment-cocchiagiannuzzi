#ifndef WOTHANDLER_H
#define WOTHANDLER_H

#include "Singleton.h"
#include "Observer.h"
#include "WoTClient.h"
#include "MQTTHandler.h"
#include "WString.h"
#include "topicNames.h"
#include "WoTMaster.h"
#include "WoTClient.h"
#include <Ticker.h>
#include "debugPrint.h"

class WoTClient;
class WoTMaster;

class WoTHandler : Singleton, public IObserver{
private:
    WoTMaster* master;
    WoTClient* client;

    Ticker ticker;

    bool isLookingForMaster = false;
    bool masterFound = false;
    int maxAttempts = 0;     //Workaround because
    int delayAttempts_ms = 0;//these 2 has to be passed to a void(void) lambda

    WoTHandler();

    void checkMasterPresence();
   
    void checkMasterPresence(int _maxAttempts, int _delayAttempts_ms) {
        maxAttempts = _maxAttempts;
        delayAttempts_ms = _delayAttempts_ms;
        checkMasterPresence();
    }

    void onMasterChecked(bool isMasterFound);

    void onDeviceDisconnected(const String& mac_address);

public:
    ~WoTHandler();

    static WoTHandler& getInstance();

    void init();

    void onMessageReceived(String& topic, String& payload) override;
    
    void onMQTTConnected() override;

    bool isInitialized();

    bool isMaster();
    WoTMaster* getMaster();

    WoTClient* getClient();
};


#endif //WOTHANDLER_H