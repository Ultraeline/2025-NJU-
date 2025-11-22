#include<easyx.h>
#include<conio.h>
#include<windows.h>
#include<vector>
#include <time.h>
#include "map.h"
#include "character.h"



Character::Character(int x, int y, const char* appearance, COLORREF color)
	: m_x(x), m_y(y), m_appearance(appearance), m_color(color) {
}

void Character::Move() {};


void Character::Draw() const//绘制角色     (编译器提示将成员函数设为常量，已保证不修改变量，所以我就加了)
{
	settextcolor(m_color);
	outtextxy(m_x, m_y, m_appearance);
}


Player::Player(int x, int y, const char* appearance, COLORREF color) : Character(x, y, appearance, color) {} //用构造函数初始化玩家的坐标和外形


void Player::Move(Maps& map) //移动函数
{
	bool RightMove = true;
	bool LeftMove = true;
	bool UpMove = true;
	bool DownMove = true;

	for (int i = 0; i < 200; i++)  //用于判断玩家是否靠着墙壁
	{
		if (m_x + 20 == map.MapPoint[i].m_x && m_y == map.MapPoint[i].m_y && map.MapPoint[i].m_PointType == map.obstcle) RightMove = false;
		if (m_x - 20 == map.MapPoint[i].m_x && m_y == map.MapPoint[i].m_y && map.MapPoint[i].m_PointType == map.obstcle) LeftMove = false;
		if (m_y - 20 == map.MapPoint[i].m_y && m_x == map.MapPoint[i].m_x && map.MapPoint[i].m_PointType == map.obstcle) UpMove = false;
		if (m_y + 20 == map.MapPoint[i].m_y && m_x == map.MapPoint[i].m_x && map.MapPoint[i].m_PointType == map.obstcle) DownMove = false;
	}
	if (GetAsyncKeyState('D') && RightMove) //wasd控制移动
		m_x += 20;
	if (GetAsyncKeyState('A') && LeftMove)
		m_x -= 20;
	if (GetAsyncKeyState('W') && UpMove)
		m_y -= 20;
	if (GetAsyncKeyState('S') && DownMove)
		m_y += 20;
}


Enemy::Enemy() : Character(CharLen* (rand() % Len), CharLen* (rand() % Len), _T("鬼"), RED) {}
Enemy::Enemy(int x, int y, const char* appearance, COLORREF color) : Character(x, y, appearance, color) {}

void Enemy::See() //当敌人看见玩家时，转化为追踪状态
{

}

void Enemy::Move(Maps& map)//敌人的移动逻辑，可能需要运用追踪算法
{
	enum Dirction
	{
		Right = 0, Left, Up, Down
	};

	Dirction dirs[4] = { Right, Left, Up , Down };
	static Dirction dir = dirs[rand() % 4];

	bool RightMove = true;
	bool LeftMove = true;
	bool UpMove = true;
	bool DownMove = true;
	static int count = 0;

	for (int i = 0; i < 200; i++)  //用于判断鬼是否靠着墙壁
	{
		if (m_x + 20 == map.MapPoint[i].m_x && m_y == map.MapPoint[i].m_y && map.MapPoint[i].m_PointType == map.obstcle) RightMove = false;
		if (m_x - 20 == map.MapPoint[i].m_x && m_y == map.MapPoint[i].m_y && map.MapPoint[i].m_PointType == map.obstcle) LeftMove = false;
		if (m_y - 20 == map.MapPoint[i].m_y && m_x == map.MapPoint[i].m_x && map.MapPoint[i].m_PointType == map.obstcle) UpMove = false;
		if (m_y + 20 == map.MapPoint[i].m_y && m_x == map.MapPoint[i].m_x && map.MapPoint[i].m_PointType == map.obstcle) DownMove = false;

	}
	if (CurrentBehavior == Safe && count % 4 == 3) //使鬼随机移动，且降低移动速度，并且尽量降低移动方向的随机性，使其移动不会过于杂乱
	{
		if (count % 8 == 3)
		{
			dir = dirs[rand() % 4];
			count = 0;
		}

		if (dir == Right && RightMove)
			m_x += 20;
		if (dir == Left && LeftMove)
			m_x -= 20;
		if (dir == Up && UpMove)
			m_y -= 20;
		if (dir == Down && DownMove)
			m_y += 20;
	}

	count++;
}
