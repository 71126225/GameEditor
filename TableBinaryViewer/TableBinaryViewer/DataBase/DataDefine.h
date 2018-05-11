#ifndef _DATA_DEFINE_H_
#define _DATA_DEFINE_H_

#include <windows.h>
#include <io.h>
#include <direct.h>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <typeinfo>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

const std::string GAME_DATA_PATH = "GameDataFile/";
const std::string DATA_TEMPLATE_PATH = "DataTemplate/";
const std::string TEMPLATE_SUFFIX = ".txt";
const std::string DATA_SUFFIX = ".bytes";
const std::string EMPTY_STRING = "";

#define TX_MAX(x, y) ((x) > (y) ? (x) : (y))
#define TX_MIN(x, y) ((x) < (y) ? (x) : (y))

#define TOSTRING(t) #t

// �����޲λ��ߴ��ι�������ڴ�
#define TRACE_NEW(className, ptr, ...)  NULL;\
	ptr = new className(__VA_ARGS__);

// �����޲ι��������߻����������������ڴ�
#define TRACE_NEW_ARRAY(className, count, ptr)  \
NULL;											\
if (count <= 0)									\
{												\
	ptr = NULL;									\
}												\
else											\
{												\
	ptr = new className[count];					\
	memset(ptr, 0, count * sizeof(className));	\
}

// �ͷ�TRACE_NEW������ڴ�
#define TRACE_DELETE(ptr)	\
if (ptr != NULL)			\
{							\
	delete ptr;				\
	ptr = NULL;				\
}

// �ͷ�TRACE_NEW_ARRAY������ڴ�
#define TRACE_DELETE_ARRAY(ptr) \
if (ptr != NULL)				\
{								\
	delete[] ptr;				\
	ptr = NULL;					\
}

const int DROP_LINE = 3;

typedef void(*errorMessage)(const std::string& str);
#define ENGINE_ERROR(str)				\
if (DataBase::mErrorCallback != NULL)	\
{										\
	DataBase::mErrorCallback(str);		\
}

#endif