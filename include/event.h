#pragma once

#include "event_define.h"

class Event {
    public:
        virtual void run() = 0;
        virtual bool isReady() = 0;
        
        virtual void startTracking() = 0;
        virtual void stopTracking() = 0;
        virtual bool needRemove() = 0;
        
        virtual ~Event() {}
};

template<class type, typename... args_t>
event_t makeEvent(args_t ...args) {
    return (Event*) new type(args...);
}