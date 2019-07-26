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
    public:
        EventLoop() {

        }

        void exec() {
            runing = true;
            while (runing) {
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

        void stop() {
            runing = false;
        }

        std::pair<event_id_t, event_t> getReadyEvent() {
            for (auto map_item : events) {
                event_t item = map_item.second;
                if (item->isReady()) return map_item;
            }
        }

        event_id_t addEvent(event_t event) {
            event_id_t id = generateUniqueEventId();
            events.insert(std::pair(id, event));
            return id;
        }

    private:
        event_id_t generateUniqueEventId() {

        }

    public:
        void removeEvent(event_id_t id) {
            events.erase(id);
        }

};