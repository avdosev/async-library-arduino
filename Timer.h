#pragma once

#include "event.h"
// #include <Arduino.h>

class Timer : public Event {
    private: 
        uint32_t interval_start;
        uint32_t interval;
        bool state;
        bool need_remove;
        std::function<uint32_t(void)> tiker; // функция для проверки времени
    public:
        Timer(Event::callback_t callback, uint32_t interval, std::function<uint32_t(void)> time_cheker) :
            Event(callback),
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