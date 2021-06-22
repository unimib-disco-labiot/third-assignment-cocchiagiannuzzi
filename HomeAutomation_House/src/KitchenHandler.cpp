#include "KitchenHandler.h"
#include "MYSQLHandler.h"
#include <functional>

KitchenHandler::KitchenHandler() {
    mq4 = new MQ4Sensor(MQ4_PIN);
    light1 = new WoTLed("Kitchen Light 1", LIGHT_PIN1);
    light2 = new WoTLed("Kitchen Light 2", LIGHT_PIN2);
    servo = new ServoMotorOpener("Kitchen Window", WINDOW_SERVO_PIN, false, 0, 100);
}

KitchenHandler::~KitchenHandler() {
    delete mq4;
    delete light1;
    delete light2;
    delete servo;
}

void KitchenHandler::soundAlarm() {
    static const char queryTemplate[] = "INSERT INTO lcocchia.alerts (mac_address, sensorName, alert_message) VALUES ('%s', '%s', '%s')";

    servo->setState(true);
    // ticker.once_scheduled(15, []() {
    //     servo->setState(false);
    // });

    char query[200];
    sprintf(query, queryTemplate, WiFiHandler::getInstance().getMacAddress().c_str(), "Gas Sensor", "High Methane levels.");
    MYSQLHandler::getInstance().executeQuery(query);
}

void KitchenHandler::init() {

    // Events setup
    mq4->addEvent(new WoTEvent(
        [](WoTSensor* sensor) {
            MQ4Sensor* mq4 = (MQ4Sensor*) sensor;
            return mq4->getLPGValue() > METHANE_THRESHOLD;
        },
        [this](WoTSensor* sensor) {
            soundAlarm();
        }
    ));
}
