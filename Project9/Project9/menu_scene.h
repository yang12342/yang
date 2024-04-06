#ifndef _MUEN_SCENE_H_
#define _MENU_SCENE_H_
#include "scene.h"
#include "button.h"

extern IMAGE img_menu;
extern StartGameButton btn_start_game;
extern QuitGameButton btn_quit_game;

class MenuScene :public Scene
{
public:
	MenuScene() = default;
	~MenuScene() = default;

	void on_enter()																//场景进入时的初始化
	{

	}

	void on_updata(int delta)
	{

	}																				//处理数据，根据玩家输入来整理更新数据


	void on_draw()
	{
		putimage(0, 0, &img_menu);
		btn_start_game.Draw();
		btn_quit_game.Draw();
		

	}																				//渲染绘图



	void on_input(const ExMessage& msg)											//场景菜单时玩家按下任意键都能进入游戏
	{
		btn_quit_game.ProcessEvent(msg);
		btn_start_game.ProcessEvent(msg);
	}																				//场景在处理玩家输入时调用


	void on_exit()
	{
		
	}																				//场景退出时的卸载逻辑






private:
	
};


#endif // !_MENU_SCENE_H_
