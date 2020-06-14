#pragma once

#include "event.h"
#include "event_define.h"

class Timer : public Event {
    private: 
        callback_t callback;
        uint32_t interval_start;
        uint32_t interval;
        bool state;
        bool need_remove;
        tiker_t tiker; // функция для проверки времени
    public:
        Timer(callback_t callback, uint32_t interval, tiker_t time_cheker) :
            callback(callback),
            interval(interval),
            tiker(time_cheker),
            need_remove(false)
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
            need_remove = false;
        }

        void stopTracking() {
            state = false;
            need_remove = true;
        }

        bool needRemove() {
            return need_remove;
        }

        void run() {
            state = false;
            callback();
            need_remove = true;
        }

        virtual ~Timer() {}

};

event_t makeTimer(callback_t callback, uint32_t interval, tiker_t time_cheker) {
    return makeEvent<Timer>(callback, interval, time_cheker);
}
