#include "CMImgProc.h"

#define CV_DESCALE(x,n) (((x) + (1 << ((n) - 1))) >> (n))



// ˫���Բ�ֵͼ������,ע��:����ֻ�ı���,���ı�ͨ����
// src			Դͼ��ָ��
// sw,sh,sc		�ֱ��ʾԴͼ��Ŀ�,��,��ͨ����
// dst			���ź�ͼ��ָ��
// dw,dh		���ź�ͼ���,��
void CMImgProc::ResizeLinear(unsigned char *src, int sw, int sh, int sc, unsigned char *dst, int dw, int dh)
{
	//LOGD("native ResizeLinear 1,sw=%d, sh=%d, dw=%d, dh=%d", sw, sh, dw, dh);
	unsigned char *dataDst = dst;
	int stepDst = dw * sc;
	unsigned char *dataSrc = src;
	int stepSrc = sw * sc;
	int iWidthSrc = sw;
	int iHiehgtSrc = sh;

	double scale_x = (double)sw / dw;
	double scale_y = (double)sh / dh;

	//LOGD("native ResizeLinear 2, scale_x=%f, scale_y=%f", scale_x, scale_y);

	for (int j = 0; j < dh; ++j)
	{
		float fy = (float)((j + 0.5) * scale_y - 0.5);
		int sy = (int)floor(fy);// ����ȡ��
		fy -= sy;
		sy = std::min(sy, iHiehgtSrc - 2);
		sy = std::max(0, sy);

		int cbufy[2];
		cbufy[0] = (int)((1.f - fy) * 2048 + 0.5);
		cbufy[1] = 2048 - cbufy[0];

		for (int i = 0; i < dw; ++i)
		{
			float fx = (float)((i + 0.5) * scale_x - 0.5);
			int sx = (int)floor(fx);
			fx -= sx;

			if (sx < 0) {
				fx = 0, sx = 0;
			}
			if (sx >= iWidthSrc - 1) {
				fx = 0, sx = iWidthSrc - 2;
			}

			short cbufx[2];
			cbufx[0] = (int)((1.f - fx) * 2048 + 0.5);
			cbufx[1] = 2048 - cbufx[0];

			// ��ÿ��ͨ�����м���
			for (int k = 0; k < sc; k++)
			{
				*(dataDst + j*stepDst + sc*i + k) = (*(dataSrc + sy*stepSrc + sc*sx + k) * cbufx[0] * cbufy[0] +
					*(dataSrc + (sy + 1)*stepSrc + sc*sx + k) * cbufx[0] * cbufy[1] +
					*(dataSrc + sy*stepSrc + sc*(sx + 1) + k) * cbufx[1] * cbufy[0] +
					*(dataSrc + (sy + 1)*stepSrc + sc*(sx + 1) + k) * cbufx[1] * cbufy[1]) >> 22;
			}


		}
	}
}

/**
* RGBA��ͼ��תΪRGBͼ��
*/
void CMImgProc::RGBA2RGB(const unsigned char *src, unsigned char *dst, int w, int h)
{
	//LOGD("native RGBA2GRAY 1......");

	const unsigned char *pSrc;
	unsigned char *pDst;
	for (int j = 0; j < h; j++)
	{
		pSrc = src + j * w * 4;
		pDst = dst + j * w * 3;
		for (int i = 0; i < w; i++)
		{
			pDst[2] = pSrc[2];
			pDst[1] = pSrc[1];
			pDst[0] = pSrc[0];

			pSrc += 4;
			pDst += 3;
		}
	}
}



// RGBA��ʽתGRAY,���ù�ʽ����
// Y = 0.299 * R + 0.587 * G + 0.114 * B
// Cr = (R - Y) * 0.713 + delta
// Cb = (B - Y) * 0.564 + delta
// src	Դ����,4ͨ��,RGBA
// dst	�������,3ͨ��,YCrCb
// w	ͼ��Ŀ��
// h	ͼ��ĸ߶�
// sc	Դͼ���ͨ����,һ��Ϊ4
void CMImgProc::RGBA2GRAY(const unsigned char *src, unsigned char *dst, int w, int h, int sc)
{
	LOGD("native RGBA2GRAY 1......\n");
	static int yuv_shift = 14;
	static int c0 = 4899, c1 = 9617, c2 = 1868;//R,G,B

	size_t len = strlen((char*)src);
	size_t t_sc = len / (w*h);
	//LOGD("native RGBA2GRAY 2.....,len=%d, w=%d, h=%d, sc=%d, t_sc=%d \n", len, w, h, sc, t_sc);

	const unsigned char *pSrc;
	unsigned char *pDst;
	for (int j = 0; j < h; j++)
	{
		pSrc = src + j * w * sc;
		pDst = dst + j * w;
		for (int i = 0; i < w; i++)
		{
			//int Y = CV_DESCALE(pSrc[2]*c2 + pSrc[1]*c1 + pSrc[0]*c0, yuv_shift);
			int Y = (pSrc[2] + pSrc[1] + pSrc[0]) / 3;
			*pDst = (unsigned char)(Y);

			pSrc += sc;
			pDst++;
		}
	}
	//LOGD("native RGBA2GRAY ok \n");
}

// RGBA��ʽתYCrCb
// Y = 0.299 * R + 0.587 * G + 0.114 * B
// Cr = (R - Y) * 0.713 + delta
// Cb = (B - Y) * 0.564 + delta
// src		Դ����,4ͨ��,RGBA
// dst		�������,3ͨ��,YCrCb
// w		ͼ��Ŀ��
// h		ͼ��ĸ߶�
// sc		Դͼ���ͨ����,һ��Ϊ4
// dc		���ͼ���ͨ����,Ϊ3ͨ��
void RGBA2YCrCb(const unsigned char *src, unsigned char *dst, int w, int h, int sc, int dc)
{

	static int yuv_shift = 14;
	static int c0 = 4899, c1 = 9617, c2 = 1868, c3 = 11682, c4 = 9241;//R,G,B
	static int value = 128 * (1 << 14);

	const unsigned char *pSrc;
	unsigned char *pDst;
	for (int j = 0; j < h; j++)
	{
		pSrc = src + j * w * sc;
		pDst = dst + j * w * dc;
		for (int i = 0; i < w; i++)
		{
			int Y = CV_DESCALE(pSrc[2] * c2 + pSrc[1] * c1 + pSrc[0] * c0, yuv_shift);
			int Cr = CV_DESCALE((pSrc[0] - Y)*c3 + value, yuv_shift);
			int Cb = CV_DESCALE((pSrc[2] - Y)*c4 + value, yuv_shift);

			pDst[0] = (unsigned char)(Y);
			pDst[1] = (unsigned char)(Cr);
			pDst[2] = (unsigned char)(Cb);

			pSrc += sc;
			pDst += dc;
		}
	}
}

// ֱ��ͼͳ��
// ��һ��
// ��ֵƽ��
// gray		����ͼ���ͷָ��,ע:ֻ��ԻҶ�ͼ,��ͨ����Ϊ1
// w,h		��ʾͼ��Ŀ�͸�
// hist		ֱ��ͼͳ�Ƶ�ͷָ��
// len		ֱ��ͼ����ĳ���,Ĭ��Ϊ256
void HistStat(unsigned char *gray, int w, int h, float *hist, int len)
{
	int count = w * h;

	unsigned char *pix = gray;
	for (int i = 0; i < count; i++)
	{
		hist[*(pix++)]++;
	}
	// ������ֱ��ͼ���й�һ����ͬʱ�����ֵ
	float ave = 0.0f;
	float *ph = hist;
	for (int m = 0; m < len; m++)
	{
		// ��һ��
		*ph /= count;
		ave += *ph;
		ph++;
	}
	ave /= len;

	// �����ݽ���ƽ�ƴ���
	ph = hist;
	for (int m = 0; m < len; m++)
	{
		*ph -= ave;
		ph++;
	}
}



// ����YCrCb������,��Ҫ��עY����
// src		Դͼ������
// lap		���ݵ�ͨ���Ľ������
// w		ͼ��Ŀ��
// h		ͼ��ĸ߶�
// sc		Դͼ���ͨ����
// Ĭ�ϲ��ú�Ϊ {2, 0, 2, 0, -8, 0, 2, 0, 2}
// ����ȥ�����ܵ��������صľ�ֵ
double Laplace(const unsigned char *src, unsigned char *lap, int w, int h, int sc)
{
	int val = 0;
	double ave = 0;
	memset(lap, 0, w*h*sizeof(unsigned char));

	const unsigned char *pcur, *plt, *prt, *plb, *prb;
	unsigned char lv;
	// ��Ե��������
	for (int j = 1; j < h - 1; j++)
	{
		pcur = src + (j * w + 1)* sc;// ��ǰ
		plt = pcur - (w + 1) * sc;// ����
		prt = pcur - (w - 1) * sc;// ����
		plb = pcur + (w - 1) * sc;// ����
		prb = pcur + (w + 1) * sc;// ����
		for (int i = 1; i < w - 1; i++)
		{
			// laplace�任
			val = (int)(*plt) * 2 + (int)(*prt) * 2 +
				(int)(*plb) * 2 + (int)(*prb) * 2 - (int)(*pcur) * 8;
			// ����ж�
			if (abs(val) > 255)
				lv = (unsigned char)(255);
			else
				lv = (unsigned char)(abs(val));

			lap[j * w + i] = lv;

			// ���
			ave += lv;

			pcur += sc;
			plt += sc;
			prt += sc;
			plb += sc;
			prb += sc;
		}
	}

	// ʾ������д��������,
	// ������Ͱ��մ������д,������ֵ��������
	// ʵ��Ӧд(ave/((h - 2)*(w - 2)))
	return (ave / ((h - 1)*(w - 1)));
}

