#ifndef WOT_MASTERSIDE_CLIENT
#define WOT_MASTERSIDE_CLIENT

#include <ArduinoJson.h>
#include <vector>
#include "webofthings/WoTSensor.h"

class WoTSensor;

class WoTMasterClient {
private:
    std::vector<WoTSensor*> wotSensors;
    String macAddress;

    void addSensor(WoTSensor* sensor);

public:
    const String& getMacAddress();

    void parseNewClientJson(const String& jsonString);
    WoTSensor* findSensor(const String& sensorName) const;
    
    ~WoTMasterClient();

};

#endif //WOT_MASTERSIDE_CLIENT