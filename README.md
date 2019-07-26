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
#include <iostream>
#include <ctime>

#include "event_loop.h"
#include "interval.h"

using namespace std;

int main () {
    srand(time(0));
    EventLoop loop([]() {
        return rand();
    });
    
    int counter = 0;
    EventLoop::event_t new_event1((Event*) new Interval([&loop, &counter](){
        cout << "interval 1" << endl;
        counter++;
        if (counter >= 2) {
            loop.stop();
        }
    }, 3, []() -> uint32_t {
        return time(0);
    }));
    
    EventLoop::event_t new_event2 ((Event*) new Interval([](){
        cout << "interval 2" << endl;
    }, 1, []() -> uint32_t {
        return time(0);
    }));
    
    EventLoop::event_t new_event3 ((Event*) new Interval([](){
        cout << "interval 3" << endl;
    }, 2, []() -> uint32_t {
        return time(0);
    }));
    
    loop.addEvent(new_event1);
    loop.addEvent(new_event2);
    loop.addEvent(new_event3);
    
    loop.exec();
    
    return 0;
}

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