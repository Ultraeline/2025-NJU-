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

AudioManager audioManager;
int main() {
	srand((unsigned) time(NULL));
	initgraph(ScreenLen + ExtraWidth, ScreenLen); //创建界面
	settextstyle(20, 0, "宋体"); 
	Maps* map = new Maps[5];
	for (int i = 0; i < MapNum; i++)
	{
		CreateMap(map[i], i);
	}
	audioManager.PlayBGM("normal.mp3");
	cleardevice();

	while (running)
	{
		BeginBatchDraw();
		cleardevice();
		map[mapindex].MoveAll(map[mapindex]);
		map[mapindex].DrawMap();
		DrawMessage();
		FlushBatchDraw();
		Sleep(40);
	}

	EndBatchDraw();

	delete[]map;

	return 0;

}





/*static void set() {
	settextstyle(20, 0, "宋体");
	Player player(ScreenLen / 2, ScreenLen / 2, _T("我"));
	Maps maps[5];

}
*/  // 初始化游戏，暂时还没做好



