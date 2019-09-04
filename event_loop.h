#pragma once

#include "event.h"
#include "event_pool.h"

class EventLoop {
    private:
        EventPool events;
        bool _running = true;
    public:
        EventLoop() { }
        ~EventLoop() { }

        void exec() {
            _running = true;
            while (_running) {
                this->runNext();
            }
        }

        void runNext() {
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

        event_id_t addEvent(event_t event) {
            return events.addEvent(event);
        }

        void removeEvent(event_id_t id) {
            events.removeEvent(id);
        }

        event_t getEvent(event_id_t id) {
            return events.getEvent(id);
        }

        void stop() {
            _running = false;
        }

        bool isRunning() const {
            return _running;
        }
        
        bool isFinished() const {
            return !isRunning();
        }

        void sleep(uint32_t time_to_sleep, tiker_t time_cheker) {
            uint32_t start = time_cheker();
            uint32_t current;
            do {
                runNext();
                current = time_cheker();
            } while(isRunning() && (current-start >= time_to_sleep));
        }
};