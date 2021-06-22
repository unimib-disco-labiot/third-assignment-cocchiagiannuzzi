#include "EntranceHandler.h"
#include "MYSQLHandler.h"
#include <functional>

EntranceHandler::EntranceHandler() {
    wifiSensor = new WiFiSignalSensor();
    aht20 = new AHT20Sensor();
    light1 = new WoTLed("Entrance Light 1", LIGHT_PIN1);
    light2 = new WoTLed("Entrance Light 2", LIGHT_PIN2);
    pirSensor = new PIRSensor(PIR_PIN);
}

EntranceHandler::~EntranceHandler() {
    delete wifiSensor;
    delete aht20;
    delete light1;
    delete light2;
    delete pirSensor;
}

void EntranceHandler::soundAlarm() {
    static const char queryTemplate[] = "INSERT INTO lcocchia.alerts (mac_address, sensorName, alert_message) VALUES ('%s', '%s', '%s')";

    digitalWrite(BUZZER_PIN, HIGH);
    ticker.once_scheduled(2, []() {
        digitalWrite(BUZZER_PIN, LOW);
    });

    char query[200];
    sprintf(query, queryTemplate, WiFiHandler::getInstance().getMacAddress().c_str(), "Entrance Motion Sensor", "Something moved in front of the entrance.");
    MYSQLHandler::getInstance().executeQuery(query);

    //TODO: Telegram
}

void EntranceHandler::init() {
    digitalWrite(BUZZER_PIN, LOW);
    pinMode(BUZZER_PIN, OUTPUT);

    //TODO: Telegram init

    // Events setup
    pirSensor->addEvent(new WoTEvent(
        [](WoTSensor* sensor) {
            PIRSensor* pir = (PIRSensor*) sensor;

            return pir->getMovementValue();
        },
        [this](WoTSensor* sensor) {
            soundAlarm();
        }
    ));
}
