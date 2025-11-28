#pragma once

#include<iostream>
#include<easyx.h>
#include<conio.h>
#include<windows.h>
#include<vector>
#include <time.h>
#include<memory>

constexpr int ScreenLen = 1000; //游戏界面长宽均为1000像素
constexpr int CharLen = 20;//字体长宽为20像素
constexpr int ExtraWidth = 300;
constexpr int Len = ScreenLen / CharLen;
constexpr int MapNum = 5;
extern int mapindex;
extern bool running;
extern int score;
class Maps   // 创建地图
{
public:
	
	enum PointType //记录地图上各点类型
	{
		point = 0,
		obstcle,
		exit,
		player,
		enemy,
		coin
	};

	class Point //创建一个“点”类
	{
	public:
		int m_x, m_y; //x, y坐标
		COLORREF m_color = WHITE;
		PointType m_pointtype = point;
		const char* m_appearance = "";

		Point(int x, int y);

		virtual ~Point() = default; //虚析构函数，防止内存泄漏

		virtual std::unique_ptr<Point> clone() const = 0; //克隆函数，用于将类存入动态数组中

		virtual void Move(Maps& map) = 0; //移动函数
		
		PointType GetType() const { return m_pointtype; }//用于判断子类的类型

	};

	class Obstcle : public Point
	{
	public:

		Obstcle(int x, int y);

		void Move(Maps& map) override{} 

		std::unique_ptr<Point> clone() const override;//克隆函数，用于将类存入动态数组中
	};

	class Exit : public Point
	{
	public:
		
		Exit(int x, int y);

		void Move(Maps& map) override {}

		std::unique_ptr<Point> clone() const override;//克隆函数，用于将类存入动态数组中
	};

	class Coin : public Point
	{
	public:
		Coin(int x, int y);

		void Move(Maps& map) override {}
		
		std::unique_ptr<Point> clone() const override;//克隆函数，用于将类存入动态数组中

	};



	class Character : public Point //创建一个类，表示各个角色，如玩家，鬼等的坐标和外形以及移动方式等
	{
	public:

		void Move(Maps& map) override{}

		bool RightMove = true;//用于判断是否靠着墙壁
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
		void Interact(Maps& map);//判断与地图的交互

	};

	class Enemy : public Character //创建敌人
	{
	public:
		
		const COLORREF ColorSafe = RGB(255, 189, 189);
		const COLORREF ColorDanger = RED;
		int m_moveCount = 0;
		int m_currentDirection = 0;

		std::unique_ptr<Point> clone() const override;

		enum behavior //敌人有两种状态 safe 和 danger， safe状态时随机运动， danger状态追踪玩家
		{
			Safe,
			Danger
		};

		behavior CurrentBehavior = Safe;

		Enemy(int x, int y);//构造函数初始化敌人

		bool See(); //看见玩家时，返回true，反之false
		void Move(Maps& map) override;//敌人的移动逻辑，可能需要运用追踪算法

	};



	std::vector<std::unique_ptr<Point>> MapPoint; //创建一个动态数组，记录各点的属性

	void DrawMap(); //绘制地图
	void MoveAll(Maps& map);
	
};

