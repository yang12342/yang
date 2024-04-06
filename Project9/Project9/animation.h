#ifndef _ANIMATION_H_
#define _ANIMATION_H_
#include"atlas.h"
#include"util.h"



class Animation																				//Ϊ�����ͬ�����Ҳ������⣬�����µ���
{
private:
	int interval_ms = 0;																	//������֡��� ˽�����ݳ�Ա
	int timer = 0;																			//���Ǽ�ʱ�������Ǽ������ˣ�ͨ����ʱ�����ж��Ƿ�ˢ��ͼ��
	int idx_frame = 0;																		//����֡������Ҳ����ͼƬ���

private:
	Atlas* anim_atlas;

public:

	Animation(Atlas* atlas, int interval)													//��������ǹ��캯����
	{
		anim_atlas = atlas;
		interval_ms = interval;																//����֮ǰ���˰�������õļ������ֵ��ȥ
	}

	~Animation() = default;																	//��δ�����C++�е�д������ʾΪ��ΪAnimation���ඨ����һ��Ĭ������������ʹ��= default���߱���������Ĭ�ϵ���������������Ķ�̬���������Ӧ�÷��ڸ��ϲ�Ĵ�����ȥ

	void Play(int x, int y, int delta)
	{

		timer += delta;																		//֡�����ĸ�����Ҳ���Ǽ�ʱˢ���߼�
		if (timer >= interval_ms)
		{

			idx_frame = (idx_frame + 1) % anim_atlas->frame_list.size();					//��������ŵĸ��� 
			timer = timer-interval_ms;														//�����͸�������//��ʵ���Ǽ����ϸ�ͼƬˢ�º��ʱ�����Ƿ�ﵽ�������ý�ļ�����ﵽ�˾���һ����û�оͻ�ˢ���Ǹ�ͼ

		}

		putimage_alpha(x, y, anim_atlas->get_image(idx_frame));								//������ǻ�ͼ��
	}
};
#endif // !_ANIMATION_H_

