#ifndef MQ4_SENSOR_H
#define MQ4_SENSOR_H

#include "webofthings/WoTSensor.h"
#include <MQUnifiedsensor.h>

#define Voltage_Resolution 3.3f
#define ADC_Bit_Resolution 10
#define RatioMQ4CleanAir 4.4

class MQ4Sensor : public WoTSensor {
private:
    MQUnifiedsensor* MQ4;

    Reading* getLPGReading() {
        return readings[0];
    }

public:
    MQ4Sensor(uint8_t _pin, unsigned long updatePeriod = 1000) : WoTSensor("Gas Sensor", updatePeriod) {
        MQ4 =  new MQUnifiedsensor("ESP8266", Voltage_Resolution, ADC_Bit_Resolution, _pin, "MQ-4");

        addReading(new Reading("LPG", Reading::FLOAT, "burn", "ppM"));
    }

    ~MQ4Sensor() {
        delete MQ4;
    }

    void init() override{
        MQ4->setRegressionMethod(0);
        MQ4->init(); 

        print("Calibrating MQ-4 please wait.");
        float calcR0 = 0;
        for(int i = 1; i<=10; i ++) {
            MQ4->update();
            calcR0 += MQ4->calibrate(RatioMQ4CleanAir);
            print(".");
        }
        MQ4->setR0(calcR0/10);
        println("  done!.");                                        
        
        if(isinf(calcR0)) {
            println("Warning: Conection issue founded, R0 is infite (Open circuit detected) please check your wiring and supply"); 
            while(1);
        }
        if(calcR0 == 0){
            println("Warning: Conection issue founded, R0 is zero (Analog pin with short circuit to ground) please check your wiring and supply"); 
            while(1);
        }
        
        startTicker();
    }
    
    void sensorTick() override {
        MQ4->update();

        MQ4->setA(-0.318); 
        MQ4->setB(1.133);
        getLPGReading()->setValue(MQ4->readSensor());

        notifyMQTTClient();
    }

    float getLPGValue() {
        return *((float*) getLPGReading()->getValue());
    }
};

#endif //MQ4_SENSOR_H