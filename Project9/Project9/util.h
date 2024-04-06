#ifndef _UTIL_H_
#define _UTIL_H_
#include<graphics.h>

inline void sketch_done_image(IMAGE* src, IMAGE* dst);
inline void flip_image(IMAGE* src, IMAGE* dst);
void flip_atlas(Atlas& src, Atlas& dst);
inline void putimage_alpha(int x, int y, IMAGE* img);

inline void flip_image(IMAGE* src, IMAGE* dst)																//����ˮƽͼƬ�ķ�ת���� ��src�Ƿ�תǰ��ԴͼƬ�ʹ洢��ת������Ŀ��ͼƬ
{
	int w = src->getwidth();																				//getwidth����IMAGE���һ�������ǻ�ȡͼƬ�ĵĿ��
	int h = src->getheight();
	Resize(dst, w, h);																						//��Ϊһ��ʼ��IMAGE������û�л���ռ�ģ�����δ���صķ�תͼƬ����Ӧ�������ڲ��ռ�����ȡ��Ӧ�Ļ���ռ�
																											//ĳ�������Ͽ������������һ��������֮�������滭��
	DWORD* src_buffer = GetImageBuffer(src);																//dword�������ͳ�������windows���������ַ����ָ�룬double word һ��word2�ֽڣ���Ӧ�Ĵ���ռ䲻��int���ű��������ı�
	DWORD* dst_buffer = GetImageBuffer(dst);

	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)																			//����ԴͼƬ�ĵ�ÿһ�����ص��ͼ��������ҷ�ת
		{
			int idx_src = y * w + x;																		//���˿�ͷ���������Ͷ�Ӧ�ռ�Ĵ���֪ʶ�����Ǿ��ܹ��ܺõĻ��ÿһ��ͼƬ���ص����������и���
			int idx_dst = y * w + (w - x - 1);																//��ͼƬ�������ҷ�ת������y��Ķ���������Ȼ�������Ҷ�Ӧ����ѧ�߼����и���
			dst_buffer[idx_dst] = src_buffer[idx_src];														//�������Ӧ�ĸ��Ʋ���
		}
	}
}

///��תutil/////////////////////////////////
void flip_atlas(Atlas& src, Atlas& dst)														//��������ȫ����ת
{
	for (int i = 0; i < src.get_size(); i++)
	{
		IMAGE img_flpipped;
		flip_image(src.get_image(i), &img_flpipped);										//�������Ƿ���flipimage�Ķ�����IMAGE����ÿ��IMAGE
		dst.add_image(img_flpipped);														//��������Ҫ����һ�㣬vector��pushback������Ƕ��󸱱�Ϊ���Ǳ�����Ȼ����Ķ����������ڽ���ֻ�����ѭ���ڣ���Ҳ������Ϊʲô����˷��Ĵ󵨵�ʹ��add

	}
}

void sketch_atlas(Atlas& src, Atlas& dst)													//src��ʾsourceԭ�ļ���dst��֪��
{
	for (int i = 0; i < src.get_size(); i++)
	{
		IMAGE img_sketch;
		sketch_done_image(src.get_image(i), &img_sketch);
		dst.add_image(img_sketch);
	}
}


inline void sketch_done_image(IMAGE* src, IMAGE* dst)														//���׼�Ӱ���д���
{
	int w = src->getwidth();
	int h = src->getheight();
	Resize(dst, w, h);
	//����ͼƬɫ�ʻ���������͸��������Ϊ��ɫ
	DWORD* color_buffer_raw_img = GetImageBuffer(src);														//���ԴͼƬ������ɫ�ʻ�������ͼƬ��������Ҫ��Ϊ���ڱ���������ȷ����͸��ͨ����ֵ
	DWORD* color_buffer_sketch_img = GetImageBuffer(dst);													//��ü�Ӱ������ɫ�ʻ�������ͼƬ����
	for(int y=0;y<h;y++)																					//����ÿһ������
	{
		for (int x = 0; x < w; x++)
		{
			int idx = y * w + x;																			//�����漰�����ǹ���ɫ�ʻ������Ĵ洢���⣬�����ϵĶ�λ���飬ʵ�ʵ�һά����洢��ʽ
			if ((color_buffer_raw_img[idx] & 0xFF000000) >> 24)												//�������ÿ�����ص����ֽ��ڴ���ÿ���ֽڷֱ�洢��͸��ͨ����RGB������ɫ0-255����ֵ
																											//�����ֵ��Ӽ�����������֪��16����ת�����ƣ�ÿ��16��������Ҫ��λ������������ʾ���˽�����Ҫ��λ����������ʾ
																											//����������͸��ͨ����ֵ�������ֽڵĵ�һ�ֽڣ�0X��ʾ16���ƣ�F��ʾ15������F��ʾ11111111���ð˸����أ�һ���ֽڣ����������������߼����ţ�ֻ��͸��ͨ����ֵ�����ٽ���λ���㣬����ȡ����ɫ�ʵ�ֵҲ��ͬ���ĵ���
			{
				color_buffer_sketch_img[idx] = BGR(RGB(255, 255, 255)) | (((DWORD)(BYTE)(255)) << 24);		//����Ϳ����������Ƕ��������������ݣ�����ǰ����ɫ�ʣ������Ǳ�ʾ��͸����255��
																											//ͻȻ����|����ǻ����㣬���ְ�͸��ͨ����������24λ��Ȼ��ȡ�����㣬���������ֽ��Ұ���������Ϣ

			}
		}
	}


}
/*���ص�ͼƬ������Ȼ��ʽ���Ǹ���ά���飬����ʵ���Ͼ���һά���飬IMAGE���������ÿ���������Ϳռ�ռ�ĸ��ֽڣ��ֱ𴢴���͸��ͨ����R��G��B�ĸ���Ϣ�����ݷ�Χ�ֱ���0��255��
����ʵ���Ͽ���ͨ��λ��������ȡ������Ҫ��ֵ��������ͬ������ʹ��getvalue����ȥ���������Ҫ��ֵ�����������ȡ�����һbyte��ֵ������ָ��getvalueԭ���������colorref�������ͣ�����������������ݵĴ洢��ʽ��BGR
��������Ҫ����ȥȡ��������ɫ�ʻ���������ֵ�ͱ���֪������Ҫ��ȡR����ɫ���ͱ���ʹ��GetBValue����Ϊ�ں��������Bλ�ò����������ĺ�ɫλ��*/


#pragma comment(lib,"MSIMG32.LIB")												//͸��ͨ���������ʵ��͸���ĸ��ǡ����ܵ���˵�д�ѧϰ//��ʵ���ǿ��Բο��ĵ���image��һϵ������
																				//�����õ���AlphaBlend��������������⣬����͸��������������

inline void putimage_alpha(int x, int y, IMAGE* img)							//inline��������������־������һ������������ͼƬ����
{
	int w = img->getwidth();
	int h = img->getheight();													//����IMAGE����ĳ�Ա����
	AlphaBlend(GetImageHDC(NULL), x, y, w, h,
		GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER,0,255,AC_SRC_ALPHA });		//�����ճ�������ܴ�����ҵ�͸����������
}
#endif // !_UTIL_H_
