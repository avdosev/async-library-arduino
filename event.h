#include <functional>

class Event {
    public:
        using callback_t = std::function<void(void)>;
    private:
        callback_t callback;
    protected:
        
    public:
        Event(callback_t callback) : callback(callback) { }

        void run() {
            callback();
        }

        virtual bool isReady();

        virtual void startTracking();
        virtual void stopTracking();
        
};