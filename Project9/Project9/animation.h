#ifndef _ANIMATION_H_
#define _ANIMATION_H_
#include"atlas.h"
#include"util.h"



class Animation																				//为解决相同的左右播放问题，制作新的类
{
private:
	int interval_ms = 0;																	//这里是帧间隔 私有数据成员
	int timer = 0;																			//这是计时器不再是计数器了，通过计时器来判断是否刷新图像
	int idx_frame = 0;																		//动画帧索引，也就是图片序号

private:
	Atlas* anim_atlas;

public:

	Animation(Atlas* atlas, int interval)													//这个不就是构造函数嘛
	{
		anim_atlas = atlas;
		interval_ms = interval;																//这里之前忘了把这个设置的间隔给赋值上去
	}

	~Animation() = default;																	//这段代码是C++中的写法，表示为名为Animation的类定义了一个默认析构函数。使用= default告诉编译器生成默认的析构函数。这里的动态数组的析构应该放在更上层的代码中去

	void Play(int x, int y, int delta)
	{

		timer += delta;																		//帧索引的跟换，也就是计时刷新逻辑
		if (timer >= interval_ms)
		{

			idx_frame = (idx_frame + 1) % anim_atlas->frame_list.size();					//这里是序号的更换 
			timer = timer-interval_ms;														//这样就更流畅了//其实就是计算上个图片刷新后的时间间隔是否达到了我设置界的间隔，达到了就下一个，没有就还刷新那个图

		}

		putimage_alpha(x, y, anim_atlas->get_image(idx_frame));								//这里就是绘图了
	}
};
#endif // !_ANIMATION_H_

