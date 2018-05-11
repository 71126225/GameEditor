#ifndef _BINARY_UTILITY_H_
#define _BINARY_UTILITY_H_

#include "CommonDefine.h"

class BinaryUtility
{
	static uint16_t const crc16_table[256];
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
	static unsigned short crc16(unsigned short crc, unsigned char* buffer, unsigned short len);
	static inline uint16_t crc16_byte(uint16_t crc, const uint8_t data)
	{
		return (crc >> 8) ^ crc16_table[(crc ^ data) & 0xff];
	}
	template<typename T>
	static void read(T& value, unsigned char* buffer, int& offset, bool inverse)
	{
		if (inverse)
		{
			int byteCount = sizeof(value);
			for (int i = 0; i < byteCount; ++i)
			{
				((char*)(&value))[i] = buffer[offset + byteCount - i - 1];
			}
		}
		else
		{
			memcpy(&value, buffer + offset, sizeof(value));
		}
		offset += sizeof(value);
	}
	static void readBuffer(unsigned char* dest, unsigned char* buffer, const int& count, int& offset)
	{
		memcpy(dest, buffer + offset, count);
		offset += count;
	}
	template<typename T>
	static void write(T& value, unsigned char* buffer, int& offset, bool inverse)
	{
		if (inverse)
		{
			int byteCount = sizeof(value);
			char* valueByte = (char*)(&value);
			for (int i = 0; i < byteCount; ++i)
			{
				buffer[offset + i] = valueByte[byteCount - i - 1];
			}
		}
		else
		{
			memcpy(buffer + offset, &value, sizeof(value));
		}
		offset += sizeof(value);
	}
};

#endif