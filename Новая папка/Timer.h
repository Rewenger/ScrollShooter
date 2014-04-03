#ifndef __TIMER_H__
#define __TIMER_H__

// интерфейс для удобного использования встроенного таймера фреймворка SDL
// задержка реализуется через функцию SDL_Delay(int ms)
class Timer {
    private:
    int startTicks,      // сколько тиков игрового таймера прошло с момента запуска данного таймера
        pausedTicks,     // сколько тиков игрового таймера прошло с момента паузы данного таймера
        FPS;             // количество "кадров в секунду" (frame per second) - число запусков функции OnTick в секунду
    bool paused, started;// текущее состояние таймера

    public:
    Timer(); // конструктор
    void start(); // запускает таймер
    void stop();  // останавливает таймер (не считает pausedTicks)
    void pause(); // паузит таймер (считает pausedTicks)
    void unpause(); // заканчивает паузу
    int get_ticks();// получает тики
    bool is_started();// функция, возвращающая started
    bool is_paused(); // аналогично с paused
    void (*OnTick);   // функция, запускающаяся с каждым тиком таймера
};

#endif