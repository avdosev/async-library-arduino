#pragma once

#include <stdlib.h>

#include "event_define.h"
#include "event.h"

class EventPool {
    private:
        size_t size, max_size;
        event_pair* events;
    public:
        EventPool() {
            size = 0; max_size = 10;
            events = new event_pair[max_size];
        }

        ~EventPool() {
            delete[] events;
            events = nullptr;
        }

        event_id_t generateUniqueEventId() {
            event_id_t id;
            bool find;

            do {
                id = rand();
                find = id == 0 && this->find(id) == this->end();
            } while (find);

            return id;
        }

        bool hasReadyEvent() {
            for (auto map_item : *this) {
                event_t item = map_item.second;
                if (item->isReady()) return true;
            }
            return false;
        }

        event_pair getReadyEvent() {
            for (auto map_item : *this) {
                event_t item = map_item.second;
                if (item->isReady()) return map_item;
            }
            return event_pair{0, nullptr};
        }

        event_t getEvent(event_id_t id) {
            auto it = this->find(id);
            if (it != this->end()) {
                return it->second;
            } else {
                return event_t(nullptr);
            }
            
        }

        event_id_t addEvent(event_t event) {
            event_id_t id = generateUniqueEventId();
            this->insert(event_pair{id, event});
            event->startTracking();
            return id;
        }

        void removeEvent(event_id_t id) {
            for (size_t i = 0; i < size; i++) {
                if (events[i].first == id) {
                    for (size_t j = i; j < size-1; j++) {
                        events[j] = events[j+1];
                    }
                    size--;
                    break;
                }
            }
        }

        event_pair* begin() const{
            return events;
        }

        event_pair* end() const {
            return events+size;
        }

        event_pair* find(event_id_t id) {
            for (auto it = this->begin(); it != this->end(); it++) {
                event_t item = it->second;
                if (item->isReady())
                    return it;
            }
            return this->end();
        }

        void insert(event_pair pr) {
            if (size == max_size) {
                max_size = (max_size+1) * 2;
                auto c_events = new event_pair[max_size];
                for (size_t i = 0; i < size; i++) {
                    c_events[i] = events[i];
                }
                delete[] events;
                events = c_events;
            }

            events[size] = pr;
            size++;
        }
};