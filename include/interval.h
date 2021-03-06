#pragma once

#include "event.h"
#include "event_define.h"

class Interval : public Event {
    private:
        callback_t callback; 
        uint32_t interval_start;
        uint32_t interval;
        bool state;
        tiker_t tiker; // функция для проверки времени
    public:
        Interval(callback_t callback, uint32_t interval, tiker_t time_cheker) :
            callback(callback),
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

        void run() {
            callback();
        }

        virtual ~Interval() {}

};

event_t makeInterval(callback_t callback, uint32_t interval, tiker_t time_cheker) {
    return makeEvent<Interval>(callback, interval, time_cheker);
}