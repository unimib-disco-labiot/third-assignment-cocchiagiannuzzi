#ifndef WOTMASTER_H
#define WOTMASTER_H

#include "Observer.h"
#include "WoTMasterClient.h"

class WoTMasterClient;

class WoTMaster : public IObserver{
private:
    std::vector<WoTMasterClient*> clients;

    static void sendMasterInfo();

    WoTMasterClient* findClient(const String& _macAddress) const;

    void onSensorDataUpdate(const String& jsonString);

    void onNewClientConnected(const String& jsonString);
    void onClientDisconnected(const String& _macAddress);

public:
    WoTMaster();
    ~WoTMaster();

    void onMessageReceived(String& topic, String& payload) override;
    
    void onMQTTConnected() override;
};

#endif //WOTMASTER_H