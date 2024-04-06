#ifndef _UTIL_H_
#define _UTIL_H_
#include<graphics.h>

inline void sketch_done_image(IMAGE* src, IMAGE* dst);
inline void flip_image(IMAGE* src, IMAGE* dst);
void flip_atlas(Atlas& src, Atlas& dst);
inline void putimage_alpha(int x, int y, IMAGE* img);

inline void flip_image(IMAGE* src, IMAGE* dst)																//用于水平图片的翻转问题 ，src是翻转前的源图片和存储翻转后结果的目标图片
{
	int w = src->getwidth();																				//getwidth就是IMAGE类的一个函数是获取图片的的宽度
	int h = src->getheight();
	Resize(dst, w, h);																						//因为一开始的IMAGE数据是没有缓存空间的，所以未加载的翻转图片对象，应该设置内部空间来获取相应的缓存空间
																											//某个层面上看这就像是先拿一个画布，之后在上面画画
	DWORD* src_buffer = GetImageBuffer(src);																//dword数据类型常用来在windows下来保存地址或存放指针，double word 一个word2字节，对应的储存空间不想int随着编译器而改变
	DWORD* dst_buffer = GetImageBuffer(dst);

	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)																			//遍历源图片的的每一个像素点对图像进行左右翻转
		{
			int idx_src = y * w + x;																		//有了开头的索引，和对应空间的储存知识，我们就能够很好的获得每一个图片像素的索引并进行复制
			int idx_dst = y * w + (w - x - 1);																//让图片关于左右翻转，就是y轴的东西不动，然后用左右对应的数学逻辑进行复制
			dst_buffer[idx_dst] = src_buffer[idx_src];														//这就是相应的复制步骤
		}
	}
}

///翻转util/////////////////////////////////
void flip_atlas(Atlas& src, Atlas& dst)														//动画集的全部翻转
{
	for (int i = 0; i < src.get_size(); i++)
	{
		IMAGE img_flpipped;
		flip_image(src.get_image(i), &img_flpipped);										//这里我们发现flipimage的对象是IMAGE，对每个IMAGE
		dst.add_image(img_flpipped);														//这里我们要明白一点，vector的pushback放入的是对象副本为不是本身，不然这里的对象生命周期仅仅只在这个循环内，这也是我们为什么会如此放心大胆的使用add

	}
}

void sketch_atlas(Atlas& src, Atlas& dst)													//src表示source原文件，dst不知道
{
	for (int i = 0; i < src.get_size(); i++)
	{
		IMAGE img_sketch;
		sketch_done_image(src.get_image(i), &img_sketch);
		dst.add_image(img_sketch);
	}
}


inline void sketch_done_image(IMAGE* src, IMAGE* dst)														//纯白剪影序列处理
{
	int w = src->getwidth();
	int h = src->getheight();
	Resize(dst, w, h);
	//遍历图片色彩缓冲区将不透明的区域换为白色
	DWORD* color_buffer_raw_img = GetImageBuffer(src);														//获得源图片的像素色彩缓冲区的图片索引，主要是为了在遍历像素中确认其透明通道的值
	DWORD* color_buffer_sketch_img = GetImageBuffer(dst);													//获得剪影的像素色彩缓冲区的图片索引
	for(int y=0;y<h;y++)																					//遍历每一个像素
	{
		for (int x = 0; x < w; x++)
		{
			int idx = y * w + x;																			//这里涉及到我们关于色彩缓冲区的存储问题，表面上的二位数组，实际的一维数组存储方式
			if ((color_buffer_raw_img[idx] & 0xFF000000) >> 24)												//这里关于每个像素的四字节内存中每个字节分别存储着透明通道，RGB三种颜色0-255的数值
																											//在数字电子技术里面我们知道16进制转二进制，每个16进制数需要四位二进制数来表示，八进制需要三位二进制数表示
																											//而在在这里透明通道的值就在四字节的第一字节，0X表示16进制，F表示15，俩个F表示11111111正好八个比特，一个字节，我们再用与运算逻辑符号，只将透明通道的值留下再进行位运算，我们取其他色彩的值也是同样的道理
			{
				color_buffer_sketch_img[idx] = BGR(RGB(255, 255, 255)) | (((DWORD)(BYTE)(255)) << 24);		//这里就看不懂好像是对数据有俩个备份，反正前面是色彩，后面是表示不透明，255嘛
																											//突然发现|这个是或运算，他又把透明通道向左移了24位，然后取或运算，正好是四字节且包含完整信息

			}
		}
	}


}
/*像素的图片储存虽然形式上是个二维数组，但是实际上就是一维数组，IMAGE类型数组的每个数据类型空间占四个字节，分别储存着透明通道，R，G，B四个信息，数据范围分别在0到255，
我们实际上可以通过位运算来获取我们想要的值，但我们同样可以使用getvalue函数去获得我们想要的值（这个函数获取真好是一byte的值），但指导getvalue原本处理的是colorref数据类型，而这个数据类型数据的存储方式是BGR
所以我们要用它去取我们像素色彩缓冲区的数值就必须知道，我要获取R的颜色，就必须使用GetBValue，因为在函数眼里的B位置才是你真正的红色位置*/


#pragma comment(lib,"MSIMG32.LIB")												//透明通道混叠，来实现透明的覆盖——总的来说有待学习//其实还是可以参考文档的image的一系列内容
																				//下面用到的AlphaBlend函数就来自这个库，就是透明巴拉巴拉处理

inline void putimage_alpha(int x, int y, IMAGE* img)							//inline不是内联函数标志吗，这是一个函数，用于图片处理
{
	int w = img->getwidth();
	int h = img->getheight();													//都是IMAGE里面的成员函数
	AlphaBlend(GetImageHDC(NULL), x, y, w, h,
		GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER,0,255,AC_SRC_ALPHA });		//反正照抄这个就能处理玩家的透明背景问题
}
#endif // !_UTIL_H_
