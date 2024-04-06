#ifndef _PLAYER_H_
#define _PLAYER_H_
#include<string>												//用STRING库是为了拼凑图片路径
#include<cmath>
#include "animation.h"
extern int WINDOW_WIDTH ;
extern int WINDOW_HEIGHT ;

extern Atlas* atlas_player_left;
extern Atlas* atlas_player_right;

extern Atlas* atlas_player_left_sketch;
extern Atlas* atlas_player_right_sketch;

////////////////////////////////////player// ///////////////////////////
class Player                        //为了区分玩家和敌人我们分别封装对应的类
{
public:
	const int SPEED = 3;																		//玩家按键移动速度

	const int FRAME_WIDTH = 80;																	//玩家宽度
	const int FRAME_HEIGHT = 80;																//玩家高度
	const int SHADOW_WIDTH = 32;																//阴影宽度
	Player()
	{
		loadimage(&img_shadow, _T("img/shadow_player.png"));									//把对应的shadow图像文件加载进去


		anim_left = new Animation(atlas_player_left, 45);										//玩家对应的左右图像加载了，我们发现一旦再类里面就一般都用指针了，很少直接用变量名了
		anim_right = new Animation(atlas_player_right, 45);
		anim_sketch_left = new Animation(atlas_player_left_sketch, 45);
		anim_sketch_right = new Animation(atlas_player_right_sketch, 45);						//对应的剪影资源进入放入Animation

	}

	~Player()
	{
		delete anim_left;
		delete anim_right;
	}

	void ProcessEvent(const ExMessage& msg)														//这里处理玩家的操作指令
	{
		switch (msg.message)																	//message反应的就是按键的按下和送开
		{
		case WM_KEYDOWN:
			switch (msg.vkcode)
			{

			case VK_UP:																			//这里关于VK的相关键码可以参考文档里的EXMASSEGE和微软文档里的东西
				is_move_up = true;
				break;
			case VK_DOWN:
				is_move_down = true;
				break;
			case VK_LEFT:
				is_move_left = true;
				break;
			case VK_RIGHT:
				is_move_right = true;
				break;


			}
			break;


		case WM_KEYUP:
			switch (msg.vkcode)
			{

			case VK_UP:																				//这里关于VK的相关键码可以参考文档里的EXMASSEGE和微软文档里的东西
				is_move_up = false;
				break;
			case VK_DOWN:
				is_move_down = false;
				break;
			case VK_LEFT:
				is_move_left = false;
				break;
			case VK_RIGHT:
				is_move_right = false;
				break;


			}
			break;

		}
	}

	void Move()																						//处理玩家移动的问题
	{
		int dir_x = is_move_right - is_move_left;													//这里不仅在之前的基础上解决了每次刷新图片的位置变化，而且还解决对角移动问题，其实就是向量嘛，计算也是非常巧妙了
		int dir_y = is_move_down - is_move_up;
		double len_dir = sqrt(dir_x * dir_x + dir_y * dir_y);
		if (len_dir != 0)
		{
			double normalized_x = dir_x / len_dir;													//double的使用尽量避免了浮点数和整数转换时的精度丢失问题
			double normalized_y = dir_y / len_dir;
			position.x += (int)(SPEED * normalized_x);
			position.y += (int)(SPEED * normalized_y);

		}

		if (position.x < 0)position.x = 0;
		if (position.y < 0)position.y = 0;
		if (position.x + FRAME_WIDTH > WINDOW_WIDTH)position.x = WINDOW_WIDTH - FRAME_WIDTH;
		if (position.y + FRAME_HEIGHT > WINDOW_HEIGHT)position.y = WINDOW_HEIGHT - FRAME_HEIGHT;	//这里是位置的校准防止角色超出画面之外，这里运算逻辑还是建立再加载的图片是长方形，和坐标位置在左上角
	}

	void Draw(int delta)																			//这里是图像渲染的部分
	{
		int pos_shadow_x = position.x + (FRAME_WIDTH / 2 - SHADOW_WIDTH / 2);						//这里是绘制阴影的位置坐标，这里还是想提一嘴putimage加载传输的坐标是图像的左上角而不是图像的中心
		int pos_shadow_y = position.y + FRAME_HEIGHT - 8;
		putimage_alpha(pos_shadow_x, pos_shadow_y, &img_shadow);									//绘制阴影



		static bool facing_left = false;															//简单来说就是你通过坐标来表示位置那么你也可以通过坐标差来判断方向，左边是负半轴右边是正半轴
		int dir_x = is_move_right - is_move_left;
		if (dir_x < 0)
			facing_left = true;
		else if (dir_x > 0)
			facing_left = false;
		if (!missing_enemy_status)
		{


			if (facing_left)
				anim_left->Play(position.x, position.y, delta);											//这里的anim是指针所以只能用箭头
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

	const POINT& GetPosition() const																//const放在函数前是防止返回的尤其是指针类型的被当作左值修改，如果要用左值去接住这个指针，也必须要const 类型*的指针，放后面就是表示这个函数不能对成员变量进行修改
	{
		return position;
	}

	bool is_missing_status()
	{
		return missing_enemy_status;
	}

	void hurt()																						//受击判断,不同于enemy的hurt，我们把改变受击状态放在了函数里面，因为这个函数在敌人的类里面，可以直接调用私有成员，而player并不能调用
	{
		HP--;
		if (HP > 0)
		{
			missing_enemy_status = true;
		}
		is_be_damaged = true;
	}

	void missing_enemy_counter()																			//无敌时间倒数
	{
		static int counter = 0;																				//按理说局部函数内的变量生命周期很短，但是static则会延长到函数结束。每次循环都沿用上一次改变的数据，不会再初始化
		counter++;
		if (counter % 500 == 0)																				//是根据我的刷新次数来决定的，七毫秒一次循环，142次大概就是1秒,500次循环
		{
			missing_enemy_status = false; is_be_damaged = false;											//missing_bullet_status是一个状态量，与血量有关，有剩余血受击则会进入无敌状态，没剩余血量则不会进入无敌状态，而is_be_damaged是一个判断变量，在受击检测中，受击即改变，因为在游戏的逻辑中先检测受击，受击就触发hurt（），hurt就是改变血量并检测是否要进入无敌状态，然后根据血量来判断是否要删除
			counter = 0;
		}
	}

	bool checkAlive()
	{
		return HP;
	}




private:
	IMAGE img_shadow;																				//加载阴影图像的变量
	POINT position = { 500,500 };																	//这里应该是关于点的结构体，里面含有x,y
	Animation* anim_left;																			//暂时还没用到
	Animation* anim_right;
	Animation* anim_sketch_left;
	Animation* anim_sketch_right;
	bool is_move_up = false;
	bool is_move_down = false;
	bool is_move_left = false;
	bool is_move_right = false;
	bool missing_enemy_status = false;
	bool is_be_damaged = false;
	int HP = 2;
};




#endif _PLAYER_H_
