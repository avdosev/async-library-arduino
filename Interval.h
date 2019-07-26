#pragma once

#include "event.h"

class Interval : public Event {
    private: 
        uint32_t interval_start;
        uint32_t interval;
        bool state;
        std::function<uint32_t(void)> tiker; // функция для проверки времени
    public:
        Interval(Event::callback_t callback, uint32_t interval, std::function<uint32_t(void)> time_cheker) :
            Event(callback),
            interval(interval),
            tiker(time_cheker) 
        {

        }

        bool isReady() {
            if (!state) {
                return false;
            }

            const uint32_t work_time = tiker() - interval_start;
            
            return work_time >= this->interval;
        }

        void startTracking() {
            interval_start = tiker();
            state = true;
        }

        void stopTracking() {
            state = false;
        }

        bool needRemove() {
            return false;
        }

        virtual ~Interval() {}

};