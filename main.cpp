#include<iostream>
#include<easyx.h>
#include<conio.h>
#include<windows.h>
#include<vector>
#include <time.h>
#include "map.h"
#include<memory>


int main() {
	srand((unsigned) time(NULL));
	initgraph(ScreenLen, ScreenLen); //创建界面
	settextstyle(20, 0, "宋体"); 
	Maps* map = new Maps[5];

	CreateMap1(map[0]);
	Maps* CurrentMap = &map[0];
	
	cleardevice();

	while (true)
	{
		//std::cout << "test";
		BeginBatchDraw(); 
		cleardevice();
		CurrentMap->MoveAll(*CurrentMap);
		CurrentMap->DrawMap();
		FlushBatchDraw();
		Sleep(20);
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



