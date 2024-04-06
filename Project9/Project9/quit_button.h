#ifndef _QUIT_BUTTON_H_
#define _QUIT_BUTTON_H_
#include "button.h"

extern bool running;

//ÍË³öÓÎÏ·°´Å¥
class QuitGameButton :public Button
{
public:
	QuitGameButton(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed)
		:Button(rect, path_img_idle, path_img_hovered, path_img_pushed) {}
	~QuitGameButton() = default;

protected:
	void OnClick()
	{
		running = false;
	}
};
#endif // !_QUIT_BUTTON_H_

