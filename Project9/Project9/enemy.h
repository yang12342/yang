#ifndef _ENEMY_H_
#define _ENEMY_H_
#include<cstdlib>												//������⣬�������ɵ��˳�����λ��
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

		//����Ҵ����Ķ����Ƿ���Ҫ��Ӱ����
		if (atlas_sketch_left != NULL) {
			anim_sketch_left = new Animation(atlas_sketch_left, 45);								//�����βεĶ��Ƕ�Ӧ��ͼ����ָ�룬ָ��ָ��������ͼ��
			anim_sketch_right = new Animation(atlas_sketch_right, 45);
		}

		SPEED = speed;
		HP = hp;

		FRAME_WIDTH = atlas_left->get_image(0)->getwidth();											//��image��ĺ���ȥ��ȡ�Ҽ��ص�ͼƬ�߶ȺͿ�ȣ��Ͳ����������˶��
		FRAME_HEIGHT = atlas_left->get_image(0)->getheight();

		enum class SpawnEdge																		//��Ȼ��ö�ٵ���Ϊʲô��class,Ȼ���������жϵ���ˢ��λ�õķ������

		{
			Up = 0,
			Down,
			Left,
			Right
		};

		SpawnEdge edge = (SpawnEdge)(rand() % 4);													//�����˷����ٵ�ͼ�߽紦�����λ��
		switch (edge)																				//ȷ���˳����ķ�����ȷ�������ľ���λ��
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

	bool CheckBulletCollision(const Bullet& bullet)														//������ӵ�����ײ����������ǰ��const����ʾ�������޸��ڲ���ֵ
	{
		//���ӵ���ЧΪ�㣬�жϵ����Ƿ��ھ�����
		bool is_overlap_x = bullet.position.x >= position.x && bullet.position.x <= position.x + FRAME_WIDTH;
		bool is_overlap_y = bullet.position.y >= position.y && bullet.position.y <= position.y + FRAME_HEIGHT;
		if (is_overlap_x && is_overlap_y && (!missing_bullet_status))is_be_damaged = true;				//�ж��Ƿ��ܻ���֮����counter�û�ȥ
		return is_overlap_x && is_overlap_y && (!missing_bullet_status);
	}

	bool CheckPlayerCollision(Player& player)														//�������ҵ���ײ
	{
		//�����˵�����λ�õ�ЧΪ�㣬�жϵ��Ƿ�����Ҿ�����
		POINT check_position = { position.x + FRAME_WIDTH / 2,position.y + FRAME_HEIGHT / 2 };
		POINT player_position = player.GetPosition();

		bool is_overlap_x = check_position.x >= player_position.x && check_position.x <= player_position.x + 80;
		bool is_overlap_y = check_position.y >= player_position.y && check_position.y <= player_position.y + 80;
		if (is_overlap_x && is_overlap_y && !(player.is_missing_status()))player.hurt();				//�ж��Ƿ��ܻ���֮����counter�û�ȥ
		if (player.is_missing_status())player.missing_enemy_counter();
		return (is_overlap_x && is_overlap_y && !(player.checkAlive()));

	}

	void Move(const Player& player)
	{
		const POINT& player_position = player.GetPosition();
		int dir_x = player_position.x - position.x;
		int dir_y = player_position.y - position.y;														//ͨ�������������ķ�����ȷ��Ҫ�ж��ķ���
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
		int pos_shadow_x = position.x + (FRAME_WIDTH / 2 - SHADOW_WIDTH / 2);					//�����ǻ�����Ӱ��λ�����꣬���ﻹ������һ��putimage���ش����������ͼ������ϽǶ�����ͼ�������
		int pos_shadow_y = position.y + FRAME_HEIGHT - 8;
		putimage_alpha(pos_shadow_x, pos_shadow_y - 25, &img_shadow);								//������Ӱ

		if (!missing_bullet_status)
		{
			if (facing_left)
				anim_left->Play(position.x, position.y, delta);
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

	/// <summary>
	/// �����Ҫ�ͼ���ܻ��йأ�Ҳ����ͨ����
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
		static int counter = 0;																				//����˵�ֲ������ڵı����������ں̣ܶ�����static����ӳ�������������ÿ��ѭ����������һ�θı�����ݣ������ٳ�ʼ��
		counter++;
		if (counter % 100 == 0)																				//�Ǹ����ҵ�ˢ�´����������ģ��ߺ���һ��ѭ����142�δ�ž���1��
		{
			missing_bullet_status = false; is_be_damaged = false;											//missing_bullet_status��һ��״̬������Ѫ���йأ���ʣ��Ѫ�ܻ��������޵�״̬��ûʣ��Ѫ���򲻻�����޵�״̬����is_be_damaged��һ���жϱ��������ܻ�����У��ܻ����ı䣬��Ϊ����Ϸ���߼����ȼ���ܻ����ܻ��ʹ���hurt������hurt���Ǹı�Ѫ��������Ƿ�Ҫ�����޵�״̬��Ȼ�����Ѫ�����ж��Ƿ�Ҫɾ��
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
	int SPEED;																									//�����ƶ��ٶȣ����к���֪����Щ������λ�ȡ  

	int FRAME_WIDTH ;																							//���˿��
	int FRAME_HEIGHT;																							//���˸߶�
	int SHADOW_WIDTH = 48;																						//���˿��

	IMAGE img_shadow;																							//������Ӱͼ��ı���
	POINT position = { 0,0 };																					//����Ӧ���ǹ��ڵ�Ľṹ�壬���溬��x,y
	Animation* anim_left;																						//��ʱ��û�õ�
	Animation* anim_right;
	Animation* anim_sketch_left;
	Animation* anim_sketch_right;
	bool facing_left = false;
	int  HP;																									//�����Ѫ��
	bool missing_bullet_status = false;																			//�Ƿ����޵�״̬
	bool is_be_damaged = false;
};










#endif // !_ENEMY_H_
