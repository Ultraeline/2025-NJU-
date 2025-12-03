#include "map.h"
#include <easyx.h>

// 死亡界面绘制函数
void DrawGameOver() {
    // 绘制半透明黑色遮罩
    setfillcolor(RGB(0, 0, 0, 180));
    solidrectangle(0, 0, ScreenLen + ExtraWidth, ScreenLen);

    // 设置文字样式
    settextstyle(60, 0, "微软雅黑");
    settextcolor(RED);
    outtextxy(ScreenLen / 2 - 150, ScreenLen / 2 - 100, "游戏结束");

    settextstyle(30, 0, "微软雅黑");
    settextcolor(WHITE);
    TCHAR scoreText[50];
    sprintf_s(scoreText, _T("最终得分: %d"), score);
    outtextxy(ScreenLen / 2 - 100, ScreenLen / 2, scoreText);

    // 提示按键
    settextstyle(25, 0, "微软雅黑");
    outtextxy(ScreenLen / 2 - 150, ScreenLen / 2 + 60, "按R键重新开始");
    outtextxy(ScreenLen / 2 - 150, ScreenLen / 2 + 100, "按ESC键退出游戏");
}

// 死亡处理逻辑
void HandleGameOver() {
    while (true) {
        BeginBatchDraw();
        DrawGameOver();
        FlushBatchDraw();
        EndBatchDraw();

        // 检测按键
        if (GetAsyncKeyState('R') & 0x8000) {
            // 重置游戏状态
            isDead = false;
            running = true;
            mapindex = 0;
            score = 0;
            keynum = 0;
            // 重新初始化地图（需要调用创建地图的函数）
            break;
        }
        else if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            // 退出游戏
            exit(0);
        }
        Sleep(50);
    }
}