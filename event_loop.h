#pragma once

#include "event.h"
#include "event_pool.h"

class EventLoop {
    private:
        EventPool events;
        bool runing;
    public:
        EventLoop() { }
        ~EventLoop() { }

        void exec() {
            runing = true;
            while (runing) {
                if (events.hasReadyEvent()) {
                    auto event_pair = events.getReadyEvent();
                    auto event = event_pair.second;
                    event->stopTracking();
                    event->run();
                    if (event->needRemove()) {
                        events.removeEvent(event_pair.first);
                    } else {
                        event->startTracking();
                    }
                }
            }
        }

        event_id_t addEvent(event_t event) {
            return events.addEvent(event);
        }

        void removeEvent(event_id_t id) {
            events.removeEvent(id);
        }

        void stop() {
            runing = false;
        }
};