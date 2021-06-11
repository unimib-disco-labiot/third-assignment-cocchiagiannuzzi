#ifndef INFLUXDB_HANDLER_H
#define INFLUXDB_HANDLER_H

#include <ESP8266WiFi.h>
#include <InfluxDbClient.h>

#include "Singleton.h"
#include "debugPrint.h"

#include "webofthings/Reading.h"

class InfluxDBHandler : Singleton {
private:
    bool isActive = true;

    InfluxDBClient* client;
    Point* pointDevice;

    void check_influxdb() {
        if (client->validateConnection()) {
            print("Connected to InfluxDB: ");
            println(client->getServerUrl());
        } 
        else {
            print("InfluxDB connection failed: ");
            println(client->getLastErrorMessage());
        }
    }

public:
    static InfluxDBHandler& getInstance() {
        static InfluxDBHandler instance;
        return instance;
    }
    
    void init(const char* influxDB_URL, const char* influxDB_ORG, const char* influxDB_bucket, const char* influxDB_token, const char* pointDeviceName){
        client = new InfluxDBClient( influxDB_URL, influxDB_ORG, influxDB_bucket, influxDB_token); 
        pointDevice = new Point(pointDeviceName);
    }

    ~InfluxDBHandler(){
        delete client;
        delete pointDevice;
    }

    bool isEnabled() {
        return isActive;
    }

    void setEnabled(bool _isEnabled) {
        isActive = _isEnabled;
    }

    Point* getPointDevice(){
        return pointDevice;
    }

    void writeReadingsToInfluxDB(const String& device_mac_address, const WoTSensor& sensor, const Reading& reading) {
        if(!isActive){
            return;
        }

        void* value = reading.getValue();
        if(value != nullptr) {
            check_influxdb();
            pointDevice->clearFields();
            pointDevice->clearTags();
            pointDevice->addTag("device", device_mac_address);
            pointDevice->addTag("board", "ESP8266");//TODO: Paramete
            pointDevice->addTag("sensor", sensor.getName());

            pointDevice->addTag("reading_name", sensor.getName());

            switch(reading.getType()) {
                case Reading::INT:
                    pointDevice->addField(reading.getName(), *((int*)value));
                    break;
                case Reading::FLOAT:
                    pointDevice->addField(reading.getName(), *((float*)value));
                    break;
                case Reading::BOOL:
                    pointDevice->addField(reading.getName(), *((bool*)value));
                    break;
                case Reading::STRING:
                    pointDevice->addField(reading.getName(), *((String*)value));
                    break;
                case Reading::EVENT:
                    pointDevice->addField(reading.getName(), "Event occurred");
                    break;
            }

            print("Writing to InfluxDB: ");
            println(pointDevice->toLineProtocol());
            if (!client->writePoint(*pointDevice)) {
                print("InfluxDB write failed: ");
                println(client->getLastErrorMessage());
            }
        }
    }


};

#endif //INFLUXDB_HANDLER_H