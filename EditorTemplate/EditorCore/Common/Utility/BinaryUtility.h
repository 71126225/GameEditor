#ifndef _BINARY_UTILITY_H_
#define _BINARY_UTILITY_H_

#include "CommonDefine.h"

class BinaryUtility
{
public:
	// ��β����һ���ڴ�,width���ڴ淽��Ŀ��,ÿ�ŵ��ڴ������,height���ֽڸ߶�,blockByte��ÿ���ڴ���ֽ���
	static void swapPixelBuffer(char* buffer, int width, int height, int blockByte);
	static void upsideDownBuffer(char* buffer, int bufferWidth, int startX, int startY, int width, int height, int blockByte);
	// YUVתRGBA,��Χ����0-255
	static void YUVToRGBA(char* YUVData, int width, int height, char* RGBAData);
	// RGNAתYUV,��Χ����0-255
	static void RGBAToYUV(char* RGBAData, int width, int height, char* YUVData);
	// YUV420Pת��ΪRGB24
	static void YUV420PToRGB32(unsigned char* yuv420[3], unsigned char* rgb32, int width, int height);
	// ���� 16���Ƶ�c��1�ĸ���
	static int crc_check(char c);
};

#endif