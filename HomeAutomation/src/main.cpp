#include <Arduino.h>

#include "secrets.h"
// #include "debugPrint.h"
#include "MQTTHandler.h"
#include "WiFiHandler.h"
#include "LCDHandler.h"

// #include "MYSQLHandler.h"
// #include "InfluxDBHandler.h"

#include "Ticker.h"

#include "ScheduledTicker.h"
#include "parameters.h"
#include "webofthings/WoTHandler.h"
#include "MYSQLHandler.h"
#include "InfluxDBHandler.h"

#include "webofthings/WoTSensor.h"
#include "webofthings/WoTActuator.h"

#include "webofthings/implementations/WiFiSignalSensor.h"
#include "webofthings/implementations/DHTSensor.h"
#include "webofthings/implementations/AnalogSensor.h"
#include "webofthings/implementations/PIRSensor.h"
#include "webofthings/implementations/WoTLed.h"
#include "webofthings/implementations/MQ4Sensor.h"
#include "webofthings/implementations/AHT20Sensor.h"
#include "webofthings/implementations/ServoMotorOpener.h"

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
// WiFiSignalSensor wifiSensor;
// DHTSensor dht22Sensor(D3, DHT22, "DHT_22");
// AnalogSensor photoresistor(A0, "photoresistor", "Light", "lightbulb", "%");
// WoTLed led("Entrance Light", D2);
// PIRSensor pirSensor(D7);

// Kitchen
// WiFiSignalSensor wifiSensor;
// MQ4Sensor mq4Sensor(A0);
// AHT20Sensor aht20;
// WoTLed led("Kitchen Light", D3);

// Greenhouse
DHTSensor dht22Sensor(D6, DHT22, "DHT_22");
AnalogSensor soilMoistureSensor(A0, "soil_moisture", "Soil moisture", "leaf", "%");
#define WINDOW_OPEN_VALUE 140
#define WINDOW_CLOSED_VALUE 40
ServoMotorOpener windowsMotor("Windows Opener", D7, false, WINDOW_OPEN_VALUE, WINDOW_CLOSED_VALUE);
#define PUMP_RELAY_PIN D5


void setup() {
  debugPrintBegin(115200);

  WoTSensor::initAllSensors();
  WoTActuator::initAllActuators();

  WiFiHandler::getInstance().init(ssid, pass);
  MYSQLHandler::getInstance().init(mysql_user, mysql_password, mysql_port, mysql_server_addr, WiFiHandler::getInstance().getClient());
  InfluxDBHandler::getInstance().init(influx_url, influx_org, influx_bucket, influx_token, influx_device_name);
  MQTTHandler::getInstance().init(WiFiHandler::getClient(), mqttBrokerIP, mqttPort, mqttClientID.c_str(), mqttUsername, mqttPassword);

  WoTHandler::getInstance().init();

  //Non wot init
  digitalWrite(PUMP_RELAY_PIN, HIGH);
  pinMode(PUMP_RELAY_PIN, OUTPUT);
  digitalWrite(PUMP_RELAY_PIN, LOW);

  // Events setup
  dht22Sensor.addEvent(new WoTEvent(
    [](WoTSensor* sensor) {
      DHTSensor* dht22 = (DHTSensor*) sensor;
      return dht22->hasValue() && dht22->getTemperatureValue() > 28.0f;
    },
    [](WoTSensor* sensor) {
      println(sensor->getName());
      DHTSensor* dht22 = static_cast<DHTSensor*>(sensor);

      Serial.println(String("TEMP: ") + dht22->getTemperatureValue());
      windowsMotor.setOpen(true);
    }
  ));
  dht22Sensor.addEvent(new WoTEvent(
    [](WoTSensor* sensor) {
      DHTSensor* dht22 = (DHTSensor*) sensor;
      return dht22->hasValue() && dht22->getTemperatureValue() < 24.0f;
    },
    [](WoTSensor* sensor) {
      DHTSensor* dht22 = (DHTSensor*) sensor;
      Serial.println(String("TEMP2: ") + dht22->getTemperatureValue());
      windowsMotor.setOpen(false);
    }
  ));

  println("Setup DONE");
}

void loop() {
  ScheduledTicker::getInstance().update();
  MQTTHandler::getClient().loop();
}