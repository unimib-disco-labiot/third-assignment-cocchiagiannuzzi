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

void EntranceHandler::botExecMessages() {
for (int i = 1; i < bot.message[0][0].toInt() + 1; i++) {
    String messageRcvd = bot.message[i][5];
    Serial.println(messageRcvd);

    if (messageRcvd.equals("/entranceLightON")) {
      light1->setState(true);
      light2->setState(true);
      bot.sendMessage(bot.message[i][4], "Entrance Light ON", "");
    }
    else if (messageRcvd.equals("/entranceLightOFF")) {
      light1->setState(false);
      light2->setState(false);
      bot.sendMessage(bot.message[i][4], "Entrance Light OFF", "");
    }
    else if (messageRcvd.equals("/help") || messageRcvd.equals("/start")) {
      String welcome = "Welcome from CocchiaGiannuzziBotIoT!";
      String message = "";
      message += "/entranceLightON :%0A  to turn the entrance light ON  %0A%0A ";
      message += "/entranceLightOFF :%0A  to turn the entrance light OFF  %0A%0A  ";
      
      bot.sendMessage(bot.message[i][4], welcome, "");
      bot.sendMessage(bot.message[i][4], message, "");
    } 
    else {
      bot.sendMessage(bot.message[i][4], F("Unknown command! Use /help to see all the available commands"), "");
    }
  }
  bot.message[0][0] = "";   // all messages have been replied, reset new messages
}

void EntranceHandler::init() {
    digitalWrite(BUZZER_PIN, LOW);
    pinMode(BUZZER_PIN, OUTPUT);

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
