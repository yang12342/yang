#ifndef _START_BUTTON_H
#define _START_BUTTON_H_
#include "button.h"
#include "scene_manager.h"


extern SceneManager scene_manager;
extern Scene* game_scene;
///////////////////////////////start button
//开始游戏的按钮
class StartGameButton :public Button
{
public:
	StartGameButton(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed)
		:Button(rect, path_img_idle, path_img_hovered, path_img_pushed) {}										//从这里我们也能看到关于派生类关于基类的构造问题，是先把变量给子类，再在冒号后面把基类的变量给他
	~StartGameButton() = default;

protected:
	void OnClick()
	{
		
		scene_manager.switch_to(SceneManager::SceneType::Game);
		mciSendString(_T("play bgm repeat from 0"), NULL, 0, NULL);												//播放bgm，并且从头循环播放,有repeat

	}

};
#endif // !_START_BUTTON_H

