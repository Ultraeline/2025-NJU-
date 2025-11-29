#include<easyx.h>
#include"map.h"
#include"messagescreen.h"

void DrawMessage()
{	
	settextcolor(WHITE);
	TCHAR mapmessage[10];
	sprintf_s(mapmessage, _T("地图: %d"), mapindex + 1);
	outtextxy(ScreenLen + 3 * CharLen, 2 * CharLen, mapmessage);
	TCHAR scoremessage[10];
	sprintf_s(scoremessage, _T("分数: %d"), score);
	outtextxy(ScreenLen + 3 * CharLen, 4 * CharLen, scoremessage);
	TCHAR keymessage[20];
	sprintf_s(keymessage, _T("钥匙数量: %d"), keynum);
	outtextxy(ScreenLen + 3 * CharLen, 6 * CharLen, keymessage);

}