#ifndef _START_BUTTON_H
#define _START_BUTTON_H_
#include "button.h"
#include "scene_manager.h"


extern SceneManager scene_manager;
extern Scene* game_scene;
///////////////////////////////start button
//��ʼ��Ϸ�İ�ť
class StartGameButton :public Button
{
public:
	StartGameButton(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed)
		:Button(rect, path_img_idle, path_img_hovered, path_img_pushed) {}										//����������Ҳ�ܿ���������������ڻ���Ĺ������⣬���Ȱѱ��������࣬����ð�ź���ѻ���ı�������
	~StartGameButton() = default;

protected:
	void OnClick()
	{
		
		scene_manager.switch_to(SceneManager::SceneType::Game);
		mciSendString(_T("play bgm repeat from 0"), NULL, 0, NULL);												//����bgm�����Ҵ�ͷѭ������,��repeat

	}

};
#endif // !_START_BUTTON_H

