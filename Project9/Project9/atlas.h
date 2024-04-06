#ifndef _ATLAS_H_
#define _ATLAS_H_
#include<vector>
#include<graphics.h>

class Atlas																				//��Ԫģʽ������ʾ��������Ҫ��ͼ������animation�в�ֳ����Ĳ���
{
public:
	Atlas() = default;

	~Atlas() = default;

	void add_image(const IMAGE& img)													//��ͼ����������е�ͼƬ�������������ˮƽ��ת����ͼ��ʱʮ������
	{
		frame_list.push_back(img);
	}

	IMAGE* get_image(int idx)															//�����ӦͼƬ��������������anmiation�����Ⱦ
	{
		if (idx < 0 || idx >= frame_list.size())
			return nullptr;

		return &frame_list[idx];
	}

	int get_size()																		//��ȡͼƬͼ��
	{
		return (int)frame_list.size();
	}

	void load_from_file(LPCTSTR path_template, int num)
	{
		frame_list.resize(num);															//vector��resize��ʾ�������������

		TCHAR path_file[256];
		for (int i = 0; i < num; i++)													//��ӦΪͼƬ�������й������Ը����������ѡ��forѭ��
		{
			_stprintf_s(path_file, path_template, i);									//i+1  Ӧ������Ű�
			loadimage(&frame_list[i], path_file);
		}
	}
public:
	std::vector<IMAGE>frame_list;

};

#endif // !_ATLAS_H_

