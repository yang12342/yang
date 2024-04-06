#ifndef _BULLET_H_
#define _BULLET_H_

#include <graphics.h>
#include<vector>												//用于动态数组可以随时扩容
#include<cmath>
/// <summary>
/// ////////////////////bullet
/// </summary>
class Bullet																						//子弹类的实现
{
public:
	POINT position = { 0,0 };
	Bullet() = default;																				//这里的default看不懂嘞
	~Bullet() = default;

	void Draw() const
	{
		setlinecolor(RGB(255, 155, 50));
		setfillcolor(RGB(200, 75, 10));
		fillcircle(position.x, position.y, RADIUS);													//用橙红色填充圆来绘制子弹
	}

private:
	const int RADIUS = 10;


};

#endif // !_BULLET_H_

