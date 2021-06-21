#ifndef WOTCLIENT_H
#define WOTCLIENT_H

#include "webofthings/WoTSensor.h"
#include "webofthings/WoTActuator.h"

class WoTSensor;
class WoTActuator;

class WoTClient : public IObserver {
private:
    String buildTopic(const String& topic, const String& sensorName) const;

    void publishClientInfo();

    void onDeviceDisconnected(const String& mac_address);
public:
    WoTClient();
    ~WoTClient();

    void onMessageReceived(String& topic, String& payload) override;

    void onMQTTConnected() override;

    void onSensorReadingUpdate(WoTSensor* sensor);
    void onActuatorStateChanged(const WoTActuator* actuator);
};

#endif //WOTCLIENT_H