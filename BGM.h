#pragma once
#include <windows.h>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

class AudioManager {
public:
    AudioManager();    
    ~AudioManager();    

    void PlayBGM(const char* fliename);  // ≤•∑≈±≥æ∞“Ù¿÷
    void StopBGM();                      // Õ£÷π±≥æ∞“Ù¿÷
};
