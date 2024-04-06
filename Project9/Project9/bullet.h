#ifndef _BULLET_H_
#define _BULLET_H_

#include <graphics.h>
#include<vector>												//���ڶ�̬���������ʱ����
#include<cmath>
/// <summary>
/// ////////////////////bullet
/// </summary>
class Bullet																						//�ӵ����ʵ��
{
public:
	POINT position = { 0,0 };
	Bullet() = default;																				//�����default��������
	~Bullet() = default;

	void Draw() const
	{
		setlinecolor(RGB(255, 155, 50));
		setfillcolor(RGB(200, 75, 10));
		fillcircle(position.x, position.y, RADIUS);													//�óȺ�ɫ���Բ�������ӵ�
	}

private:
	const int RADIUS = 10;


};

#endif // !_BULLET_H_

