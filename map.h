#ifndef MAP_H
#define MAP_H

#include<iostream>
#include<easyx.h>
#include<conio.h>
#include<windows.h>
#include<vector>
#include <time.h>
#include<memory>

constexpr int ScreenLen = 1000; //游戏界面长宽均为1000像素
constexpr int CharLen = 20; //字体长宽为20像素
constexpr int Len = ScreenLen / CharLen;

class Maps   // 创建地图
{
public:
	
	enum PointType
	{
		point = 0,
		obstcle,
		exit,
		player,
		enemy
	};

	class Point
	{
	public:
		int m_x, m_y;
		COLORREF m_color = WHITE;
		PointType m_pointtype = point;
		const char* m_appearance = "";

		Point(int x, int y);

		virtual ~Point() = default;

		virtual std::unique_ptr<Point> clone() const = 0;

		virtual void Move(Maps& map) = 0;
		
		PointType GetType() const { return m_pointtype; }

	};

	class Obstcle : public Point
	{
	public:

		

		Obstcle(int x, int y);

		void Move(Maps& map) override{}


		std::unique_ptr<Point> clone() const override
		{
			return std::make_unique<Obstcle>(*this);
		}
	};

	class Exit : public Point
	{
	public:
		
		Exit(int x, int y);

		void Move(Maps& map) override {}

		std::unique_ptr<Point> clone() const override
		{
			return std::make_unique<Exit>(*this);
		}
	};



	class Character : public Point //创建一个类，表示各个角色，如玩家，鬼等的坐标和外形以及移动方式等
	{
	public:

		void Move(Maps& map) override{}

		bool RightMove = true;

		bool LeftMove = true;
		bool UpMove = true;
		bool DownMove = true;


		Character(int x, int y);

		void IsAgainstObstcle(Maps& map); //用于判断是否靠着墙壁
		
		virtual ~Character() = default;
	};


	class Player : public Character //创建一个Player类
	{
	public:

		

		Player(int x, int y); //用构造函数初始化玩家的坐标和外形
		
		std::unique_ptr<Point> clone() const override;
		void Move(Maps& map) override; //移动函数
		void Interact(Maps*& map);//判断与地图的交互

	};

	class Enemy : public Character //创建敌人
	{
	public:
		
		const COLORREF ColorSafe = RED;
		const COLORREF ColorDanger = RED;

		std::unique_ptr<Point> clone() const override;

		enum behavior //敌人有两种状态 safe 和 danger， safe状态时随机运动， danger状态追踪玩家
		{
			Safe,
			Danger
		};

		behavior CurrentBehavior = Safe;

		Enemy(int x, int y);//构造函数初始化敌人

		void See(); //当敌人看见玩家时，转化为追踪状态
		void Move(Maps& map) override;//敌人的移动逻辑，可能需要运用追踪算法

	};

	std::vector<std::unique_ptr<Point>> MapPoint; //创建一个动态数组，记录各点的属性

	void DrawMap(); //绘制地图
	void MoveAll(Maps& map);
	
};

void CreateMap1(Maps& map);//创建map1

#endif