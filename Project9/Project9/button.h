#ifndef _BUTTON_H_
#define _BUTTON_H_
#include <graphics.h>
#include<string>												//��STRING����Ϊ��ƴ��ͼƬ·��


//////////////////////////////button
//��ť�Ĵ�����Ϊ�����ְ�ť�����������ǻ��֮࣬��Ҫ������
class Button
{
public:
	Button(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed)					//�԰�ť�����������ֵ�����ض�Ӧ��ͼƬ
	{
		region = rect;
		loadimage(&img_idle, path_img_idle);
		loadimage(&img_hovered, path_img_hovered);
		loadimage(&img_pushed, path_img_pushed);
	}

	~Button()
	{

	}

	void Draw()																									//ÿһ֡����Ҫ�������е�������ͼ��
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

	void ProcessEvent(const ExMessage& msg)																		//Exmessage��ʵ���ǽ�����Ϣ�ı�������
	{
		switch (msg.message)
		{
		case WM_MOUSEMOVE:
			if (status == Status::Idle && CheckCursorHit(msg.x, msg.y))											//cursor���
				status = Status::Hovered;
			else if (status == Status::Hovered && !CheckCursorHit(msg.x, msg.y))
				status = Status::Idle;
			break;
		case WM_LBUTTONDOWN:																					//left button down
			if (CheckCursorHit(msg.x, msg.y))
				status = Status::Pushed;
			break;
		case WM_LBUTTONUP:
			if (status == Status::Pushed)																		//ֻ�����������²�̧���ʱ��Żᴥ��onclick�߼�
				OnClick();
			break;
		default:break;
		}
	}

protected:
	virtual void OnClick() = 0;																					//���麯�� ,��Ӧ��ť�������߼�

private:
	enum class Status
	{
		Idle = 0,
		Hovered,
		Pushed
	};

private:
	RECT region;																								//���������Լ���λ�úʹ�С,rect�������
	IMAGE img_idle;																								//����״̬��ͼƬ
	IMAGE img_hovered;
	IMAGE img_pushed;
	Status status = Status::Idle;
private:
	//������λ����û�г����ڰ�ť����
	bool CheckCursorHit(int x, int y)
	{
		return x >= region.left && x <= region.right && y >= region.top && y <= region.bottom;
	}

};
#endif // !_BUTTON_H_

