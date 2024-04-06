#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "scene.h"
#include "player.h"
#include "enemy.h"
#include <vector>
#include<iostream>

extern SceneManager scene_manager;										//ͬ��Ҳ��main�������ȫ�ֱ���

extern std::vector<Enemy*> enemy_pig_list;															//���ǵ��˶�̬���飬�������ɴ���Ұ��
extern std::vector<Enemy*> enemy_bee_list;
extern std::vector<Bullet> bullet_list;														//�������ȷ���������ӵ�
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

	void on_enter()														//��������ʱ�ĳ�ʼ��
	{
		
	}

	void on_updata(int delta)
	{
		player.Move();

		TryGenerateEnemy(enemy_pig_list, enemy_bee_list);								//�������vector������װ����
		for (Enemy* enemy : enemy_pig_list)												//�����ʾ�������ͷ��βѭ��һ�ߣ�enemy�ǹ��ɱ仯��ָ��
			enemy->Move(player);														//�����������Ҫ��ѭ����������ÿһ�����˵�move�����Ի����һ�����ƶ�Ŀ��
		for (Enemy* enemy : enemy_bee_list)
			enemy->Move(player);

		//���Ұ��������ײ
		for (Enemy* enemy : enemy_pig_list)
		{
			if (enemy->CheckPlayerCollision(player))
			{
				static TCHAR text[128];
				_stprintf_s(text, _T("���յ÷�: %d"), score);
				MessageBox(GetHWnd(), text, _T("��Ϸ����"), MB_OK);
				running = false;
				break;
			}
		}
		//����۷�������ײ
		for (Enemy* enemy : enemy_bee_list)
		{
			if (enemy->CheckPlayerCollision(player))
			{
				static TCHAR text[128];
				_stprintf_s(text, _T("���յ÷�: %d"), score);
				MessageBox(GetHWnd(), text, _T("��Ϸ����"), MB_OK);
				running = false;
				break;
			}
		}

		//����ӵ���Ұ�����ײ
		for (Enemy* enemy : enemy_pig_list)													//��̬����Ķ���д���𣬴ӵ�һ�����������һ��
		{
			for (const Bullet& bullet : bullet_list)
			{
				if (enemy->CheckBulletCollision(bullet))
				{
					mciSendString(_T("play hit from 0"), NULL, 0, NULL);				//�������֣����²���������
					enemy->Hurt();														//�ı䵱ǰ���˵Ĵ��״̬
				}

			}
			if (enemy->check_is_damaged())
				enemy->missing_bullet_counter();
		}

		//����ӵ����۷����ײ
		for (Enemy* enemy : enemy_bee_list)													//��̬����Ķ���д���𣬴ӵ�һ�����������һ��
		{
			for (const Bullet& bullet : bullet_list)
			{
				if (enemy->CheckBulletCollision(bullet))
				{
					mciSendString(_T("play hit from 0"), NULL, 0, NULL);					//�������֣����²���������
					enemy->Hurt();															//�ı䵱ǰ���˵Ĵ��״̬
				}

			}
			if (enemy->check_is_damaged())
				enemy->missing_bullet_counter();
		}

		//�Ƴ�����ֵ�����Ұ��
		for (size_t i = 0; i < enemy_pig_list.size(); i++)
		{
			Enemy* enemy = enemy_pig_list[i];												//������Ϊ�˵����������ĵ��˺��ܰ�������NEW�ĵ��˶���Ŀռ�ɾ����˳�����ȵ�����ɾ�ռ�
			if (!enemy->CheckAlive())
			{
				std::swap(enemy_pig_list[i], enemy_pig_list.back());							//vector���鵯��ֻ�ܴ�ĩ�˵��������Ƚ��������˺����һ���Ƚ���λ��
				enemy_pig_list.pop_back();													//Ȼ���ٰ����һ��������
				delete enemy;
				score++;

			}

		}

		//�Ƴ�����ֵ������۷�
		for (size_t i = 0; i < enemy_bee_list.size(); i++)
		{
			Enemy* enemy = enemy_bee_list[i];												//������Ϊ�˵����������ĵ��˺��ܰ�������NEW�ĵ��˶���Ŀռ�ɾ����˳�����ȵ�����ɾ�ռ�
			if (!enemy->CheckAlive())
			{
				std::swap(enemy_bee_list[i], enemy_bee_list.back());							//vector���鵯��ֻ�ܴ�ĩ�˵��������Ƚ��������˺����һ���Ƚ���λ��
				enemy_bee_list.pop_back();													//Ȼ���ٰ����һ��������
				delete enemy;
				score++;

			}

		}
		UpdateBullets(bullet_list, player);												//���»���ӵ�λ��

	}																	//�������ݣ�������������������������


	void on_draw()
	{
		putimage(0, 0, &img_background);												//��IMAGED��������ڴ���ָ��λ��
		player.Draw(1000 / 144);
		for (Enemy* enemy : enemy_pig_list)													//��������������Ⱦ�����ڵ���һ����һ����������Ҫ������Ⱦ    
			enemy->Draw(1000 / 144);
		for (Enemy* enemy : enemy_bee_list)													//��������������Ⱦ�����ڵ���һ����һ����������Ҫ������Ⱦ    
			enemy->Draw(1000 / 144);

		for (const Bullet& bullet : bullet_list)										//�ӵ��Ļ���
			bullet.Draw();
		DrawPlayerScore(score);
	}																	//��Ⱦ��ͼ


	void on_input(const ExMessage& msg)
	{
		player.ProcessEvent(msg);
	}																	//�����ڴ����������ʱ����


	void on_exit()
	{
		std::cout << "���˵��˳�" << std::endl;
	}																	//�����˳�ʱ��ж���߼�

private:
	void TryGenerateEnemy(std::vector<Enemy*>& enemy_pig_list, std::vector<Enemy*>& enemy_bee_list)					//�������ɵ��˶���
	{
		const int INTERVAL_pig = 250;																					//Ұ���ˢ��ʱ����
		const int INTERVAL_bee = 1000;
		static int counter1 = 0, counter2 = 0;																				//�����ɵ��˾��ܿ�������һ���ƴ�����һ���Ĵ�ѭ����Ҳ��������ĸ�ѭ����Ҫһ��
		if ((++counter1) % INTERVAL_pig == 0)																			//ȷ����һ��ʱ��ˢ��һ�ε��ˣ���Ϊ����������û�ܿ�
			enemy_pig_list.push_back(new Enemy(atlas_enemy_left, atlas_enemy_right, atlas_enemy_left_sketch, atlas_enemy_right_sketch, 2, 2));
		if ((++counter2) % INTERVAL_bee == 0)
			enemy_bee_list.push_back(new Enemy(atlas_enemy_bee_left, atlas_enemy_bee_right, NULL, NULL, 4, 1));
	}

	void UpdateBullets(std::vector<Bullet>& bullet_list, const Player& player)										//ʵʱ�����ӵ���λ��
	{
		const double RADIAL_SPEED = 0.0045;																			//���򲨶��ٶ�
		const double TANGENT_SPEED = 0.0055;																		//���򲨶��ٶ�
		double radian_interval = 2 * 3.14159 / bullet_list.size();													//�ӵ�֮��Ļ��ȼ��
		POINT player_position = player.GetPosition();
		double radius = 100 + 25 * sin(GetTickCount() * RADIAL_SPEED);												//����������С��������ɵľ��룬�����ʱ�������仯�õ��˱仯�ĽǶȷǳ�����

		for (size_t i = 0; i < bullet_list.size(); i++)
		{
			double radian = GetTickCount() * TANGENT_SPEED + radian_interval * i;									//��ǰ�ӵ����ڵĻ���ֵ�����i���������ӵ��ֿܷ���ԭ��
			bullet_list[i].position.x = player_position.x + player.FRAME_WIDTH / 2 + (int)(radius * sin(radian));	//��X����Ϊ��������Ȼ��������������Ϊ��׼����֪С�������ҵľ���Ͷ�̬�仯�ĽǶȣ���ôֻҪ�þ�����Ա仯�ĽǶ��ټ�����ҵ���������Ϳ��Եõ�С�������
			bullet_list[i].position.y = player_position.y + player.FRAME_HEIGHT / 2 + (int)(radius * cos(radian));	//���Ƿ����������Ǻ������õ��ǻ�����

		}
	}

	//������ҵ÷�
	void DrawPlayerScore(int score)
	{
		static TCHAR text[64];
		_stprintf_s(text, _T("��ǰ��ҵ÷�: %d"), score);										//�����ǰ���仰�Ž�����ַ��������

		setbkmode(TRANSPARENT);																	//��һ�俴��������һ��ʲôʹ���ֱ���͸��
		settextcolor(RGB(255, 85, 185));														//����Ȼ������������ɫ��֮������ʾ���ݣ��β������ݺ��ı�
		outtextxy(10, 10, text);

	}
private:
	int score = 0;																			//��ҵ÷�

};


#endif // !_GAME_SCENE_H_

