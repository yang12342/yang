#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

#include "scene.h"
#include "game_scene.h"
#include "menu_scene.h"


extern Scene* menu_scene;										//文件的全局变量在别的文件中可以通过extern调用，但如果在全局变量前加cosnt别的文件就不可以调用了
extern Scene* game_scene;

class SceneManager
{
public:
	enum class SceneType										//用枚举来表明当前的游戏状态
	{
		Menu,
		Game,
	};

public:
	SceneManager() = default;
	~SceneManager() = default;



	void set_current_scene(Scene* scene)						//设置当前的游戏场景
	{
		current_scene = scene;									//指针保存当前的状态
		current_scene->on_enter();								//更改游戏场景就要进入
	}

	void switch_to(SceneType type)								//用来执行场景之间的跳转逻辑
	{
		current_scene->on_exit();								//先退出当前的场景
		switch (type)
		{
		case SceneType::Menu:									//变更当前场景状态变量
			current_scene = menu_scene;
			break;
		case SceneType::Game:
			current_scene = game_scene;
			break;
		default:
			break;

		}

		current_scene->on_enter();								//进入新的场景
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
