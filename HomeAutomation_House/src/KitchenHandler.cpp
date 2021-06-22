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

void KitchenHandler::botExecMessages() {
for (int i = 1; i < bot.message[0][0].toInt() + 1; i++) {
    String messageRcvd = bot.message[i][5];
    Serial.println(messageRcvd);

    if (messageRcvd.equals("/kitchenLightON")) {
      light1->setState(true);
      light2->setState(true);
      bot.sendMessage(bot.message[i][4], "Kitchen Light ON", "");
    }
    else if (messageRcvd.equals("/kitchenLightOFF")) {
      light1->setState(false);
      light2->setState(false);
      bot.sendMessage(bot.message[i][4], "Kitchen Light OFF", "");
    }
    else if (messageRcvd.equals("/help") || messageRcvd.equals("/start")) {
      String welcome = "Welcome from CocchiaGiannuzziBotIoT!";
      String message = "";
      message += "/kitchenLightON :%0A  to turn the kitchen light ON %0A%0A ";
      message += "/kitchenLightOFF :%0A  to turn the kitchen light OFF  %0A%0A ";
      
      bot.sendMessage(bot.message[i][4], welcome, "");
      bot.sendMessage(bot.message[i][4], message, "");
    } 
    else {
      bot.sendMessage(bot.message[i][4], F("Unknown command! Use /help to see all the available commands"), "");
    }
  }
  bot.message[0][0] = "";   // all messages have been replied, reset new messages
}

void KitchenHandler::init() {

    telegramTicker.attach_ms_scheduled(BOT_MTBS, [this]() {
        static bool init = false;
        if(!init && WiFi.status() == WL_CONNECTED) {
            bot.begin();
            init = true;
        }
        bot.getUpdates(bot.message[0][1]);
        botExecMessages();
        for (int i = 1; i < bot.message[0][0].toInt() + 1; i++) {
            bot.sendMessage(bot.message[i][4], "Test.ugz", "");
        }
    });

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
