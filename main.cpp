#include<easyx.h>
#include<conio.h>
#include<windows.h>
#include<vector>
#include <time.h>
#include "map.h"
#include "character.h"



int main() {
	srand((unsigned) time(NULL));
	initgraph(ScreenLen, ScreenLen); //创建界面
	settextstyle(20, 0, "宋体"); 
	Player player(ScreenLen / 2, ScreenLen / 2);
	Maps* map = new Maps[5];
	Enemy* enemy = new Enemy[5];

	CreateMap1(map[0]);

	Maps* CurrentMap = map;

	cleardevice();
	while (true)
	{
		BeginBatchDraw(); 
		cleardevice();
		player.Draw();
		for (int i = 0; i < 5; i++)
		{
			enemy[i].Draw();
			enemy[i].Move(*CurrentMap);
		}
		CurrentMap -> DrawMap();
		player.Move(*CurrentMap);
		Sleep(20);
		FlushBatchDraw();
	}
	EndBatchDraw();

	delete[]enemy;
	delete[]map;

	return 0;

}

/*static void set() {
	settextstyle(20, 0, "宋体");
	Player player(ScreenLen / 2, ScreenLen / 2, _T("我"));
	Maps maps[5];

}
*/  // 初始化游戏，暂时还没做好



