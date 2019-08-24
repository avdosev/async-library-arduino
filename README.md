# Библиотека для ассинхронной работы на Ардуино 

Возможности:
1. удобная работа с временными интервалами и таймерами
3. расширяемость с помощью событий
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

    // добавляем события
    event_loop.addEvent( makeInterval([&event_loop, &counter](){
        Serial.print("interval "),
        Serial.println(counter);
        
        event_loop.addEvent(makeTimer([counter, &event_loop](){
            Serial.print("timeout after interval with counter - ");
            Serial.println(counter);
            if (counter >= 5) event_loop.stop();
        }, 1500, tiker)); // 1,5 секунды

        counter++;

    }, 3000, tiker) ); // интервал в 3 секунды
    
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

## Методы и юз кейсы

### EventLoop

EventLoop можно останавливать вызвав метод `stop()`, готовые события далее не будут обрабатываться 

метод `exec()` начинает обрабатывать события

`event_id_t addEvent(event_t event)` - добавляет событие и возвращает его айди

`void removeEvent(event_id_t id)` - удаляет событие при его наличии

### Timer Interval

Все параметры задаются при создании

Параметры:
    `(callback_t callback, uint32_t interval, tiker_t time_cheker)`

`callback_t` - функция, функтор и лямбда не принимающая и не возвращающая значений
```
void function_name(void) {
    ...
}
```

`tiker_t` - функция, функтор и лямбда не принимающая значений и возвращающая uint32_t

### Хочу синтаксис как на JavaScript

```
EventLoop event_loop;

void setInterval(callback_t callback, uint32_t time) {
    event_loop.addEvent(makeInterval(callback, time, millis));
}

void setTimeout(callback_t callback, uint32_t time) {
    event_loop.addEvent(makeTimer(callback, time, millis));
}
```

??? \
Profit!!!

используется также как и в js

```
void setup() {
    Serial.begin(115200);

    // создаем события интервала
    
    int counter = 0;

    // добавляем события
    setInterval([&counter](){
        Serial.print("interval "),
        Serial.println(counter);
        
        setTimeout([counter](){
            Serial.print("timeout after interval with counter - ");
            Serial.println(counter);
            if (counter >= 5) event_loop.stop();
        }, 1500); // 1,5 секунды

        counter++;

    }, 3000); // интервал в 3 секунды
    
    event_loop.exec(); // запускаем цикл событий
}

void loop() { }
```

## Event`s и расширение событий

```c++
class Event {
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
```

Цикл событий поддерживает любые события отнаследованные от класса `Event`. Это позволяет писать свои события а также делать из них функторы, циклы событий и так далее, ограничитель - ваше воображение и возможности вашей платформы

**Ньюансы:**

Как видно ниже нужно учитывать что перед срабатыванием метода run() вызвается stopTraking(), после чего проверяется необходимость удаления, и заново стратует отслеживание.

```c++
void exec() {
    runing = true;
    while (runing) {
        if (events.hasReadyEvent()) {
            auto event_pair = events.getReadyEvent();
            auto event = event_pair.second;
            event->stopTracking();
            event->run();
            if (event->needRemove()) {
                events.removeEvent(event_pair.first);
            } else {
                event->startTracking();
            }
        }
    }
}
```

## Ошибки и их решение

### Soft WDT reset 
Если пытаться быть корректным, то у esp есть процессы выполняющие работу в перерыве между вызовом функции `loop`, во время вызова `delay` или `yield`, тк цикл событий блокирует вызовы функции `loop` то esp думает что мы зависли из-за чего происходит перезагрузка и вывод ошибки.

**Решение:**

проблема при использовании esp, решается добавлением интервала:
```
void setup() {
    ...
    event_loop.addEvent(makeInterval(yield, 200, millis));
    ...
    event_loop.exec();
}
```

либо можно использовать неблокирующий цикл, но тогда некоторые методы могут работать не так как задумывалось и я не поощряю данный способ.
```
void setup() {
    ...
}

void loop() {
    event_loop.runNext();
}
```