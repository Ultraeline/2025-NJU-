#include "cleargame.h"
#include "map.h"
#include "BGM.h"
#include <easyx.h>
#include <string>
#include <sstream>
#include "createmap.h"

// 声明仓库中已有的全局变量
extern bool isDead;
extern bool running;
extern int mapindex;
extern int score;
extern int keynum;
extern Maps* map;
extern AudioManager audioManager;

// 绘制死亡结算界面
void DrawPass() {
    // 半透明黑色背景
    setfillcolor(RGB(0, 0, 180));
    solidrectangle(0, 0, ScreenLen + ExtraWidth, ScreenLen);
    setbkmode(TRANSPARENT);

    // 绘制标题
    settextstyle(60, 0, "微软雅黑");
    settextcolor(RED);
    outtextxy(ScreenLen / 2 - 150, ScreenLen / 2 - 100, "恭喜通关！");

    // 显示分数
    settextstyle(30, 0, "微软雅黑");
    settextcolor(WHITE);
    TCHAR scoreText[50];
    _stprintf_s(scoreText, _T("分数: %d"), score);
    outtextxy(ScreenLen / 2 - 100, ScreenLen / 2, scoreText);


    // 显示操作提示
    settextstyle(25, 0, "微软雅黑");
    outtextxy(ScreenLen / 2 - 150, ScreenLen / 2 + 90, "按R键重玩");
    outtextxy(ScreenLen / 2 - 150, ScreenLen / 2 + 130, "按ESC键退出游戏");
}

// 处理死亡结算逻辑（当前关卡重来）
void HandlePassGame() {
    // 停止BGM（适配仓库的audioManager）
    audioManager.StopBGM();
    DrawPass();
    // 记录死亡时的当前关卡（保留mapindex，不重置为0）
    // 检测R键（重玩当前关，加防抖）
    if (GetAsyncKeyState('R') & 0x8000)
    {
        // 重置游戏状态（适配仓库变量）
        mapindex = 0;
        isDead = false;
        isPass = false;
        score = 0; // 重置分数
        keynum = 0; // 重置当前关钥匙

        for (int i = 0; i < MapNum; i++) {
            map[i].MapPoint.clear();
            CreateMap(map[i], i);
        }
        audioManager.PlayBGM("normal.mp3");
    }


    // 检测ESC键（退出游戏）
    else if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
    {
        closegraph();
        exit(0);
    }
}