#include <Arduino.h>
#include <event_loop.h>
#include <interval.h>

EventLoop event_loop;

void setInterval(callback_t callback, uint32_t time) {
    event_loop.addEvent(makeInterval(callback, time, millis));
}

void setTimeout(callback_t callback, uint32_t time) {
    event_loop.addEvent(makeTimer(callback, time, millis));
}

void setup() {
    Serial.begin(115200);

    // добавляем события
    setInterval([](){
        static int counter = 0;
        Serial.print("interval "),
        Serial.println(counter);
        
        setTimeout([counter](){
            Serial.print("timeout after interval with counter - ");
            Serial.println(counter);
            if (counter >= 5) event_loop.stop();
        }, 1500); // 1,5 секунды

        counter++;
    }, 3000); // интервал в 3 секунды
}

void loop() { 
    event_loop.runNext();
}