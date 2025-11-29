#include<iostream>
#include<easyx.h>
#include<conio.h>
#include<windows.h>
#include<vector>
#include <time.h>
#include "map.h"
#include<memory>

int mapindex = 0;
int score = 0;
int keynum = 0;
bool running = true;
int tempcount = 0;
bool lastW = false, lastA = false, lastS = false, lastD = false;
char lastPressedKey = 0;

Maps::Point::Point(int x, int y) :
	m_x(x), m_y(y) {}

Maps::Obstacle::Obstacle(int x, int y) : Point(x, y) //构造函数初始化Obstacle
{
	m_pointtype = obstacle;
	m_appearance = "墙";  
	m_color = WHITE;
}

Maps::Exit::Exit(int x, int y) : Point(x, y)//构造函数初始化Exit
{
	m_pointtype = exit;
	m_appearance = "出";  
	m_color = GREEN;
}

Maps::Coin::Coin(int x, int y) : Point(x, y)
{
	m_pointtype = coin;
	m_appearance = "币";
	m_color = YELLOW;
}

Maps::Key::Key(int x, int y) : Point(x, y)
{
	m_pointtype = key;
	m_appearance = "钥";
	m_color = YELLOW;
}

Maps::Door::Door(int x, int y) : Point(x, y)
{
	m_pointtype = door;
	m_appearance = "门";
	m_color = BROWN;
}



Maps::Character::Character(int x, int y) : Point(x, y) {}

Maps::Player::Player(int x, int y) : Character(x, y)//构造函数初始化Player
{
	m_pointtype = player;
	m_appearance = "我";  
	m_color = RGB(0, 255, 255);
}


Maps::Enemy::Enemy(int x, int y) : Character(x, y)//构造函数初始化Enemy
{
	m_pointtype = enemy;
	m_appearance = "鬼";  
	m_color = RGB(255,189,189);
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

std::unique_ptr<Maps::Point> Maps::Obstacle::clone() const//克隆函数，用于将类存入动态数组中
{
	return std::make_unique<Obstacle>(*this);
}

std::unique_ptr<Maps::Point> Maps::Exit::clone() const//克隆函数，用于将类存入动态数组中
{
	return std::make_unique<Exit>(*this);
}

std::unique_ptr<Maps::Point> Maps::Coin::clone() const//克隆函数，用于将类存入动态数组中
{
	return std::make_unique<Coin>(*this);
}

std::unique_ptr<Maps::Point> Maps::Key::clone() const//克隆函数，用于将类存入动态数组中
{
	return std::make_unique<Key>(*this);
}

std::unique_ptr<Maps::Point> Maps::Door::clone() const//克隆函数，用于将类存入动态数组中
{
	return std::make_unique<Door>(*this);
}

void Maps::Player::IsAgainstObstcle(Maps& map)//用于判断是否靠着墙壁
{
	for (int i = 0; i < map.MapPoint.size(); i++)
	{
		if (m_x + 20 == map.MapPoint[i] -> m_x && m_y == map.MapPoint[i] -> m_y && (map.MapPoint[i]->GetType() == map.obstacle || (map.MapPoint[i]->GetType() == map.door && keynum <= 0))) RightMove = false;
		if (m_x - 20 == map.MapPoint[i] -> m_x && m_y == map.MapPoint[i] -> m_y && (map.MapPoint[i]->GetType() == map.obstacle || (map.MapPoint[i]->GetType() == map.door && keynum <= 0))) LeftMove = false;
		if (m_y - 20 == map.MapPoint[i] -> m_y && m_x == map.MapPoint[i] -> m_x && (map.MapPoint[i]->GetType() == map.obstacle || (map.MapPoint[i]->GetType() == map.door && keynum <= 0))) UpMove = false;
		if (m_y + 20 == map.MapPoint[i] -> m_y && m_x == map.MapPoint[i] -> m_x && (map.MapPoint[i]->GetType() == map.obstacle || (map.MapPoint[i]->GetType() == map.door && keynum <= 0))) DownMove = false;
	}
}

void Maps::Enemy::IsAgainstObstcle(Maps& map)//用于判断是否靠着墙壁
{
	for (int i = 0; i < map.MapPoint.size(); i++)
	{
		if (m_x + 20 == map.MapPoint[i]->m_x && m_y == map.MapPoint[i]->m_y && (map.MapPoint[i]->GetType() == map.obstacle || map.MapPoint[i]->GetType() == map.door)) RightMove = false;
		if (m_x - 20 == map.MapPoint[i]->m_x && m_y == map.MapPoint[i]->m_y && (map.MapPoint[i]->GetType() == map.obstacle || map.MapPoint[i]->GetType() == map.door)) LeftMove = false;
		if (m_y - 20 == map.MapPoint[i]->m_y && m_x == map.MapPoint[i]->m_x && (map.MapPoint[i]->GetType() == map.obstacle || map.MapPoint[i]->GetType() == map.door)) UpMove = false;
		if (m_y + 20 == map.MapPoint[i]->m_y && m_x == map.MapPoint[i]->m_x && (map.MapPoint[i]->GetType() == map.obstacle || map.MapPoint[i]->GetType() == map.door)) DownMove = false;
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
	bool TouchCoin = false;
	bool TouchKey = false;
	bool TouchDoor = false;
	for (int i = 0; i < map.MapPoint.size() && !TouchEnemy && !TouchExit && !TouchCoin && !TouchKey && !TouchDoor; i++)
	{
		if (m_x == map.MapPoint[i]->m_x && m_y == map.MapPoint[i]->m_y && map.MapPoint[i]->GetType() == map.enemy)
			TouchEnemy = true;
		else if (m_x == map.MapPoint[i]->m_x && m_y == map.MapPoint[i]->m_y && map.MapPoint[i]->GetType() == map.exit)
			TouchExit = true;
		else if (m_x == map.MapPoint[i]->m_x && m_y == map.MapPoint[i]->m_y && map.MapPoint[i]->GetType() == map.coin)
			TouchCoin = true;
		else if (m_x == map.MapPoint[i]->m_x && m_y == map.MapPoint[i]->m_y && map.MapPoint[i]->GetType() == map.key)
			TouchKey = true;
		else if (m_x == map.MapPoint[i]->m_x && m_y == map.MapPoint[i]->m_y && map.MapPoint[i]->GetType() == map.door)
			TouchDoor = true;

		if (TouchExit && mapindex < MapNum - 1)
		{
			mapindex++;
			score += 10;
		}
		else if (TouchCoin)
		{
			map.MapPoint.erase(map.MapPoint.begin() + i);
			--i;
			score++;
		}
		else if (TouchKey)
		{
			map.MapPoint.erase(map.MapPoint.begin() + i);
			--i;
			keynum++;
		}
		else if (TouchDoor)
		{
			keynum--;
			map.MapPoint.erase(map.MapPoint.begin() + i);
			--i;
		}


	}



}

void Maps::Player::Move(Maps& map) //移动函数
{
	RightMove = true;
	LeftMove = true;
	UpMove = true;
	DownMove = true;
	
	IsAgainstObstcle(map);

	bool currentW = (GetAsyncKeyState('W') & 0x8000) != 0;
	bool currentA = (GetAsyncKeyState('A') & 0x8000) != 0;
	bool currentS = (GetAsyncKeyState('S') & 0x8000) != 0;
	bool currentD = (GetAsyncKeyState('D') & 0x8000) != 0;

	if (currentW && !lastW) {
		lastPressedKey = 'W';
	}
	else if (currentA && !lastA) {
		lastPressedKey = 'A';
	}
	else if (currentS && !lastS) {
		lastPressedKey = 'S';
	}
	else if (currentD && !lastD) {
		lastPressedKey = 'D';
	}
	else if (!currentW && !currentA && !currentS && !currentD) {
		lastPressedKey = 0;
	}
	else if(!((lastPressedKey == 'W' && currentW) || (lastPressedKey == 'A' && currentA) || (lastPressedKey == 'S' && currentS) || (lastPressedKey == 'D' && currentD)))
	{
		if (currentW)
			lastPressedKey = 'W';
		if (currentA)
			lastPressedKey = 'A';
		if (currentS)
			lastPressedKey = 'S';
		if (currentD)
			lastPressedKey = 'D';

	}



	lastW = currentW;
	lastA = currentA;
	lastS = currentS;
	lastD = currentD;
	switch (lastPressedKey) {
	case 'W': if (UpMove)  m_y -= CharLen ; break;
	case 'A': if (LeftMove) m_x -= CharLen; break;
	case 'S': if (DownMove) m_y += CharLen; break;
	case 'D': if (RightMove) m_x += CharLen; break;
	}
	Interact(map);//与地图的交互
}

std::unique_ptr<Maps::Point> Maps::Enemy::clone() const //克隆函数，用于将类存入动态数组中
{
	return std::make_unique<Enemy>(*this);
}


bool Maps::Enemy::See(Maps& map) {
	// 找玩家的位置
	int PlayerX = 0, PlayerY = 0;
	for (int i = 0; i < map.MapPoint.size(); i++) {
		if (map.MapPoint[i]->GetType() == map.player) {
			PlayerX = map.MapPoint[i]->m_x;
			PlayerY = map.MapPoint[i]->m_y;
			break;
		}
	}

	const int VIEW_RANGE = CharLen * 12;
	int distanceX = abs(m_x - PlayerX);
	int distanceY = abs(m_y - PlayerY);

	if (distanceX + distanceY > VIEW_RANGE) return false;//太远直接false

	// 计算视线方向
	int dx = (PlayerX > m_x) ? CharLen : (PlayerX < m_x) ? -CharLen : 0;
	int dy = (PlayerY > m_y) ? CharLen : (PlayerY < m_y) ? -CharLen : 0;

	// 沿视线方向检查是否有墙遮挡
	int checkX = m_x + dx;
	int checkY = m_y + dy;

	while (checkX != PlayerX || checkY != PlayerY) {
		// 检查当前位置是否有障碍物
		for (int i = 0; i < map.MapPoint.size(); i++) {
			if (map.MapPoint[i]->m_x == checkX &&
				map.MapPoint[i]->m_y == checkY &&
				(map.MapPoint[i]->GetType() == map.obstacle || map.MapPoint[i]->GetType() == map.door)) {
				return false;  // 被墙遮挡
			}
		}

		// 向玩家方向移动一格
		if (checkX != PlayerX) checkX += dx;
		if (checkY != PlayerY) checkY += dy;
	}

	return true;  // 可以看见玩家
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
	static int PlayerX = 0, PlayerY = 0;
	bool ChangeSafe = false;
	if (m_x == PlayerX && m_y == PlayerY || (m_moveCount - tempcount) % 4 == 0)
		ChangeSafe = true;
	IsAgainstObstcle(map);
	if (See(map))
	{
		tempcount = m_moveCount;
		bool ChangeSafe = false;
		CurrentBehavior = Danger;
		m_color = ColorDanger;
		for (int i = 0; i < map.MapPoint.size(); i++)
		{
			if (map.MapPoint[i]->GetType() == map.player)
			{
				PlayerX = map.MapPoint[i]->m_x;
				PlayerY = map.MapPoint[i]->m_y;
			}
		}
	}
	else if(!See(map) && ChangeSafe)
	{
		CurrentBehavior = Safe;
		m_color = ColorSafe;
	}

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
		
	}
	else if (CurrentBehavior == Danger)
	{
		
		if (m_moveCount % 2 == 0)
		{
			if (m_x - PlayerX >= 0 && m_y - PlayerY >= 0)
			{
				if (m_x - PlayerX > m_y - PlayerY && LeftMove)
				{
					m_x -= CharLen;
				}
				else if( UpMove )
				{
					m_y -= CharLen;
				}
			}
			else if (m_x - PlayerX >= 0 && m_y - PlayerY <= 0)
			{
				if (m_x - PlayerX > PlayerY - m_y && LeftMove)
				{
					m_x -= CharLen;
				}
				else if (DownMove)
				{
					m_y += CharLen;
				}
			}
			else if (m_x - PlayerX <= 0 && m_y - PlayerY >= 0)
			{
				if ( PlayerX - m_x> PlayerY - m_y && RightMove)
				{
					m_x += CharLen;
				}
				else if (UpMove)
				{
					m_y -= CharLen;
				}
			}
			else if (m_x - PlayerX <= 0 && m_y - PlayerY <= 0)
			{
				if ( PlayerX - m_x > PlayerY - m_y && RightMove)
				{
					m_x += CharLen;
				}
				else if (DownMove)
				{
					m_y += CharLen;
				}
			}
			else
			{
				ChangeSafe = true;
				CurrentBehavior = Safe;
			}
		}


	}
	if (m_moveCount > 1000) m_moveCount = 0;
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