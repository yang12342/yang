#ifndef _ENEMY_H_
#define _ENEMY_H_
#include<cstdlib>												//随机数库，用于生成敌人出生的位置
#include "animation.h"
#include "bullet.h"
extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;


/// <summary>
/// /////////enemy
/// </summary>
class Enemy
{
public:
	Enemy(Atlas* atlas_left,Atlas* atlas_right,Atlas* atlas_sketch_left,Atlas* atlas_sketch_right,int speed,int hp)
	{
		loadimage(&img_shadow, _T("img/shadow_enemy.png"));
		anim_left = new Animation(atlas_left, 45);
		anim_right = new Animation(atlas_right, 45);

		//检查我创建的对象是否需要剪影对象
		if (atlas_sketch_left != NULL) {
			anim_sketch_left = new Animation(atlas_sketch_left, 45);								//这里形参的都是对应的图集的指针，指针指向真正的图集
			anim_sketch_right = new Animation(atlas_sketch_right, 45);
		}

		SPEED = speed;
		HP = hp;

		FRAME_WIDTH = atlas_left->get_image(0)->getwidth();											//用image里的函数去获取我加载的图片高度和宽度，就不用我输入了多好
		FRAME_HEIGHT = atlas_left->get_image(0)->getheight();

		enum class SpawnEdge																		//虽然是枚举但是为什么带class,然后这里是判断敌人刷新位置的方向基础

		{
			Up = 0,
			Down,
			Left,
			Right
		};

		SpawnEdge edge = (SpawnEdge)(rand() % 4);													//将敌人放置再地图边界处的随机位置
		switch (edge)																				//确定了出生的方向再确定出生的具体位置
		{
		case SpawnEdge::Up:
			position.x = rand() % WINDOW_WIDTH;
			position.y = -FRAME_HEIGHT;
			break;
		case SpawnEdge::Down:
			position.x = rand() % WINDOW_WIDTH;
			position.y = WINDOW_HEIGHT;
			break;
		case SpawnEdge::Left:
			position.y = rand() % WINDOW_HEIGHT;
			position.x = -FRAME_HEIGHT;
			break;
		case SpawnEdge::Right:
			position.y = rand() % WINDOW_HEIGHT;
			position.x = WINDOW_WIDTH;
			break;
		default:
			break;
		}
	}

	bool CheckBulletCollision(const Bullet& bullet)														//检测与子弹的碰撞，在引用面前加const，表示不可以修改内部的值
	{
		//将子弹等效为点，判断敌人是否在矩形内
		bool is_overlap_x = bullet.position.x >= position.x && bullet.position.x <= position.x + FRAME_WIDTH;
		bool is_overlap_y = bullet.position.y >= position.y && bullet.position.y <= position.y + FRAME_HEIGHT;
		if (is_overlap_x && is_overlap_y && (!missing_bullet_status))is_be_damaged = true;				//判定是否受击，之后在counter该回去
		return is_overlap_x && is_overlap_y && (!missing_bullet_status);
	}

	bool CheckPlayerCollision(Player& player)														//检测与玩家的碰撞
	{
		//将敌人的中心位置等效为点，判断点是否在玩家矩形内
		POINT check_position = { position.x + FRAME_WIDTH / 2,position.y + FRAME_HEIGHT / 2 };
		POINT player_position = player.GetPosition();

		bool is_overlap_x = check_position.x >= player_position.x && check_position.x <= player_position.x + 80;
		bool is_overlap_y = check_position.y >= player_position.y && check_position.y <= player_position.y + 80;
		if (is_overlap_x && is_overlap_y && !(player.is_missing_status()))player.hurt();				//判定是否受击，之后在counter该回去
		if (player.is_missing_status())player.missing_enemy_counter();
		return (is_overlap_x && is_overlap_y && !(player.checkAlive()));

	}

	void Move(const Player& player)
	{
		const POINT& player_position = player.GetPosition();
		int dir_x = player_position.x - position.x;
		int dir_y = player_position.y - position.y;														//通过坐标差和向量的方法来确定要行动的方向
		double len_dir = sqrt(dir_x * dir_x + dir_y * dir_y);
		if (len_dir != 0)
		{
			double normalized_x = dir_x / len_dir;
			double normalized_y = dir_y / len_dir;
			position.x += (int)(SPEED * normalized_x);
			position.y += (int)(SPEED * normalized_y);

		}

		if (dir_x < 0)
			facing_left = true;
		else if (dir_x > 0)
			facing_left = false;
	}

	void Draw(int delta)
	{
		int pos_shadow_x = position.x + (FRAME_WIDTH / 2 - SHADOW_WIDTH / 2);					//这里是绘制阴影的位置坐标，这里还是想提一嘴putimage加载传输的坐标是图像的左上角而不是图像的中心
		int pos_shadow_y = position.y + FRAME_HEIGHT - 8;
		putimage_alpha(pos_shadow_x, pos_shadow_y - 25, &img_shadow);								//绘制阴影

		if (!missing_bullet_status)
		{
			if (facing_left)
				anim_left->Play(position.x, position.y, delta);
			else
				anim_right->Play(position.x, position.y, delta);
		}
		else
		{
			static int counter1 = 0;																//剪影画面和正常画面的的切换播放显示无敌状态，这里是帧计数器来确定是否要切换
			static bool is_choose_sketch = false;
			counter1++;
			if (counter1 % 10 == 0)
			{
				is_choose_sketch = !is_choose_sketch; counter1 = 0;                            //一个循环七毫秒，无敌状态100帧，大概700毫秒，每10帧换一次绘画状态，就是70毫秒
			}
			if (is_choose_sketch)
			{
				if (facing_left)
					anim_sketch_left->Play(position.x, position.y, delta);
				else
					anim_sketch_right->Play(position.x, position.y, delta);
			}
			else
			{
				if (facing_left)
					anim_left->Play(position.x, position.y, delta);
				else
					anim_right->Play(position.x, position.y, delta);
			}
		}
	}

	/// <summary>
	/// 这段主要和检测受击有关，也具有通用性
	/// </summary>
	void Hurt()
	{
		HP--;
		if (HP > 0)
		{
			missing_bullet_status = true;
		}
	}

	void missing_bullet_counter()
	{
		static int counter = 0;																				//按理说局部函数内的变量生命周期很短，但是static则会延长到函数结束。每次循环都沿用上一次改变的数据，不会再初始化
		counter++;
		if (counter % 100 == 0)																				//是根据我的刷新次数来决定的，七毫秒一次循环，142次大概就是1秒
		{
			missing_bullet_status = false; is_be_damaged = false;											//missing_bullet_status是一个状态量，与血量有关，有剩余血受击则会进入无敌状态，没剩余血量则不会进入无敌状态，而is_be_damaged是一个判断变量，在受击检测中，受击即改变，因为在游戏的逻辑中先检测受击，受击就触发hurt（），hurt就是改变血量并检测是否要进入无敌状态，然后根据血量来判断是否要删除
			counter = 0;
		}
	}

	bool check_is_damaged()
	{
		return is_be_damaged;
	}

	int CheckAlive()
	{
		return HP;
	}

	~Enemy()
	{
		delete anim_left;
		delete anim_right;
		delete anim_sketch_left;
		delete anim_sketch_right;
	}

private:
	int SPEED;																									//敌人移动速度，还有很想知道这些参数如何获取  

	int FRAME_WIDTH ;																							//敌人宽度
	int FRAME_HEIGHT;																							//敌人高度
	int SHADOW_WIDTH = 48;																						//敌人宽度

	IMAGE img_shadow;																							//加载阴影图像的变量
	POINT position = { 0,0 };																					//这里应该是关于点的结构体，里面含有x,y
	Animation* anim_left;																						//暂时还没用到
	Animation* anim_right;
	Animation* anim_sketch_left;
	Animation* anim_sketch_right;
	bool facing_left = false;
	int  HP;																									//怪物的血量
	bool missing_bullet_status = false;																			//是否处于无敌状态
	bool is_be_damaged = false;
};










#endif // !_ENEMY_H_
