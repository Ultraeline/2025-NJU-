#ifndef MAP_H
#define MAP_H

#include<easyx.h>
#include<conio.h>
#include<windows.h>
#include<vector>
#include <time.h>

constexpr int ScreenLen = 1000; //游戏界面长宽均为1000像素
constexpr int CharLen = 20; //字体长宽为20像素
constexpr int Len = ScreenLen / CharLen;

class Maps   // 创建地图
{
public:
	enum tpye   //用枚举定义地图上各点的属性，如障碍，宝箱，陷阱，出口等
	{
		obstcle,
		exit
	};


	class Point  //创建一个类，用于表示各点的属性
	{
	public:
		int m_x, m_y, m_PointType;

		Point(int x, int y, tpye PointType); 
	};

	std::vector<Point> MapPoint; //创建一个动态数组，记录各点的属性

	void DrawMap(); //绘制地图
	
};

void CreateMap1(Maps& map);//创建map1




#endif