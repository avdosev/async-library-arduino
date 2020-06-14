#pragma once

#include "event_define.h"
#include "event.h"

class IEventPool {
    public:
        virtual ~IEventPool() { }

        virtual bool hasReadyEvent() = 0;
        virtual event_pair getReadyEvent() = 0;
        virtual event_t getEvent(event_id_t id) = 0;
        virtual event_id_t addEvent(event_t event) = 0;
        virtual void removeEvent(event_id_t id) = 0;
};