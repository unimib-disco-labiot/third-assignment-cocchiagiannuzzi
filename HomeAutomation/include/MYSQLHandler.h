#ifndef MYSQL_HANDLER_H
#define MYSQL_HANDLER_H

#include <ESP8266WiFi.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

#include "Singleton.h"
#include "debugPrint.h"

class MYSQLHandler : Singleton {
private:
    bool isActive = true;

    MySQL_Connection* connection;
    const char* mysql_user;
    const char* mysql_password; 
    int mysql_port;  
    IPAddress* server_address;  
   
public:
    static MYSQLHandler& getInstance() {
        static MYSQLHandler instance;
        return instance;
    }

    void init(const char* _mysql_user, const char* _mysql_password, int _mysql_port, IPAddress& _server_address, WiFiClient& _client){
        mysql_user = _mysql_user;
        mysql_password = _mysql_password;
        mysql_port = _mysql_port;
        server_address = &_server_address;
        connection = new MySQL_Connection((Client*)&_client);
    }

    ~MYSQLHandler(){
        delete connection;
        //delete server_address;
    }

    bool isEnabled() {
        return isActive;
    }

    void setEnabled(bool _isEnabled) {
        isActive = _isEnabled;
    }

    void executeQuery(const char* query) {
        if(!isActive){
            return;
        }

        if (connection->connect(*server_address, mysql_port, (char*)mysql_user, (char*)mysql_password)) {
            MySQL_Cursor *cur_mem = new MySQL_Cursor(connection);
            cur_mem->execute(query);
            delete cur_mem;
            connection->close();
            println("Query executed.");
        } 
        else {
            println("MySQL connection failed.");
        }
    }
};

#endif //MYSQL_HANDLER_H