#ifndef TOPIC_NAMES_H
#define TOPIC_NAMES_H

//Device
#define DEVICE_DISCONNECTED "device_disconnected"

//Master
#define MASTER_INFO "master/info"
#define MASTER_INFO_TRIGGER "master/info/get"
#define MASTER_INFLUXDB "master/influx_db"

//Client
#define SENSOR_DATA_UPDATE "sensor_data_update"
#define NEW_CLIENT_INFO "new_client_info"
#define TRIGGER_ALL_CLIENT_INFO "new_client_info/get"
#define SET_ACTUATOR_STATE "actuator_state/set"
#define ACTUATOR_STATE_CHANGED "actuator_state"

//Readings
#define READINGTYPEBASE_VALUENAME "value"

#endif