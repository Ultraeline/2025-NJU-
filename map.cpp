#include<iostream>
#include<easyx.h>
#include<conio.h>
#include<windows.h>
#include<vector>
#include <time.h>
#include "map.h"
#include<memory>

int mapindex = 0;

Maps::Point::Point(int x, int y) :
	m_x(x), m_y(y) {}

Maps::Obstcle::Obstcle(int x, int y) : Point(x, y) //构造函数初始化Obstacle
{
	m_pointtype = obstcle;
	m_appearance = "墙";  
	m_color = WHITE;
}

Maps::Exit::Exit(int x, int y) : Point(x, y)//构造函数初始化Exit
{
	m_pointtype = exit;
	m_appearance = "出";  
	m_color = GREEN;
}


Maps::Character::Character(int x, int y) : Point(x, y) {}

Maps::Player::Player(int x, int y) : Character(x, y)//构造函数初始化Player
{
	m_pointtype = player;
	m_appearance = "我";  
	m_color = YELLOW;
}


Maps::Enemy::Enemy(int x, int y) : Character(x, y)//构造函数初始化Enemy
{
	m_pointtype = enemy;
	m_appearance = "鬼";  
	m_color = RED;
	m_moveCount = rand() % 8;
	m_currentDirection = rand() % 4;
}


void Maps::DrawMap() //绘制地图
{
	for (int i = 0; i < MapPoint.size(); i++)
	{
		settextcolor(MapPoint[i]->m_color);
		outtextxy(MapPoint[i]->m_x, MapPoint[i]->m_y, MapPoint[i]->m_appearance);
	}
}

std::unique_ptr<Maps::Point> Maps::Obstcle::clone() const//克隆函数，用于将类存入动态数组中
{
	return std::make_unique<Obstcle>(*this);
}

std::unique_ptr<Maps::Point> Maps::Exit::clone() const//克隆函数，用于将类存入动态数组中
{
	return std::make_unique<Exit>(*this);
}

void Maps::Character::IsAgainstObstcle(Maps& map)//用于判断是否靠着墙壁
{
	for (int i = 0; i < map.MapPoint.size(); i++)
	{
		if (m_x + 20 == map.MapPoint[i] -> m_x && m_y == map.MapPoint[i] -> m_y && map.MapPoint[i] -> GetType() == map.obstcle) RightMove = false;
		if (m_x - 20 == map.MapPoint[i] -> m_x && m_y == map.MapPoint[i] -> m_y && map.MapPoint[i] -> GetType() == map.obstcle) LeftMove = false;
		if (m_y - 20 == map.MapPoint[i] -> m_y && m_x == map.MapPoint[i] -> m_x && map.MapPoint[i] -> GetType() == map.obstcle) UpMove = false;
		if (m_y + 20 == map.MapPoint[i] -> m_y && m_x == map.MapPoint[i] -> m_x && map.MapPoint[i] -> GetType() == map.obstcle) DownMove = false;
	}
}

std::unique_ptr<Maps::Point> Maps::Player::clone() const //克隆函数，用于将类存入动态数组中
{
	return std::make_unique<Player>(*this);
}

void Maps::Player::Interact(Maps& map) //与地图的交互，如逃出出口或碰到鬼
{
	bool TouchEnemy = false;
	bool TouchExit = false;

	for (int i = 0; i < map.MapPoint.size() && !TouchEnemy && !TouchExit; i++)
	{
		if (m_x == map.MapPoint[i]->m_x && m_y == map.MapPoint[i]->m_y && map.MapPoint[i]->GetType() == map.obstcle)
			TouchEnemy = true;
		if (m_x == map.MapPoint[i]->m_x && m_y == map.MapPoint[i]->m_y && map.MapPoint[i]->GetType() == map.exit)
			TouchExit = true;
	}

	if (TouchExit && mapindex < MapNum)
		mapindex++;

}

void Maps::Player::Move(Maps& map) //移动函数
{
	RightMove = true;
	LeftMove = true;
	UpMove = true;
	DownMove = true;
	IsAgainstObstcle(map);

	if ((GetAsyncKeyState('D')& 0x8000) && RightMove) //wasd控制移动
		m_x += 20;
	if ((GetAsyncKeyState('A') & 0x8000) && LeftMove)
		m_x -= 20;
	if ((GetAsyncKeyState('W') & 0x8000) && UpMove)
		m_y -= 20;
	if ((GetAsyncKeyState('S') & 0x8000) && DownMove)
		m_y += 20;

	Interact(map);//与地图的交互
}

std::unique_ptr<Maps::Point> Maps::Enemy::clone() const //克隆函数，用于将类存入动态数组中
{
	return std::make_unique<Enemy>(*this);
}

void Maps::Enemy::See() //当敌人看见玩家时，转化为追踪状态
{

}



void Maps::Enemy::Move(Maps& map)//敌人的移动逻辑，可能需要运用追踪算法
{
	enum Direction
	{
		Right = 0, Left, Up, Down
	};

	Direction dirs[4] = { Right, Left, Up , Down };

	RightMove = true;
	LeftMove = true;
	UpMove = true;
	DownMove = true;

	IsAgainstObstcle(map);

	if (CurrentBehavior == Safe && m_moveCount % 4 == 0) //使鬼随机移动，且降低移动速度，并且尽量降低移动方向的随机性，使其移动不会过于杂乱
	{
		if (rand() % 4 == 0 ||
			(m_currentDirection == Right && !RightMove) ||
			(m_currentDirection == Left && !LeftMove) ||
			(m_currentDirection == Up && !UpMove) ||
			(m_currentDirection == Down && !DownMove))
		{
			// 遇到障碍或随机改变方向
			m_currentDirection = rand() % 4;
		}

		Direction dir = dirs[m_currentDirection];

		if (dir == Right && RightMove)
			m_x += CharLen;
		else if (dir == Left && LeftMove)
			m_x -= CharLen;
		else if (dir == Up && UpMove)
			m_y -= CharLen;
		else if (dir == Down && DownMove)
			m_y += CharLen;

		if (m_moveCount > 1000) m_moveCount = 0;
	}

	m_moveCount++;
}




void Maps::MoveAll(Maps& map)//统一玩家和敌人的移动
{
	for (int i = 0; i < MapPoint.size(); i++)
	{
		PointType type = MapPoint[i]->GetType();
		if (type == player){ 
			MapPoint[i]->Move(map); 
		}
		else if (type == enemy) {
			MapPoint[i]->Move(map);
		}
	}
}