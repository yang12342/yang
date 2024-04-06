#ifndef _BUTTON_H_
#define _BUTTON_H_
#include <graphics.h>
#include<string>												//用STRING库是为了拼凑图片路径


//////////////////////////////button
//按钮的处理，因为是俩种按钮，所以这里是基类，之后还要做派生
class Button
{
public:
	Button(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed)					//对按钮的区域变量赋值并加载对应的图片
	{
		region = rect;
		loadimage(&img_idle, path_img_idle);
		loadimage(&img_hovered, path_img_hovered);
		loadimage(&img_pushed, path_img_pushed);
	}

	~Button()
	{

	}

	void Draw()																									//每一帧都需要根据现有的来绘制图像
	{
		switch (status)
		{
		case Status::Idle:
			putimage(region.left, region.top, &img_idle);
			break;
		case Status::Hovered:
			putimage(region.left, region.top, &img_idle);
		case Status::Pushed:
			putimage(region.left, region.top, &img_pushed);
		}
	}

	void ProcessEvent(const ExMessage& msg)																		//Exmessage其实就是交互信息的变量类型
	{
		switch (msg.message)
		{
		case WM_MOUSEMOVE:
			if (status == Status::Idle && CheckCursorHit(msg.x, msg.y))											//cursor光标
				status = Status::Hovered;
			else if (status == Status::Hovered && !CheckCursorHit(msg.x, msg.y))
				status = Status::Idle;
			break;
		case WM_LBUTTONDOWN:																					//left button down
			if (CheckCursorHit(msg.x, msg.y))
				status = Status::Pushed;
			break;
		case WM_LBUTTONUP:
			if (status == Status::Pushed)																		//只有玩家左键按下并抬起的时候才会触发onclick逻辑
				OnClick();
			break;
		default:break;
		}
	}

protected:
	virtual void OnClick() = 0;																					//纯虚函数 ,对应按钮的运行逻辑

private:
	enum class Status
	{
		Idle = 0,
		Hovered,
		Pushed
	};

private:
	RECT region;																								//用于描述自己的位置和大小,rect这个类型
	IMAGE img_idle;																								//三个状态的图片
	IMAGE img_hovered;
	IMAGE img_pushed;
	Status status = Status::Idle;
private:
	//检测鼠标位置有没有出现在按钮区域
	bool CheckCursorHit(int x, int y)
	{
		return x >= region.left && x <= region.right && y >= region.top && y <= region.bottom;
	}

};
#endif // !_BUTTON_H_

