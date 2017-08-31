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
	// 双线性插值图像缩放,注意:缩放只改变宽高,不改变通道数
	// src			源图像指针
	// sw,sh,sc		分别表示源图像的宽,高,及通道数
	// dst			缩放后图像指针
	// dw,dh		缩放后图像宽,高
	void ResizeLinear(unsigned char *src, int sw, int sh, int sc, unsigned char *dst, int dw, int dh);

	/**
	* RGBA的图像转为RGB图像
	*/
	void RGBA2RGB(const unsigned char *src, unsigned char *dst, int w, int h);

	/**
	* 彩色图像转灰度
	* src:源图像
	* dst:目标图像
	* w:图像宽度
	* h:图像高度
	* sc:图像通道数
	*/
	void RGBA2GRAY(const unsigned char *src, unsigned char *dst, int w, int h, int sc);
};
