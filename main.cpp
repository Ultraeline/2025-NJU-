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

		Point(int x, int y, tpye PointType) :
			m_x(x), m_y(y), m_PointType(PointType) {
		}
	};

	std::vector<Point> MapPoint; //创建一个动态数组，记录各点的属性

	void DrawMap() //绘制地图
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
};

static void CreateMap1(Maps& map) //创建map1
{
	for (int i = 0; i < Len; i++) {
		map.MapPoint.push_back(Maps::Point(CharLen * i, 0, Maps::tpye::obstcle));
		map.MapPoint.push_back(Maps::Point(CharLen * i, ScreenLen - CharLen, Maps::tpye::obstcle));
		map.MapPoint.push_back(Maps::Point(0, CharLen * i, Maps::tpye::obstcle));
		map.MapPoint.push_back(Maps::Point(ScreenLen - CharLen, CharLen * i, Maps::tpye::obstcle));
	}
	map.MapPoint.push_back(Maps::Point(CharLen * 8, CharLen * 9, Maps::tpye::exit));

}

class Character //创建一个类，表示各个角色，如玩家，鬼等的坐标和外形以及移动方式等
{
public:
	COLORREF m_color;
	int m_x, m_y;
	const char* m_appearance;

	virtual void move() {};
	virtual void draw() {};

	Character (int x, int y, const char* appearance, COLORREF color )
		: m_x(x), m_y(y), m_appearance(appearance), m_color(color) {}

	void Draw() const//绘制角色     (编译器提示将成员函数设为常量，已保证不修改变量，所以我就加了)
	{
		settextcolor(m_color);
		outtextxy(m_x, m_y, m_appearance);
	}
};


class Player : public Character //创建一个Player类
{
public:
	Player(int x, int y, const char* appearance = _T("人"), COLORREF color = YELLOW) : Character(x, y, appearance, color) {} //用构造函数初始化玩家的坐标和外形

	void Move(Maps& map) //移动函数
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
	Enemy() : Character(CharLen * (rand()%Len), CharLen* (rand() % Len), _T("鬼"), RED) {} //构造函数初始化敌人
	Enemy(int x, int y, const char* appearance = _T("鬼"), COLORREF color = RED) : Character(x, y, appearance, color) {}

	void See() //当敌人看见玩家时，转化为追踪状态
	{



	}

	void Move(Maps& map)//敌人的移动逻辑，可能需要运用追踪算法
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

};


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



