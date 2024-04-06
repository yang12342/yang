#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "scene.h"
#include "player.h"
#include "enemy.h"
#include <vector>
#include<iostream>

extern SceneManager scene_manager;										//同样也是main函数里的全局变量

extern std::vector<Enemy*> enemy_pig_list;															//这是敌人动态数组，用于容纳存活的野猪
extern std::vector<Enemy*> enemy_bee_list;
extern std::vector<Bullet> bullet_list;														//这里就明确创建三个子弹
extern Player player;
extern IMAGE img_background;
extern Atlas* atlas_enemy_left;
extern Atlas* atlas_enemy_right;
extern Atlas* atlas_enemy_bee_left;
extern Atlas* atlas_enemy_bee_right;
extern Atlas* atlas_enemy_bee_left_sketch;
extern Atlas* atlas_enemy_bee_right_sketch;
extern Atlas* atlas_enemy_left_sketch;
extern Atlas* atlas_enemy_right_sketch;


class GameScene :public Scene
{
public:
	GameScene() = default;
	~GameScene() = default;

	void on_enter()														//场景进入时的初始化
	{
		
	}

	void on_updata(int delta)
	{
		player.Move();

		TryGenerateEnemy(enemy_pig_list, enemy_bee_list);								//接受这个vector数组来装东西
		for (Enemy* enemy : enemy_pig_list)												//这里表示把数组从头到尾循环一边，enemy是规律变化的指针
			enemy->Move(player);														//这里的作用主要是循环数组里面每一个敌人的move函数以获得下一步的移动目标
		for (Enemy* enemy : enemy_bee_list)
			enemy->Move(player);

		//检测野猪和玩家碰撞
		for (Enemy* enemy : enemy_pig_list)
		{
			if (enemy->CheckPlayerCollision(player))
			{
				static TCHAR text[128];
				_stprintf_s(text, _T("最终得分: %d"), score);
				MessageBox(GetHWnd(), text, _T("游戏结束"), MB_OK);
				running = false;
				break;
			}
		}
		//检测蜜蜂和玩家碰撞
		for (Enemy* enemy : enemy_bee_list)
		{
			if (enemy->CheckPlayerCollision(player))
			{
				static TCHAR text[128];
				_stprintf_s(text, _T("最终得分: %d"), score);
				MessageBox(GetHWnd(), text, _T("游戏结束"), MB_OK);
				running = false;
				break;
			}
		}

		//检测子弹与野猪的碰撞
		for (Enemy* enemy : enemy_pig_list)													//动态数组的独特写法吗，从第一个遍历到最后一个
		{
			for (const Bullet& bullet : bullet_list)
			{
				if (enemy->CheckBulletCollision(bullet))
				{
					mciSendString(_T("play hit from 0"), NULL, 0, NULL);				//背景音乐，等下播放有问题
					enemy->Hurt();														//改变当前敌人的存活状态
				}

			}
			if (enemy->check_is_damaged())
				enemy->missing_bullet_counter();
		}

		//检测子弹与蜜蜂的碰撞
		for (Enemy* enemy : enemy_bee_list)													//动态数组的独特写法吗，从第一个遍历到最后一个
		{
			for (const Bullet& bullet : bullet_list)
			{
				if (enemy->CheckBulletCollision(bullet))
				{
					mciSendString(_T("play hit from 0"), NULL, 0, NULL);					//背景音乐，等下播放有问题
					enemy->Hurt();															//改变当前敌人的存活状态
				}

			}
			if (enemy->check_is_damaged())
				enemy->missing_bullet_counter();
		}

		//移除生命值归零的野猪
		for (size_t i = 0; i < enemy_pig_list.size(); i++)
		{
			Enemy* enemy = enemy_pig_list[i];												//这里是为了弹出已死亡的敌人后能把在数组NEW的敌人对象的空间删掉，顺序是先弹出再删空间
			if (!enemy->CheckAlive())
			{
				std::swap(enemy_pig_list[i], enemy_pig_list.back());							//vector数组弹出只能从末端弹出所以先将死亡敌人和最后一个先交换位置
				enemy_pig_list.pop_back();													//然后再把最后一个弹出来
				delete enemy;
				score++;

			}

		}

		//移除生命值归零的蜜蜂
		for (size_t i = 0; i < enemy_bee_list.size(); i++)
		{
			Enemy* enemy = enemy_bee_list[i];												//这里是为了弹出已死亡的敌人后能把在数组NEW的敌人对象的空间删掉，顺序是先弹出再删空间
			if (!enemy->CheckAlive())
			{
				std::swap(enemy_bee_list[i], enemy_bee_list.back());							//vector数组弹出只能从末端弹出所以先将死亡敌人和最后一个先交换位置
				enemy_bee_list.pop_back();													//然后再把最后一个弹出来
				delete enemy;
				score++;

			}

		}
		UpdateBullets(bullet_list, player);												//更新获得子弹位置

	}																	//处理数据，根据玩家输入来整理更新数据


	void on_draw()
	{
		putimage(0, 0, &img_background);												//把IMAGED对象绘制在窗口指定位置
		player.Draw(1000 / 144);
		for (Enemy* enemy : enemy_pig_list)													//这里三个都是渲染，由于敌人一个算一个对象所以要遍历渲染    
			enemy->Draw(1000 / 144);
		for (Enemy* enemy : enemy_bee_list)													//这里三个都是渲染，由于敌人一个算一个对象所以要遍历渲染    
			enemy->Draw(1000 / 144);

		for (const Bullet& bullet : bullet_list)										//子弹的绘制
			bullet.Draw();
		DrawPlayerScore(score);
	}																	//渲染绘图


	void on_input(const ExMessage& msg)
	{
		player.ProcessEvent(msg);
	}																	//场景在处理玩家输入时调用


	void on_exit()
	{
		std::cout << "主菜单退出" << std::endl;
	}																	//场景退出时的卸载逻辑

private:
	void TryGenerateEnemy(std::vector<Enemy*>& enemy_pig_list, std::vector<Enemy*>& enemy_bee_list)					//用于生成敌人对象
	{
		const int INTERVAL_pig = 250;																					//野猪的刷新时间间隔
		const int INTERVAL_bee = 1000;
		static int counter1 = 0, counter2 = 0;																				//从生成敌人就能看出这是一个计次数，一秒四次循环，也就是完成四个循环需要一秒
		if ((++counter1) % INTERVAL_pig == 0)																			//确保隔一段时间刷新一次敌人，因为这个函数调用会很快
			enemy_pig_list.push_back(new Enemy(atlas_enemy_left, atlas_enemy_right, atlas_enemy_left_sketch, atlas_enemy_right_sketch, 2, 2));
		if ((++counter2) % INTERVAL_bee == 0)
			enemy_bee_list.push_back(new Enemy(atlas_enemy_bee_left, atlas_enemy_bee_right, NULL, NULL, 4, 1));
	}

	void UpdateBullets(std::vector<Bullet>& bullet_list, const Player& player)										//实时更新子弹的位置
	{
		const double RADIAL_SPEED = 0.0045;																			//径向波动速度
		const double TANGENT_SPEED = 0.0055;																		//切向波动速度
		double radian_interval = 2 * 3.14159 / bullet_list.size();													//子弹之间的弧度间隔
		POINT player_position = player.GetPosition();
		double radius = 100 + 25 * sin(GetTickCount() * RADIAL_SPEED);												//这里计算的是小球距离派蒙的距离，这里的时间连续变化得到了变化的角度非常巧妙

		for (size_t i = 0; i < bullet_list.size(); i++)
		{
			double radian = GetTickCount() * TANGENT_SPEED + radian_interval * i;									//当前子弹所在的弧度值这里的i就是三个子弹能分开的原因
			bullet_list[i].position.x = player_position.x + player.FRAME_WIDTH / 2 + (int)(radius * sin(radian));	//以X坐标为例，很显然以派蒙中心坐标为基准，已知小球距离玩家的距离和动态变化的角度，那么只要拿距离乘以变化的角度再加上玩家的中心坐标就可以得到小球的坐标
			bullet_list[i].position.y = player_position.y + player.FRAME_HEIGHT / 2 + (int)(radius * cos(radian));	//我们发现这里三角函数采用的是弧度制

		}
	}

	//绘制玩家得分
	void DrawPlayerScore(int score)
	{
		static TCHAR text[64];
		_stprintf_s(text, _T("当前玩家得分: %d"), score);										//这里是把这句话放进这个字符数组变量

		setbkmode(TRANSPARENT);																	//第一句看不懂搜了一下什么使文字背景透明
		settextcolor(RGB(255, 85, 185));														//很显然是设置字体颜色，之后是显示内容，形参是内容和文本
		outtextxy(10, 10, text);

	}
private:
	int score = 0;																			//玩家得分

};


#endif // !_GAME_SCENE_H_

