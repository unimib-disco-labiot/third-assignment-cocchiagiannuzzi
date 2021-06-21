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


#include "ESPDailyTaskNTP.h"
#include "GreenhouseHandler.h"

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

//OpenWeather Config
const char weather_server[] = "api.openweathermap.org";
const char weather_query[] = "GET /data/2.5/onecall?lat=%f&lon=%f&units=metric&exclude=current,minutely,hourly,alerts&appid=%s";
#define WEATHER_API_KEY "6532cc005b43169ee7229801372f15e5"
float latitude = 45.5959f;
float longitude = 9.0757f;


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

typedef struct {
  bool shouldWakeUpMore;
  int wakeUpCounter;
} rtcStruct __attribute__((aligned(4)));
rtcStruct rtcData;

#define AWAKE_TIME_SEC 10 //TODO: Da cambiare dopo aver calcolato consumo batteria
#define WAKE_UP_HOUR 20
#define WAKE_UP_MIN 35
#define TIME_ZONE 2.0
#define RTC_MEM_ADDR 129
#define SHORT_SLEEP_TIME_SEC 20//3600L //1 hour
#define SHORT_SLEEP_WAKEUPS 2//24
#define HOT_TEMP 25.0f

uint64_t shortSleepTime_us = 1000000L * SHORT_SLEEP_TIME_SEC;

Ticker ticker;
ESPDailyTaskNTP dailyTask(WAKE_UP_HOUR, WAKE_UP_MIN, TIME_ZONE, ssid, pass);
GreenhouseHandler greenhouse;

bool getTomorrowsAvgTemp(float& temp_out) {

  WiFiHandler::getInstance().init(ssid, pass);
  WiFiHandler::connectLoopWifi();
  WiFiClient* client = &WiFiHandler::getClient();

  if (client->connect(weather_server, 80)) {
    char request[200];
    sprintf(request, weather_query, latitude, longitude, WEATHER_API_KEY);
    client->println(request);
    client->println(F("Host: api.openweathermap.org"));
    client->println(F("User-Agent: ArduinoWiFi/1.1"));
    client->println(F("Connection: close"));
    client->println();
  } else {
    Serial.println(F("Connection to api.openweathermap.org failed!\n"));
    return false;
  }

  while (client->connected() && !client->available()) {
    delay(1);   // wait for data
  }
  String result;
  while (client->connected() || client->available()) {   // read data
    char c = client->read();
    result = result + c;
  }
  client->stop();   // end communication

  result.replace(String((char)255), "");
  println(result); 
  
  DynamicJsonDocument doc(6144);
  DeserializationError error = deserializeJson(doc, result);

  if (error) {
    print("deserializeJson() failed: ");
    println(error.c_str());
    return false;
  }
  
  temp_out = doc["daily"][1]["temp"]["day"].as<float>();
  doc.clear();
  
  return true;
}

void setup() {
  debugPrintBegin(115200);
  println("Hello!");

    int readSuccess = system_rtc_mem_read(RTC_MEM_ADDR, &rtcData, sizeof(rtcData));
    println(rtcData.shouldWakeUpMore);
    println(rtcData.wakeUpCounter);
    if(!readSuccess || rtcData.wakeUpCounter > 100000) {
      rtcData.shouldWakeUpMore = false;
      rtcData.wakeUpCounter = -1;
      system_rtc_mem_write(RTC_MEM_ADDR, &rtcData, sizeof(rtcData));
      println("Read RTC memory failed");
    }

// rtcData.shouldWakeUpMore = false;
    if(!rtcData.shouldWakeUpMore) {
      dailyTask.sleepOneDay();

      float tomorrowsAvgTemp = -1;
      bool ok = getTomorrowsAvgTemp(tomorrowsAvgTemp);
      println(String("Checking if tomorrow's temperature is going to be over ") + HOT_TEMP);
      if(ok) {
        if(tomorrowsAvgTemp > HOT_TEMP) {
          rtcData.shouldWakeUpMore = true;
          rtcData.wakeUpCounter = SHORT_SLEEP_WAKEUPS;

          println(String("Tomorrow is going to be hot(") + tomorrowsAvgTemp + "°C). I'm going to wake up each " + SHORT_SLEEP_TIME_SEC + " seconds for " + SHORT_SLEEP_WAKEUPS + " times." );
          
          system_rtc_mem_write(RTC_MEM_ADDR, &rtcData, sizeof(rtcData));
        }
      }
      else {
        println(String("Unable to connect to ") + weather_server);
      }
    }

    println("Starting up!");

    greenhouse.init();

    
    WoTSensor::initAllSensors();
    WoTActuator::initAllActuators();
    println("Sensors and Actuators initialized");

    WiFiHandler::getInstance().init(ssid, pass);
    println("WiFi initialized");
    MYSQLHandler::getInstance().init(mysql_user, mysql_password, mysql_port, mysql_server_addr, WiFiHandler::getInstance().getClient());
    println("MYSQLHandler initialized");
    InfluxDBHandler::getInstance().init(influx_url, influx_org, influx_bucket, influx_token, influx_device_name);
    println("InfluxDBHandler initialized");
    MQTTHandler::getInstance().init(WiFiHandler::getClient(), mqttBrokerIP, mqttPort, mqttClientID.c_str(), mqttUsername, mqttPassword);
    println("InfluxDBHandler initialized");

    WoTHandler::getInstance().init();
    println("WoTHandler initialized");


    println("Setup DONE");


    ticker.attach_scheduled(AWAKE_TIME_SEC, []() {
      MQTTHandler::getClient().disconnect();
      WiFiClient::stopAll();

      if(rtcData.shouldWakeUpMore) {
        if(rtcData.wakeUpCounter > 0) {
          rtcData.wakeUpCounter--;
        }
        else {
          rtcData.shouldWakeUpMore = false;
        }
        
        system_rtc_mem_write(RTC_MEM_ADDR, &rtcData, sizeof(rtcData));

        println(String() + rtcData.wakeUpCounter + " wakeups remaining. Sleeping for " + SHORT_SLEEP_TIME_SEC + " seconds.");

        ESP.deepSleep(shortSleepTime_us);
      }
      else{
          println(String("Goung to sleep until ") + WAKE_UP_HOUR + ":" + WAKE_UP_MIN);
          dailyTask.backToSleep();
      }
    });
}

void loop() {
  ScheduledTicker::getInstance().update();
  MQTTHandler::getClient().loop();
}