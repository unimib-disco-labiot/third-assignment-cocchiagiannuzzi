#ifndef WIFI_HANDLER_H
#define WIFI_HANDLER_H

#include <ESP8266WiFi.h>
#include <Ticker.h>

#include "Singleton.h"
#include "debugPrint.h"

class WiFiHandler : Singleton {
private:
    String MAC_ADDRESS;
    
    Ticker wifiConnectTicker;
    
    const char* ssid;
    const char* pass;

    #ifdef IP
    IPAddress ip(IP);
    IPAddress subnet(SUBNET);
    IPAddress dns(DNS);
    IPAddress gateway(GATEWAY);
    #endif
public:
    static WiFiHandler& getInstance() {
        static WiFiHandler instance;
        return instance;
    }

    void init(const char* _ssid, const char* _pass, bool shouldStartConnectLoop = true) {
        static bool initDone = false;
        if(!initDone) {
            ssid = _ssid;
            pass = _pass;

            Serial.println("INIT");

            WiFiHandler::initWiFi();
            MAC_ADDRESS = String(WiFi.macAddress());
        }
        if(shouldStartConnectLoop && !wifiConnectTicker.active()) {
            wifiConnectTicker.attach_ms_scheduled(500, connectLoopWifi);
        }
        
        initDone = true;
    }

    const String& getMacAddress() {
        return MAC_ADDRESS;
    }

    static WiFiClient& getClient() {
        static WiFiClient client;
        return client;
    }

    static void connectLoopWifi() {
        if (WiFi.status() != WL_CONNECTED) {
            print("Connecting to SSID: ");
            println(getSSID());

            #ifdef IP
                WiFi.config(ip, dns, gateway, subnet);   // by default network is configured using DHCP
            #endif

            WiFi.begin(getSSID(), getPassword());
            while (WiFi.status() != WL_CONNECTED) {
                print(".");
                delay(250);
            }
            println("\nWiFi Connected!");
        }
    }

private:
    static const char* getSSID() {
        return WiFiHandler::getInstance().ssid;
    }
    
    static const char* getPassword() {
        return WiFiHandler::getInstance().pass;
    }
    
    static void initWiFi() {
        WiFi.mode(WIFI_STA);
        delay(250);
    }
};

#endif //WIFI_HANDLER_H
