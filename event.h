#pragma once

#include <functional>

class Event {
    public:
        using callback_t = std::function<void(void)>;
    protected:
        callback_t callback;
    public:
        Event(callback_t callback) : callback(callback) { }

        virtual void run() {
            callback();
        }

        virtual bool isReady() = 0;

        virtual void startTracking() = 0;
        virtual void stopTracking() = 0;
        virtual bool needRemove() = 0;
        
        virtual ~Event() {}
};