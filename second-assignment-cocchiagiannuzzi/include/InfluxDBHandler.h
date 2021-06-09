#ifndef INFLUXDB_HANDLER_H
#define INFLUXDB_HANDLER_H

#include <ESP8266WiFi.h>
#include <InfluxDbClient.h>

#include "Singleton.h"
#include "debugPrint.h"

#include "webofthings/reading/ReadingInt.h"
#include "webofthings/reading/ReadingFloat.h"
#include "webofthings/reading/ReadingBool.h"
#include "webofthings/reading/ReadingString.h"

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

        check_influxdb();
        pointDevice->clearFields();
        pointDevice->clearTags();
        pointDevice->addTag("device", device_mac_address);
        pointDevice->addTag("board", "ESP8266");//TODO: Parameter


        pointDevice->addTag("sensor", sensor.getName());
        pointDevice->addTag("reading_name", reading.getName());

        switch(reading.getType()) {
            case INT:
                pointDevice->addField("reading_value", ((ReadingInt*)reading.getValue())->getValue());
                break;
            case FLOAT:
                pointDevice->addField("reading_value", ((ReadingFloat*)reading.getValue())->getValue());
                break;
            case BOOL:
                pointDevice->addField("reading_value", ((ReadingBool*)reading.getValue())->getValue());
                break;
            case STRING:
                pointDevice->addField("reading_value", ((ReadingString*)reading.getValue())->getValue());
                break;
            case EVENT:
                pointDevice->addField("reading_value", "Event occurred");
                break;
        }

        print("Writing to InfluxDB: ");
        println(pointDevice->toLineProtocol());
        if (!client->writePoint(*pointDevice)) {
            print("InfluxDB write failed: ");
            println(client->getLastErrorMessage());
        }
    }


};

#endif //INFLUXDB_HANDLER_H