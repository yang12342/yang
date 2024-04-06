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

	void on_enter()																//��������ʱ�ĳ�ʼ��
	{

	}

	void on_updata(int delta)
	{

	}																				//�������ݣ�������������������������


	void on_draw()
	{
		putimage(0, 0, &img_menu);
		btn_start_game.Draw();
		btn_quit_game.Draw();
		

	}																				//��Ⱦ��ͼ



	void on_input(const ExMessage& msg)											//�����˵�ʱ��Ұ�����������ܽ�����Ϸ
	{
		btn_quit_game.ProcessEvent(msg);
		btn_start_game.ProcessEvent(msg);
	}																				//�����ڴ����������ʱ����


	void on_exit()
	{
		
	}																				//�����˳�ʱ��ж���߼�






private:
	
};


#endif // !_MENU_SCENE_H_
