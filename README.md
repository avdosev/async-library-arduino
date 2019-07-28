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

пример с интервалами

~~~c++
#include <event_loop.h>
#include <interval.h>

// функция тикер, для проверки временных промежутков Интервала и Таймера
uint32_t tiker() {
    return time(0);
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
    }, 3, tiker));
    
    event_t new_event2 ((Event*) new Interval([](){
        Serial.println("interval 2");
    }, 1, tiker));
    
    event_t new_event3 ((Event*) new Interval([](){
        Serial.println("interval 3");
    }, 2, tiker));
    
    
    
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