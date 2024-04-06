#ifndef _ATLAS_H_
#define _ATLAS_H_
#include<vector>
#include<graphics.h>

class Atlas																				//享元模式中来表示动画所需要的图集，从animation中拆分出来的部分
{
public:
	Atlas() = default;

	~Atlas() = default;

	void add_image(const IMAGE& img)													//向图集中添加已有的图片对象，这个在生成水平翻转动画图集时十分有用
	{
		frame_list.push_back(img);
	}

	IMAGE* get_image(int idx)															//获得相应图片的索引，有利于anmiation类的渲染
	{
		if (idx < 0 || idx >= frame_list.size())
			return nullptr;

		return &frame_list[idx];
	}

	int get_size()																		//获取图片图集
	{
		return (int)frame_list.size();
	}

	void load_from_file(LPCTSTR path_template, int num)
	{
		frame_list.resize(num);															//vector的resize表示调整数组的容量

		TCHAR path_file[256];
		for (int i = 0; i < num; i++)													//正应为图片命名很有规律所以根据最后的序号选择for循环
		{
			_stprintf_s(path_file, path_template, i);									//i+1  应该是序号吧
			loadimage(&frame_list[i], path_file);
		}
	}
public:
	std::vector<IMAGE>frame_list;

};

#endif // !_ATLAS_H_

