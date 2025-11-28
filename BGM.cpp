#include "BGM.h"
#include <stdio.h>
AudioManager::AudioManager() {
}

// 退出时自动停止音乐
AudioManager::~AudioManager() {
    StopBGM();
}

// 播放背景音乐（循环）
void AudioManager::PlayBGM(const char* filename) {
    char cmd[128];

    
    StopBGM();

    // 打开并循环播放
    sprintf_s(cmd, "open \"%s\" alias bgm", filename);
    mciSendString(cmd, NULL, 0, NULL);
    mciSendString("play bgm repeat", NULL, 0, NULL);
}

// 停止背景音乐
void AudioManager::StopBGM() {
    mciSendString("stop bgm", NULL, 0, NULL);
    mciSendString("close bgm", NULL, 0, NULL);
}