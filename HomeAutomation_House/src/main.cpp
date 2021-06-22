#include <Arduino.h>

#include "secrets.h"
// #include "debugPrint.h"
#include "MQTTHandler.h"
#include "WiFiHandler.h"
#include "LCDHandler.h"

// #include "MYSQLHandler.h"
// #include "InfluxDBHandler.h"

#include <Ticker.h>
#include <ArduinoJson.h>

#include "ScheduledTicker.h"
#include "parameters.h"
#include "webofthings/WoTHandler.h"
#include "MYSQLHandler.h"
#include "InfluxDBHandler.h"

#include "webofthings/WoTSensor.h"
#include "webofthings/WoTActuator.h"


#include "EntranceHandler.h"
#include "KitchenHandler.h"

String topic(MQTT_TOPIC_PREFIX);

//WiFi Config
const char ssid[] = SECRET_SSID;
const char pass[] = SECRET_PASS;

//MQTT Config
const char mqttBrokerIP[] = MQTT_BROKERIP;
const int mqttPort = MQTT_PORT;
String mqttClientID = String(MQTT_CLIENTID) + "[" + WiFi.macAddress() + "]";
const char mqttUsername[] = MQTT_USERNAME;
const char mqttPassword[] = MQTT_PASSWORD;

//MySQL Config
char mysql_user[] = MYSQL_USER;       // MySQL user login username
char mysql_password[] = MYSQL_PASS;   // MySQL user login password
int mysql_port = MYSQL_PORT;
IPAddress mysql_server_addr(MYSQL_IP);      // IP of the MySQL *server* here

//InfluxDB Config
const char influx_url[] = INFLUXDB_URL;
const char influx_org[] = INFLUXDB_ORG;
const char influx_bucket[] = INFLUXDB_BUCKET;
const char influx_token[] = INFLUXDB_TOKEN;
const char influx_device_name[] = "home_monitoring";


// Entrance
EntranceHandler entrance;
// KitchenHandler kitchen;


void setup() {
  debugPrintBegin(115200);
  println("Hello!");

    println("Starting up!");


    WoTSensor::initAllSensors();
    WoTActuator::initAllActuators();
    println("Sensors and Actuators initialized");

    WiFiHandler::getInstance().init(ssid, pass);
    println("WiFi initialized");
    MYSQLHandler::getInstance().init(mysql_user, mysql_password, mysql_port, mysql_server_addr);
    println("MYSQLHandler initialized");
    InfluxDBHandler::getInstance().init(influx_url, influx_org, influx_bucket, influx_token, influx_device_name);
    println("InfluxDBHandler initialized");
    MQTTHandler::getInstance().init(WiFiHandler::getClient(), mqttBrokerIP, mqttPort, mqttClientID.c_str(), mqttUsername, mqttPassword);
    println("InfluxDBHandler initialized");

    WoTHandler::getInstance().init();
    println("WoTHandler initialized");

    entrance.init();
    // kitchen.init();

    println("Setup DONE");

}

void loop() {
  ScheduledTicker::getInstance().update();
  MQTTHandler::getClient().loop();
}