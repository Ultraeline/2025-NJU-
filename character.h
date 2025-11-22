#ifndef CHARACTER_H
#define CHARATER_H

#include<easyx.h>
#include<conio.h>
#include<windows.h>
#include<vector>
#include <time.h>
#include "map.h"



class Character //创建一个类，表示各个角色，如玩家，鬼等的坐标和外形以及移动方式等
{
public:
	COLORREF m_color;
	int m_x, m_y;
	const char* m_appearance;
	bool RightMove = true;
	bool LeftMove = true;
	bool UpMove = true;
	bool DownMove = true;

	virtual void Move();

	Character(int x, int y, const char* appearance, COLORREF color);
		

	void Draw() const;//绘制角色     (编译器提示将成员函数设为常量，已保证不修改变量，所以我就加了)
	void IsAgainstObstcle(Maps& map); //用于判断是否靠着墙壁
	
};


class Player : public Character //创建一个Player类
{
public:
	Player(int x, int y, const char* appearance = _T("我"), COLORREF color = YELLOW); //用构造函数初始化玩家的坐标和外形

	void Move(Maps& map); //移动函数
	void Interact(Maps*& map);//判断与地图的交互
	
};

class Enemy : public Character //创建敌人
{
public:
	const COLORREF ColorSafe = RED;
	const COLORREF ColorDanger = RED;

	enum behavior //敌人有两种状态 safe 和 danger， safe状态时随机运动， danger状态追踪玩家
	{
		Safe,
		Danger
	};

	behavior CurrentBehavior = Safe;

	Enemy(); //构造函数初始化敌人
	Enemy(int x, int y, const char* appearance = _T("鬼"), COLORREF color = RED);

	void See(); //当敌人看见玩家时，转化为追踪状态
	void Move(Maps& map);//敌人的移动逻辑，可能需要运用追踪算法

};

#endif
