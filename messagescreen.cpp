#include<easyx.h>
#include"map.h"
#include"messagescreen.h"

void DrawMessage()
{	
	settextcolor(WHITE);
	TCHAR mapmessage[10];
	sprintf_s(mapmessage, _T("µØÍ¼: %d"), mapindex + 1);
	outtextxy(ScreenLen + 3 * CharLen, 2 * CharLen, mapmessage);
	TCHAR scoremessage[10];
	sprintf_s(scoremessage, _T("·ÖÊý: %d"), score);
	outtextxy(ScreenLen + 3 * CharLen, 4 * CharLen, scoremessage);
}