#ifndef _TX_BINARY_UTILITY_H_
#define _TX_BINARY_UTILITY_H_

#include "txEngineDefine.h"

class txBinaryUtility
{
public:
	template<typename T>
	static void inverseByte(T& value)
	{
		int typeSize = sizeof(T);
		for (int i = 0; i < typeSize / 2; ++i)
		{
			swapByte(value, i, typeSize - i - 1);
		}
	}
	template<typename T>
	static void swapByte(T& value, const int& pos0, const int& pos1)
	{
		char byte0 = (value & (0xff << (8 * pos0))) >> (8 * pos0);
		char byte1 = (value & (0xff << (8 * pos1))) >> (8 * pos1);
		SET_BYTE(value, byte0, pos1);
		SET_BYTE(value, byte1, pos0);
	}

	// ��β����һ���ڴ�,width���ڴ淽��Ŀ��,ÿ�ŵ��ڴ������,height���ֽڸ߶�,blockByte��ÿ���ڴ���ֽ���
	static void swapPixelBuffer(char* buffer, int width, int height, int blockByte);
	static void swapColourByte(char* buffer, int width, int height, int blockByte, int exchangeIndex0, int exchangeIndex1);
	static void upsideDownBuffer(char* buffer, int bufferWidth, int startX, int startY, int width, int height, int blockByte);
	// YUVתRGBA,��Χ����0-255
	static void YUVToRGBA(char* YUVData, int width, int height, char* RGBAData);
	// RGNAתYUV,��Χ����0-255
	static void RGBAToYUV(char* RGBAData, int width, int height, char* YUVData);
	// YUV420Pת��ΪRGB24
	static void YUV420PToRGB32(unsigned char* yuv420[3], unsigned char* rgb32, int width, int height);
	// ����16���Ƶ�c��1�ĸ���
	static int checkCRC(char c);
	// �Ƚ��ڴ������Ƿ���ͬ,lengthΪ�Ƚϵĳ���
	static bool memoryCompare(char* buffer0, char* buffer1, const int& length);
};

#endif