#ifndef OBSERVER_DP_H
#define OBSERVER_DP_H

#include "WString.h"
#include <map>

//Observer design pattern

class IObserver {
public:
    virtual ~IObserver(){};
    virtual void onMessageReceived(String& topic, String& payload) = 0;
    virtual void onMQTTConnected() = 0;
};

class ISubject {
public:
    virtual ~ISubject(){};
    virtual void attachObserver(IObserver *observer, std::map<String, int>& topics) = 0;
    virtual void detachObserver(IObserver *observer) = 0;
    virtual void notifyObservers(String& topic, String& payload) = 0;
    virtual void notifyObserversConnected() = 0;
};

#endif //OBSERVER_DP_H