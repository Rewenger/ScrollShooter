#ifndef __TIMER_H__
#define __TIMER_H__

// ��������� ��� �������� ������������� ����������� ������� ���������� SDL
// �������� ����������� ����� ������� SDL_Delay(int ms)
class Timer {
    private:
    int startTicks,      // ������� ����� �������� ������� ������ � ������� ������� ������� �������
        pausedTicks,     // ������� ����� �������� ������� ������ � ������� ����� ������� �������
        FPS;             // ���������� "������ � �������" (frame per second) - ����� �������� ������� OnTick � �������
    bool paused, started;// ������� ��������� �������

    public:
    Timer(); // �����������
    void start(); // ��������� ������
    void stop();  // ������������� ������ (�� ������� pausedTicks)
    void pause(); // ������ ������ (������� pausedTicks)
    void unpause(); // ����������� �����
    int get_ticks();// �������� ����
    bool is_started();// �������, ������������ started
    bool is_paused(); // ���������� � paused
    void (*OnTick);   // �������, ������������� � ������ ����� �������
};

#endif