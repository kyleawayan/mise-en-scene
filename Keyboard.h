#pragma once
#ifndef KEYBOARD
#define KEYBOARD

// TO DO: Find a way to simulate key presses cross platform
#include <Windows.h>
#include <future>
#include <chrono>
#include <thread>

struct ThrottledHitKey
{
    INPUT kb;
    int key;
    int delay;
    int timer;
    bool timerRunning;

    ThrottledHitKey(int key, int delay)
    {
        kb.type = INPUT_KEYBOARD;
        timerRunning = false;
        this->key = key;
        this->delay = delay;
        timer = delay;
    }

    void startTimer()
    {
        this->timerRunning = true;
        for (int i = 0; i < delay; i++)
        {
            timer++;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        this->timerRunning = false;
    }

    // to do: learn how to use callbacks
    bool throttle()
    {
        if (timer == delay)
        {
            timer = 0;
            return true;
        }
        else if (!timerRunning)
        {
            std::async([this] {this->startTimer(); });
        }
        return false;
    }
};

void hitKey(int key)
{
    // https://www.cplusplus.com/forum/general/174557/
    INPUT kb;
    kb.type = INPUT_KEYBOARD;

    // Press the key
    kb.ki.wVk = key;
    kb.ki.dwFlags = 0;
    SendInput(1, &kb, sizeof(INPUT));

    // Release the key
    kb.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &kb, sizeof(INPUT));
}

#endif