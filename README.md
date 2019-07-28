# Небольшая библиотека для ассинхронной работы на Ардуино 

Возможности:
1. удобная работа с временными интервалами и таймерами
2. все(

**Warning** библиотека расчитана на однопоточное выполнение и не безопасна при многопоточном использовании

## Что есть

class:
1. EventLoop - объект цикла событий
1. Event - объект родитель для всех евентов
1. Interval - выполняет callback с заданным интервалом 
1. Timer - по истечении времени выполнит один раз свой callback


## Использование

Пример с интервалами

~~~c++
#include <event_loop.h>
#include <interval.h>

// функция тикер, для проверки временных промежутков Интервала и Таймера
uint32_t tiker() {
    return millis();
}

EventLoop event_loop;

void setup() {
    Serial.begin(115200);

    // создаем события интервала
    
    int counter = 0;
    event_t new_event1((Event*) new Interval([&event_loop, &counter](){
        Serial.println("interval 1");
        counter++;
        if (counter >= 2) {
            event_loop.stop(); // останавливаем цикл событий
        }
    }, 3000, tiker)); // 3 секунды
    
    event_t new_event2 ((Event*) new Interval([](){
        Serial.println("interval 2");
    }, 1000, tiker)); // 1 секунда
    
    event_t new_event3 ((Event*) new Interval([](){
        Serial.println("interval 3");
    }, 2000, tiker)); // 2 секунды
    
    
    
    // добавляем события
    event_loop.addEvent(new_event1);
    event_loop.addEvent(new_event2);
    event_loop.addEvent(new_event3);
    
    event_loop.exec(); // запускаем цикл событий
    
}

void loop() { }

~~~
Вывод: \
(на разных устройствах может немного отличаться)
~~~
interval 2
interval 3
interval 2
interval 1
interval 2
interval 3
interval 2
interval 2
interval 3
interval 1
~~~

Пример с использованием таймера и интервала

~~~c++
#include <event_loop.h>
#include <interval.h>
#include <timer.h>

// функция тикер, для проверки временных промежутков Интервала и Таймера
uint32_t tiker() {
    return millis();
}

EventLoop event_loop;

void setup() {
    Serial.begin(115200);

    // создаем события интервала
    
    int counter = 0;
    event_t new_event((Event*) new Interval([&event_loop, &counter](){
        Serial.print("interval "),
        Serial.println(counter);
        
        event_loop.addEvent((Event*) new Timer([counter, &event_loop](){
            Serial.print("timeout after interval with counter - ");
            Serial.println(counter);
            if (counter >= 5) event_loop.stop();
        }, 1500, tiker)); // 1,5 секунды

        counter++;

    }, 3000, tiker)); // 3 секунды
    
    // добавляем события
    event_loop.addEvent(new_event);
    
    event_loop.exec(); // запускаем цикл событий

    Serial.println("i am after event loop");
}

void loop() { }
~~~

Вывод

~~~
16:48:48.121 -> interval 0
16:48:49.631 -> timeout after interval with counter - 0
16:48:51.118 -> interval 1
16:48:52.618 -> timeout after interval with counter - 1
16:48:54.130 -> interval 2
16:48:55.604 -> timeout after interval with counter - 2
16:48:57.121 -> interval 3
16:48:58.605 -> timeout after interval with counter - 3
16:49:00.122 -> interval 4
16:49:01.624 -> timeout after interval with counter - 4
16:49:03.119 -> interval 5
16:49:04.618 -> timeout after interval with counter - 5
16:49:04.618 -> i am after event loop
~~~