#include <iostream> 
#include <cmath>
using namespace std;

#include <string.h>

//#define LOGI(...)
//#define LOGD(...)
//#define LOGE(...)

#define LOGI printf
#define LOGE printf
#define LOGD printf

#include <algorithm>
namespace CMImgProc
{
	// ˫���Բ�ֵͼ������,ע��:����ֻ�ı���,���ı�ͨ����
	// src			Դͼ��ָ��
	// sw,sh,sc		�ֱ��ʾԴͼ��Ŀ�,��,��ͨ����
	// dst			���ź�ͼ��ָ��
	// dw,dh		���ź�ͼ���,��
	void ResizeLinear(unsigned char *src, int sw, int sh, int sc, unsigned char *dst, int dw, int dh);

	/**
	* RGBA��ͼ��תΪRGBͼ��
	*/
	void RGBA2RGB(const unsigned char *src, unsigned char *dst, int w, int h);

	/**
	* ��ɫͼ��ת�Ҷ�
	* src:Դͼ��
	* dst:Ŀ��ͼ��
	* w:ͼ����
	* h:ͼ��߶�
	* sc:ͼ��ͨ����
	*/
	void RGBA2GRAY(const unsigned char *src, unsigned char *dst, int w, int h, int sc);
};
