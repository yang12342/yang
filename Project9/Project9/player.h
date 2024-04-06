#ifndef _PLAYER_H_
#define _PLAYER_H_
#include<string>												//��STRING����Ϊ��ƴ��ͼƬ·��
#include<cmath>
#include "animation.h"
extern int WINDOW_WIDTH ;
extern int WINDOW_HEIGHT ;

extern Atlas* atlas_player_left;
extern Atlas* atlas_player_right;

extern Atlas* atlas_player_left_sketch;
extern Atlas* atlas_player_right_sketch;

////////////////////////////////////player// ///////////////////////////
class Player                        //Ϊ��������Һ͵������Ƿֱ��װ��Ӧ����
{
public:
	const int SPEED = 3;																		//��Ұ����ƶ��ٶ�

	const int FRAME_WIDTH = 80;																	//��ҿ��
	const int FRAME_HEIGHT = 80;																//��Ҹ߶�
	const int SHADOW_WIDTH = 32;																//��Ӱ���
	Player()
	{
		loadimage(&img_shadow, _T("img/shadow_player.png"));									//�Ѷ�Ӧ��shadowͼ���ļ����ؽ�ȥ


		anim_left = new Animation(atlas_player_left, 45);										//��Ҷ�Ӧ������ͼ������ˣ����Ƿ���һ�����������һ�㶼��ָ���ˣ�����ֱ���ñ�������
		anim_right = new Animation(atlas_player_right, 45);
		anim_sketch_left = new Animation(atlas_player_left_sketch, 45);
		anim_sketch_right = new Animation(atlas_player_right_sketch, 45);						//��Ӧ�ļ�Ӱ��Դ�������Animation

	}

	~Player()
	{
		delete anim_left;
		delete anim_right;
	}

	void ProcessEvent(const ExMessage& msg)														//���ﴦ����ҵĲ���ָ��
	{
		switch (msg.message)																	//message��Ӧ�ľ��ǰ����İ��º��Ϳ�
		{
		case WM_KEYDOWN:
			switch (msg.vkcode)
			{

			case VK_UP:																			//�������VK����ؼ�����Բο��ĵ����EXMASSEGE��΢���ĵ���Ķ���
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

			case VK_UP:																				//�������VK����ؼ�����Բο��ĵ����EXMASSEGE��΢���ĵ���Ķ���
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

	void Move()																						//��������ƶ�������
	{
		int dir_x = is_move_right - is_move_left;													//���ﲻ����֮ǰ�Ļ����Ͻ����ÿ��ˢ��ͼƬ��λ�ñ仯�����һ�����Խ��ƶ����⣬��ʵ�������������Ҳ�Ƿǳ�������
		int dir_y = is_move_down - is_move_up;
		double len_dir = sqrt(dir_x * dir_x + dir_y * dir_y);
		if (len_dir != 0)
		{
			double normalized_x = dir_x / len_dir;													//double��ʹ�þ��������˸�����������ת��ʱ�ľ��ȶ�ʧ����
			double normalized_y = dir_y / len_dir;
			position.x += (int)(SPEED * normalized_x);
			position.y += (int)(SPEED * normalized_y);

		}

		if (position.x < 0)position.x = 0;
		if (position.y < 0)position.y = 0;
		if (position.x + FRAME_WIDTH > WINDOW_WIDTH)position.x = WINDOW_WIDTH - FRAME_WIDTH;
		if (position.y + FRAME_HEIGHT > WINDOW_HEIGHT)position.y = WINDOW_HEIGHT - FRAME_HEIGHT;	//������λ�õ�У׼��ֹ��ɫ��������֮�⣬���������߼����ǽ����ټ��ص�ͼƬ�ǳ����Σ�������λ�������Ͻ�
	}

	void Draw(int delta)																			//������ͼ����Ⱦ�Ĳ���
	{
		int pos_shadow_x = position.x + (FRAME_WIDTH / 2 - SHADOW_WIDTH / 2);						//�����ǻ�����Ӱ��λ�����꣬���ﻹ������һ��putimage���ش����������ͼ������ϽǶ�����ͼ�������
		int pos_shadow_y = position.y + FRAME_HEIGHT - 8;
		putimage_alpha(pos_shadow_x, pos_shadow_y, &img_shadow);									//������Ӱ



		static bool facing_left = false;															//����˵������ͨ����������ʾλ����ô��Ҳ����ͨ����������жϷ�������Ǹ������ұ���������
		int dir_x = is_move_right - is_move_left;
		if (dir_x < 0)
			facing_left = true;
		else if (dir_x > 0)
			facing_left = false;
		if (!missing_enemy_status)
		{


			if (facing_left)
				anim_left->Play(position.x, position.y, delta);											//�����anim��ָ������ֻ���ü�ͷ
			else
				anim_right->Play(position.x, position.y, delta);
		}
		else
		{
			static int counter1 = 0;																//��Ӱ�������������ĵ��л�������ʾ�޵�״̬��������֡��������ȷ���Ƿ�Ҫ�л�
			static bool is_choose_sketch = false;
			counter1++;
			if (counter1 % 10 == 0)
			{
				is_choose_sketch = !is_choose_sketch; counter1 = 0;                            //һ��ѭ���ߺ��룬�޵�״̬100֡�����700���룬ÿ10֡��һ�λ滭״̬������70����
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

	const POINT& GetPosition() const																//const���ں���ǰ�Ƿ�ֹ���ص�������ָ�����͵ı�������ֵ�޸ģ����Ҫ����ֵȥ��ס���ָ�룬Ҳ����Ҫconst ����*��ָ�룬�ź�����Ǳ�ʾ����������ܶԳ�Ա���������޸�
	{
		return position;
	}

	bool is_missing_status()
	{
		return missing_enemy_status;
	}

	void hurt()																						//�ܻ��ж�,��ͬ��enemy��hurt�����ǰѸı��ܻ�״̬�����˺������棬��Ϊ��������ڵ��˵������棬����ֱ�ӵ���˽�г�Ա����player�����ܵ���
	{
		HP--;
		if (HP > 0)
		{
			missing_enemy_status = true;
		}
		is_be_damaged = true;
	}

	void missing_enemy_counter()																			//�޵�ʱ�䵹��
	{
		static int counter = 0;																				//����˵�ֲ������ڵı����������ں̣ܶ�����static����ӳ�������������ÿ��ѭ����������һ�θı�����ݣ������ٳ�ʼ��
		counter++;
		if (counter % 500 == 0)																				//�Ǹ����ҵ�ˢ�´����������ģ��ߺ���һ��ѭ����142�δ�ž���1��,500��ѭ��
		{
			missing_enemy_status = false; is_be_damaged = false;											//missing_bullet_status��һ��״̬������Ѫ���йأ���ʣ��Ѫ�ܻ��������޵�״̬��ûʣ��Ѫ���򲻻�����޵�״̬����is_be_damaged��һ���жϱ��������ܻ�����У��ܻ����ı䣬��Ϊ����Ϸ���߼����ȼ���ܻ����ܻ��ʹ���hurt������hurt���Ǹı�Ѫ��������Ƿ�Ҫ�����޵�״̬��Ȼ�����Ѫ�����ж��Ƿ�Ҫɾ��
			counter = 0;
		}
	}

	bool checkAlive()
	{
		return HP;
	}




private:
	IMAGE img_shadow;																				//������Ӱͼ��ı���
	POINT position = { 500,500 };																	//����Ӧ���ǹ��ڵ�Ľṹ�壬���溬��x,y
	Animation* anim_left;																			//��ʱ��û�õ�
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
