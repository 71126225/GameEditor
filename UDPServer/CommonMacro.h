#pragma once

// ��󲢷�������Ϊ64,��Ҫ��winsock.h֮ǰ���ж���
#ifdef FD_SETSIZE
#undef FD_SETSIZE
#endif
#define FD_SETSIZE 64

// �������涨��Ĳ��ֺ�������ϵͳͷ�ļ��б�����,�Ӷ���ɱ����޷�ͨ��,�����Ȱ���ϵͳͷ�ļ�,Ȼ���ٶ����Լ��ĺ�
#ifdef WIN32
// ���Ӿ�̬��
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")
#ifdef _MYSQL
#pragma comment(lib, "libmysql.lib")
#endif
#pragma warning(disable: 4005)
// libevent��ͷ�ļ�ֻ����windows���ļ�֮ǰ����,������ж����ͻ�ı���
// ����ƽ̨δ��װlibevent,������Ҫʹ�ú����ж��Ƿ���Ҫ����libevent��ش���
#ifdef _LIBEVENT
#include "event2/event.h"
#include "event2/buffer.h"
#include "event2/http.h"
#endif
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <io.h>
#include <direct.h>
#include <winsock.h>
#include <tlhelp32.h>
#include <dbghelp.h>
#else
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/sysinfo.h>
#include <sys/un.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <stdarg.h>
#include <signal.h>
#include <dirent.h>
#include <pthread.h>
#include <locale.h>
#include <execinfo.h>
#ifdef _LIBEVENT
#include "event2/event.h"
#include "event2/buffer.h"
#include "event2/http.h"
#endif
#endif
#include <string>
#include <map>
#include <vector>
#include <set>
#include <list>
#include <stack>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <typeinfo>
#include <memory>
#include <time.h>
#include <cmath>
#include <math.h>
#include <sys/types.h>
#include <cctype>
#include <algorithm>
#include <assert.h>
#include <fcntl.h>
#include <atomic>
#include <functional>

#ifdef WIN32
#define MY_THREAD							HANDLE
#define MY_SOCKET							SOCKET
#define NULL_THREAD							nullptr
#define THREAD_CALLBACK_DECLEAR(func)		static DWORD WINAPI func(LPVOID args)
#define THREAD_CALLBACK(class, func)		DWORD WINAPI class##::##func(LPVOID args)
#define CREATE_THREAD(thread, func, args)	thread = CreateThread(nullptr, 0, func, args, 0, nullptr)
#define CLOSE_THREAD(thread)		\
if (thread != NULL_THREAD)			\
{									\
									\
	TerminateThread(thread, 0);		\
	CloseHandle(thread);			\
	thread = NULL_THREAD;			\
}
#define CLOSE_SOCKET(socket)						closesocket(socket);
#define SPRINTF(buffer, bufferSize, ...)			sprintf_s(buffer, bufferSize, __VA_ARGS__)
#define MEMCPY(dest, bufferSize, src, count)		memcpy_s((void*)(dest), bufferSize, (void*)(src), count)
// ��ȡ��ͬƽ̨�µ����泣���ַ�����UTF8�����ַ���,ֻ�ܴ������泣��,Ҳ�����ڴ�����д�����ַ���
// windows����Ҫ��GB2312ת��ΪUTF8,��linux��ֱ�Ӿ���UTF8��
// ��һ�����泣���ַ���ת��ΪUTF8��洢Ϊ����
#define UNIFIED_UTF8(var, size, constantString)		Array<size> var{ 0 }; StringUtility::ANSIToUTF8(constantString, var.toBuffer(), size, false)
#define UNIFIED_UTF8_STRING(str, constantString)	StringUtility::ANSIToUTF8(constantString, str, false)
#else
#define MY_THREAD							pthread_t
#define MY_SOCKET							unsigned int
#define NULL_THREAD							0
#define SOCKADDR_IN							sockaddr_in
#define THREAD_CALLBACK_DECLEAR(func)		static void* func(void* args)
#define THREAD_CALLBACK(class, func)		void* class##::##func(void* args)
#define CREATE_THREAD(thread, func, args)	pthread_create(&thread, nullptr, func, args);
#define CLOSE_THREAD(thread)	\
if (thread != NULL_THREAD)		\
{								\
	pthread_cancel(thread);		\
	thread = NULL_THREAD;		\
}
#define CLOSE_SOCKET(socket)				close(socket);
#ifdef __GNUC__
#define CSET_GBK							"GBK"
#define CSET_UTF8							"UTF-8"
#define LC_NAME_zh_CN						"zh_CN"
#endif
#define LC_NAME_zh_CN_GBK					LC_NAME_zh_CN "." CSET_GBK
#define LC_NAME_zh_CN_UTF8					LC_NAME_zh_CN "." CSET_UTF8
#define LC_NAME_zh_CN_DEFAULT				LC_NAME_zh_CN_GBK
#define SPRINTF(buffer, bufferSize, ...)	sprintf(buffer, __VA_ARGS__)
// ��ΪbufferSize��windows������Ҫ��,linux�²���Ҫ,����Ϊ�˱��⾯��,��Ȼʹ�ô˲�������,���ǲ������κ�Ӱ��
#define MEMCPY(dest, bufferSize, src, count) memcpy((void*)((char*)dest + (bufferSize) - (bufferSize)), (void*)(src), count)
// ��һ�����泣���ַ���ת��ΪUTF8��洢Ϊ����
#define UNIFIED_UTF8(var, size, constantString)		Array<size> var{ 0 };	var.setString(constantString)
#define UNIFIED_UTF8_STRING(str, constantString)	str = constantString
#endif

#ifdef ERROR
#undef ERROR
#define ERROR(info) cout << info << endl;
#endif

// ֻ���Ų���std������,���ⲻ��Ҫ��������ͻ
using std::vector;
using std::map;
using std::set;
using std::stack;
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::wstring;
using std::atomic_flag;
using std::make_pair;
using std::exception;
using std::move;
using std::is_same;
using std::decay;
using std::function;

//--------------------------------------------------------------------------------------------------------------------------------------------------------
// �����������ͼ򻯶���
typedef unsigned char byte;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned long long ullong;
typedef long long llong;