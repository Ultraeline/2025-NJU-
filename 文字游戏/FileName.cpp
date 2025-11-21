#include<easyx.h>
#include<conio.h>
#include<windows.h>
#include<vector>

constexpr int ScreenLen = 1000; //游戏界面长宽均为1000像素
constexpr int CharLen = 20; //字体长宽为20像素

class Maps   // 创建地图
{
public:
	enum tpye   //用枚举定义地图上各点的属性，如障碍，宝箱，陷阱等
	{
		obstcle

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
		settextcolor(WHITE);
		for (int i = 0; i < MapPoint.size(); i++)
		{
			if (MapPoint[i].m_PointType == obstcle)
			{
				outtextxy(MapPoint[i].m_x, MapPoint[i].m_y, _T("墙"));
			}

		}


	}
};

class Character //创建一个类，表示各个角色，如玩家，鬼等的坐标和外形以及移动方式等
{
public:
	int m_x, m_y;
	const char* m_appearance;

	virtual void move() {};
	virtual void draw() {};

	Character (int x, int y, const char* appearance)
		: m_x(x), m_y(y), m_appearance(appearance){}

};


class Player : public Character //创建一个Player类
{
public:
	COLORREF color = YELLOW; //颜色为黄色

	Player(int x, int y, const char* appearance) : Character(x, y, appearance) {} //用构造函数初始化玩家的坐标和外形

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
		if (GetAsyncKeyState('D') && RightMove)
			m_x += 20;
		if (GetAsyncKeyState('A') && LeftMove)
			m_x -= 20;
		if (GetAsyncKeyState('W') && UpMove)
			m_y -= 20;
		if (GetAsyncKeyState('S') && DownMove)
			m_y += 20;
	}

	void Draw() //绘制玩家
	{
		settextcolor(color);
		outtextxy(m_x, m_y, m_appearance);
	}

};

class Enemy : Character //创建敌人
{
public:
	const char* appearance = _T("鬼");
	COLORREF ColorSafe = RED;
	COLORREF ColorDanger = RED;

	enum behavior //敌人有两种状态 safe 和 danger， safe状态时随机运动， danger状态追踪玩家
	{
		Safe,
		Danger
	};
	behavior CurrentBehavior = Safe;
	Enemy(int x, int y, const char* appearance) : Character(x, y, appearance) {};

	void See() //当敌人看见玩家时，转化为追踪状态
	{



	}

	void Move()//敌人的移动逻辑，可能需要运用追踪算法
	{


	}

};



static void CreateMap1(Maps& map) //创建map1
{
	for (int i = 0; i < ScreenLen / CharLen; i++) {
		map.MapPoint.push_back(Maps::Point(CharLen * i, 0, Maps::tpye::obstcle));
		map.MapPoint.push_back(Maps::Point(CharLen * i, ScreenLen - CharLen, Maps::tpye::obstcle));
		map.MapPoint.push_back(Maps::Point(0, CharLen * i, Maps::tpye::obstcle));
		map.MapPoint.push_back(Maps::Point(ScreenLen - CharLen, CharLen * i, Maps::tpye::obstcle));
	}

}





int main() {
	initgraph(ScreenLen, ScreenLen); //创建界面
	settextstyle(20, 0, "宋体"); 
	Player player(ScreenLen / 2, ScreenLen / 2, _T("我"));
	Maps map1;
	CreateMap1(map1);
	cleardevice();
	while (true)
	{
		BeginBatchDraw(); 
		cleardevice();
		player.Draw();
		map1.DrawMap();
		player.Move(map1);
		Sleep(20);
		FlushBatchDraw();
	}
	EndBatchDraw();
	return 0;

}

/*static void set() {
	settextstyle(20, 0, "宋体");
	Player player(ScreenLen / 2, ScreenLen / 2, _T("我"));
	Maps maps[5];

}
*/  // 初始化游戏，暂时还没做好



