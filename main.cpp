#include<iostream>
#include<easyx.h>
#include<conio.h>
#include<windows.h>
#include<vector>
#include <time.h>
#include "map.h"
#include "createmap.h"
#include "messagescreen.h"
#include<memory>
#include "BGM.h"
#include "gameover.h"
Maps* map;
AudioManager audioManager;
int main() {
	srand((unsigned)time(NULL));
	initgraph(ScreenLen + ExtraWidth, ScreenLen); //创建界面
	settextstyle(CharLen, 0, "宋体");
	map = new Maps[5];
	for (int i = 0; i < MapNum; i++)
	{
		CreateMap(map[i], i);
	}
	audioManager.PlayBGM("normal.mp3");
	cleardevice();

	while (running) {
		settextstyle(CharLen, 0, "宋体");
		BeginBatchDraw();
		cleardevice(); // 清空画布

		if (isDead) {
			// 死亡时进入结算逻辑，结算完自动重置isDead/running
			HandleGameOver();
		}
		else {
			// 正常游戏逻辑：移动元素 + 绘制地图 + 绘制信息
			map[mapindex].MoveAll(map[mapindex]);
			map[mapindex].DrawMap();
			DrawMessage(); // 绘制分数、钥匙等
		}

		FlushBatchDraw();
		EndBatchDraw();
		Sleep(40); // 控制帧率（可调整，数值越小越流畅）
	}

	// 清理资源
	delete[] map;
	closegraph();  // 确保关闭图形界面
	return 0;
}





/*static void set() {
	settextstyle(20, 0, "宋体");
	Player player(ScreenLen / 2, ScreenLen / 2, _T("我"));
	Maps maps[5];

}
*/  // 初始化游戏，暂时还没做好



