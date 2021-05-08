#pragma once
#ifndef KEYBOARD
#define KEYBOARD

// TO DO: Find a way to simulate key presses cross platform
#include <Windows.h>

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