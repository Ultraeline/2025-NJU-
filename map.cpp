#include<iostream>
#include<easyx.h>
#include<conio.h>
#include<windows.h>
#include<vector>
#include <time.h>
#include "map.h"
#include<memory>

Maps::Point::Point(int x, int y) :
	m_x(x), m_y(y) {}

Maps::Obstcle::Obstcle(int x, int y) : Point(x, y)
{
	m_pointtype = obstcle;
	m_appearance = "墙";  // 先用英文字符测试
	m_color = WHITE;
}

Maps::Exit::Exit(int x, int y) : Point(x, y)
{
	m_pointtype = exit;
	m_appearance = "出";  // 先用英文字符测试
	m_color = GREEN;
}


Maps::Character::Character(int x, int y) : Point(x, y) {}

Maps::Player::Player(int x, int y) : Character(x, y)
{
	m_pointtype = player;
	m_appearance = "我";  // 先用英文字符测试
	m_color = YELLOW;
}


Maps::Enemy::Enemy(int x, int y) : Character(x, y)
{
	m_pointtype = enemy;
	m_appearance = "鬼";  // 先用英文字符测试
	m_color = RED;
}


void Maps::DrawMap() //绘制地图
{
	for (int i = 0; i < MapPoint.size(); i++)
	{
		settextcolor(MapPoint[i]->m_color);
		outtextxy(MapPoint[i]->m_x, MapPoint[i]->m_y, MapPoint[i]->m_appearance);
	}
}



void CreateMap1(Maps& map) //创建map1
{
	for (int i = 0; i < Len; i++) {
		map.MapPoint.push_back(std::make_unique<Maps::Obstcle>(CharLen * i, 0));
		map.MapPoint.push_back(std::make_unique<Maps::Obstcle>(CharLen * i, ScreenLen - CharLen));
		map.MapPoint.push_back(std::make_unique<Maps::Obstcle>(0, CharLen * i));
		map.MapPoint.push_back(std::make_unique<Maps::Obstcle>(ScreenLen - CharLen, CharLen * i));
	
	}
	map.MapPoint.push_back(std::make_unique<Maps::Exit>(CharLen, CharLen));
	map.MapPoint.push_back(std::make_unique<Maps::Player>(5 * CharLen, 5 * CharLen));
	map.MapPoint.push_back(std::make_unique<Maps::Enemy>(10 * CharLen, 10 * CharLen));
}


void Maps::Character::IsAgainstObstcle(Maps& map)//用于判断是否靠着墙壁W
{
	for (int i = 0; i < map.MapPoint.size(); i++)
	{
		if (m_x + 20 == map.MapPoint[i] -> m_x && m_y == map.MapPoint[i] -> m_y && map.MapPoint[i] -> GetType() == map.obstcle) RightMove = false;
		if (m_x - 20 == map.MapPoint[i] -> m_x && m_y == map.MapPoint[i] -> m_y && map.MapPoint[i] -> GetType() == map.obstcle) LeftMove = false;
		if (m_y - 20 == map.MapPoint[i] -> m_y && m_x == map.MapPoint[i] -> m_x && map.MapPoint[i] -> GetType() == map.obstcle) UpMove = false;
		if (m_y + 20 == map.MapPoint[i] -> m_y && m_x == map.MapPoint[i] -> m_x && map.MapPoint[i] -> GetType() == map.obstcle) DownMove = false;
	}


}

std::unique_ptr<Maps::Point> Maps::Player::clone() const
{
	return std::make_unique<Player>(*this);
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
}

std::unique_ptr<Maps::Point> Maps::Enemy::clone() const
{
	return std::make_unique<Enemy>(*this);
}

void Maps::Enemy::See() //当敌人看见玩家时，转化为追踪状态
{

}

void Maps::Enemy::Move(Maps& map)//敌人的移动逻辑，可能需要运用追踪算法
{
	enum Dirction
	{
		Right = 0, Left, Up, Down
	};

	Dirction dirs[4] = { Right, Left, Up , Down };
	static Dirction dir = dirs[rand() % 4];
	static int count = 0;
	RightMove = true;
	LeftMove = true;
	UpMove = true;
	DownMove = true;

	IsAgainstObstcle(map);

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


void Maps::Player::Interact(Maps*& map)
{
	bool TouchEnemy = false;
	bool TouchExit = false;

	for (int i = 0; i < map->MapPoint.size() && !TouchEnemy && !TouchExit; i++)
	{
		if (m_x == map->MapPoint[i] -> m_x && m_y == map -> MapPoint[i] -> m_y && map->MapPoint[i] -> GetType() == map->obstcle)
			TouchEnemy = true;
		if (m_x == map->MapPoint[i]->m_x && m_y == map->MapPoint[i]->m_y && map->MapPoint[i]->GetType() == map -> exit)
			TouchExit = true;
	}

	if (TouchExit)
		++(map);

}

void Maps::MoveAll(Maps& map)
{
	//std::cout << "test";
	for (int i = 0; i < MapPoint.size(); i++)
	{
		PointType type = MapPoint[i]->GetType();
		//printf("处理第 %d 个对象，类型: %d\n", i, MapPoint[i]->GetType());
		if (type == player){ 
			//printf("找到玩家对象，调用 Move\n");
			MapPoint[i]->Move(map); 
		}
		else if (type == enemy) {
			//std::cout << "test";
			MapPoint[i]->Move(map);
		}
	}
}