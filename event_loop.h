#pragma once

#include <map>
#include <memory>
#include "event.h"

class EventLoop {
    public:
        using event_id_t = uint32_t;
        using event_t = std::shared_ptr<Event>;
    private:
        bool runing;
        std::map<event_id_t, event_t> events;
        std::function<event_id_t()> rand;
    public:
        EventLoop(std::function<event_id_t()> rand) : rand(rand) {

        }

        void exec() {
            runing = true;
            while (runing) {
                if (hasReadyEvent()) {
                    auto event_pair = getReadyEvent();
                    auto event = event_pair.second;
                    event->stopTracking();
                    event->run();
                    if (event->needRemove()) {
                        removeEvent(event_pair.first);
                    } else {
                        event->startTracking();
                    }
                }
            }
        }

        void stop() {
            runing = false;
        }

        bool hasReadyEvent() {
            for (auto map_item : events) {
                event_t item = map_item.second;
                if (item->isReady()) return true;
            }
            return false;
        }

        std::pair<event_id_t, event_t> getReadyEvent() {
            for (auto map_item : events) {
                event_t item = map_item.second;
                if (item->isReady()) return map_item;
            }
            return std::make_pair(0, nullptr);
        }

        event_id_t addEvent(event_t event) {
            event_id_t id = generateUniqueEventId();
            events.insert(std::make_pair(id, event));
            event->startTracking();
            return id;
        }

    private:
        event_id_t generateUniqueEventId() {
            event_id_t id;
            bool find;

            do {
                id = this->rand();
                find = id == 0 && events.find(id) == events.end();
            } while (find);

            return id;
        }

    public:
        void removeEvent(event_id_t id) {
            events.erase(id);
        }

};