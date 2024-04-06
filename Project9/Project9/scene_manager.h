#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

#include "scene.h"
#include "game_scene.h"
#include "menu_scene.h"


extern Scene* menu_scene;										//�ļ���ȫ�ֱ����ڱ���ļ��п���ͨ��extern���ã��������ȫ�ֱ���ǰ��cosnt����ļ��Ͳ����Ե�����
extern Scene* game_scene;

class SceneManager
{
public:
	enum class SceneType										//��ö����������ǰ����Ϸ״̬
	{
		Menu,
		Game,
	};

public:
	SceneManager() = default;
	~SceneManager() = default;



	void set_current_scene(Scene* scene)						//���õ�ǰ����Ϸ����
	{
		current_scene = scene;									//ָ�뱣�浱ǰ��״̬
		current_scene->on_enter();								//������Ϸ������Ҫ����
	}

	void switch_to(SceneType type)								//����ִ�г���֮�����ת�߼�
	{
		current_scene->on_exit();								//���˳���ǰ�ĳ���
		switch (type)
		{
		case SceneType::Menu:									//�����ǰ����״̬����
			current_scene = menu_scene;
			break;
		case SceneType::Game:
			current_scene = game_scene;
			break;
		default:
			break;

		}

		current_scene->on_enter();								//�����µĳ���
	}


	void on_updata(int delta)
	{
		current_scene->on_updata(delta);
	}

	void on_draw()
	{
		current_scene->on_draw();
	}

	void on_input(const ExMessage& msg)
	{
		current_scene->on_input(msg);
	}


private:
	Scene* current_scene = nullptr;

};



#endif // !_SCENE_MANAGER_H_
