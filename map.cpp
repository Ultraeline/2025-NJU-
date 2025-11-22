#include<easyx.h>
#include<conio.h>
#include<windows.h>
#include<vector>
#include <time.h>
#include "map.h"


Maps::Point::Point(int x, int y, tpye PointType) :
	m_x(x), m_y(y), m_PointType(PointType) {
}


void Maps::DrawMap() //绘制地图
{
	for (int i = 0; i < MapPoint.size(); i++)
	{
		if (MapPoint[i].m_PointType == obstcle)//绘制墙
		{
			settextcolor(WHITE);
			outtextxy(MapPoint[i].m_x, MapPoint[i].m_y, _T("墙"));
		}
		else if (MapPoint[i].m_PointType == exit)//绘制出口
		{
			settextcolor(GREEN);
			outtextxy(MapPoint[i].m_x, MapPoint[i].m_y, _T("出"));
		}
	}
}



void CreateMap1(Maps& map) //创建map1
{
	for (int i = 0; i < Len; i++) {
		map.MapPoint.push_back(Maps::Point(CharLen * i, 0, Maps::tpye::obstcle));
		map.MapPoint.push_back(Maps::Point(CharLen * i, ScreenLen - CharLen, Maps::tpye::obstcle));
		map.MapPoint.push_back(Maps::Point(0, CharLen * i, Maps::tpye::obstcle));
		map.MapPoint.push_back(Maps::Point(ScreenLen - CharLen, CharLen * i, Maps::tpye::obstcle));
	}
	map.MapPoint.push_back(Maps::Point(CharLen * 8, CharLen * 9, Maps::tpye::exit));

}