#ifndef SCHEDULED_TICKER_H
#define SCHEDULED_TICKER_H

#include <Arduino.h>
#include <set>
#include "Singleton.h"

class ScheduledTicker : Singleton {
public:
    class Tickable {
    private:
        unsigned long period;
        unsigned long previousMillis = 0;
        bool isActive = false;

    public:
        Tickable(unsigned long _period) {
            if(_period > 0) {
                setTickerPeriod(_period);
                ScheduledTicker::getInstance().attach(this);
            }
        }

        virtual ~Tickable() {
            ScheduledTicker::getInstance().detach(this);
        }

        void startTicker() {
            isActive = true;
        }

        void stopTicker() {
            isActive = false;
        }

        unsigned long getTickerPeriod() {
            return period;
        }

        unsigned long getPreviousMillis() {
            return previousMillis;
        }

        void setTickerPeriod(unsigned long _period) {
            period = _period;
        }

        void setPreviousMillis(unsigned long _previousMillis) {
            previousMillis = _previousMillis;
        }

        bool isTickerActive(){
            return isActive;
        }

        void setTickerActive(bool _isActive) {
            isActive = _isActive;
        }

        void toggleTicker() {
            setTickerActive(!isTickerActive());
        }

        virtual void tick() = 0;
    };
private:
    std::set<Tickable*> tickables;

public:
    static ScheduledTicker& getInstance() {
        static ScheduledTicker instance;
        return instance;
    }

    void update() {
        unsigned long currentMillis = millis();
        for(Tickable* tickable : tickables) {
            if (tickable->isTickerActive() && (currentMillis - tickable->getPreviousMillis() >= tickable->getTickerPeriod())) {
                tickable->setPreviousMillis(currentMillis);
                tickable->tick();
            }
        }
    }

    void attach(Tickable* tickable) {
        tickables.insert(tickable);
    }

    void detach(Tickable* tickable) {
        tickables.erase(tickable);
    }
};

#endif //SCHEDULED_TICKER_H