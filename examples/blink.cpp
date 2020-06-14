#include <Arduino.h>
#include <event_loop.h>
#include <interval.h>

EventLoop event_loop;

void blink() {
    static bool state = false;
    digitalWrite(LED_BUILTIN, state = !state);
}

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);

    // добавляем события
    event_loop.addEvent( makeInterval(blink, 1500, millis) ); // интервал в 3 секунды
}

void loop() { 
    event_loop.runNext();
}