///<------------------------------------------------------------------------------
//< @file:   server\def\platform.h
//< @author: 洪坤安
//< @date:   2014年11月25日 15:35:3
//< @brief:  平台相关的定义
//< Copyright (c) 2014 Tokit. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _platform_h_
#define _platform_h_

#include <assert.h>
#include <stdio.h>

/*
#ifdef WIN
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#undef WIN32_LEAN_AND_MEAN
	#include <io.h>
	#include <tchar.h>
#else
	#include<pthread.h>
#endif
*/

// 当前处于哪个平台 ?
#if !defined(PLATFORM)
	#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
		#define WIN
	#else
		#define POSIX
	#endif
	#define PLATFORM
#endif

// 平台相关的头文件包含
#if defined(WIN)
	#ifndef WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN
		#define __UNDEF_LEAN_AND_MEAN
	#endif

	#include <windows.h>

	#ifdef __UNDEF_LEAN_AND_MEAN
		#undef WIN32_LEAN_AND_MEAN
		#undef __UNDEF_LEAN_AND_MEAN
	#endif
#else
	#include <pthread.h>
	#include <signal.h>
	#include <sched.h>
	#include <unistd.h>

	#ifndef INT8_MAX
		#define INT8_MAX	0x7f
		#define INT16_MAX	0x7fff
		#define INT32_MAX	0x7fffffff
		#define INT64_MAX	0x7fffffffffffffff
		#define UINT8_MAX	0xff
		#define UINT16_MAX	0xffff
		#define UINT32_MAX	0xffffffff
		#define UINT64_MAX	0xffffffffffffffffU
	#endif
#endif

#ifdef WIN
	// 屏蔽protobuf警告
	#pragma warning( disable : 4996 )
#endif


// 线程、锁
#ifdef WIN
	#include <process.h>
	#include <sys/locking.h>
#else
	#include <pthread.h>

	#define nullptr NULL
#endif

// 套接字、网络
#ifdef WIN
	#define FD_SETSIZE 1024

	#include <winsock2.h>
	#include <ws2tcpip.h>
	#include <io.h>

	#define SHUT_RD SD_RECEIVE
	#define SHUT_WR SD_SEND
	#define SHUT_RDWR SD_BOTH
#else
	#include <netinet/in.h>
	#include <sys/socket.h>
	#include <sys/epoll.h>
	#include <sys/uio.h>
	#include <arpa/inet.h>
#endif

// 函数调用
#ifdef WIN
	#define fstat _fstati64
	#define stat _stati64
	#define mode_t int
	#define sleep Sleep
	#define bzero(buf, len) memset(buf, 0, len)
#else
	#define sprintf_s snprintf
	#define vsprintf_s vsnprintf
	#define sleep(ms) usleep(ms * 1000)
#endif

// 编译器是否支持c++11的全部特性 ?
#if (__cplusplus > 199711L) || (defined(__STDCXX_VERSION__) && (__STDCXX_VERSION__ >= 201001L))
	#define _CPP11_
#endif

// 线程局部存储
#if !defined(_TTHREAD_CPP11_) && !defined(thread_local)
	#if defined(__clang__)
		// clang编译器已实现thread_local，若重新#define将导致编译错误
	#elif defined(__GNUC__) || defined(__INTEL_COMPILER) || defined(__SUNPRO_CC) || defined(__IBMCPP__)
		#define thread_local __thread
	#else
		#define thread_local __declspec(thread)
	#endif
#endif

// 公用头文件
#include <string>
#include <list>
#include <vector>
#include <map>
#include <iterator>
#include <algorithm>

// 本项目公用头文件
#include <basic/singleton.h>
#include <basic/log.h>

// unordered_map、unordered_set头文件路径定义
#ifdef WIN
	#include <unordered_map> // 如果对map的有序性不作要求的话，请尽量用unorder_map代替map
	#include <unordered_set>
#else
	#include <tr1/unordered_map>
	#include <tr1/unordered_set>
#endif

using namespace std;

// libcurl预定义
#define CURL_STATICLIB
#define HTTP_ONLY

// 套接字、网络
#ifdef WIN
	#define NetModel Select
	class Select;

	#define MSG_NOSIGNAL 0
#else
	#define NetModel Epoll
	class Epoll;

	#define SOCKET_ERROR (-1)
#endif

// 错误码重定义
#ifdef WIN
	#undef EWOULDBLOCK
	#undef EINPROGRESS
	#undef EALREADY
	#undef ENOTSOCK
	#undef EDESTADDRREQ
	#undef EMSGSIZE
	#undef EPROTOTYPE
	#undef ENOPROTOOPT
	#undef EPROTONOSUPPORT
	#undef ESOCKTNOSUPPORT
	#undef EOPNOTSUPP
	#undef EPFNOSUPPORT
	#undef EAFNOSUPPORT
	#undef EADDRINUSE
	#undef EADDRNOTAVAIL
	#undef ENETDOWN
	#undef ENETUNREACH
	#undef ENETRESET
	#undef ECONNABORTED
	#undef ECONNRESET
	#undef ENOBUFS
	#undef EISCONN
	#undef ENOTCONN
	#undef ESHUTDOWN
	#undef ETOOMANYREFS
	#undef ETIMEDOUT
	#undef ECONNREFUSED
	#undef ELOOP
	#undef ENAMETOOLONG
	#undef EHOSTDOWN
	#undef EHOSTUNREACH
	#undef ENOTEMPTY
	#undef EPROCLIM
	#undef EUSERS
	#undef EDQUOT
	#undef ESTALE
	#undef EREMOTE

	#define EWOULDBLOCK             WSAEWOULDBLOCK
	#define EINPROGRESS             WSAEINPROGRESS
	#define EALREADY                WSAEALREADY
	#define ENOTSOCK                WSAENOTSOCK
	#define EDESTADDRREQ            WSAEDESTADDRREQ
	#define EMSGSIZE                WSAEMSGSIZE
	#define EPROTOTYPE              WSAEPROTOTYPE
	#define ENOPROTOOPT             WSAENOPROTOOPT
	#define EPROTONOSUPPORT         WSAEPROTONOSUPPORT
	#define ESOCKTNOSUPPORT         WSAESOCKTNOSUPPORT
	#define EOPNOTSUPP              WSAEOPNOTSUPP
	#define EPFNOSUPPORT            WSAEPFNOSUPPORT
	#define EAFNOSUPPORT            WSAEAFNOSUPPORT
	#define EADDRINUSE              WSAEADDRINUSE
	#define EADDRNOTAVAIL           WSAEADDRNOTAVAIL
	#define ENETDOWN                WSAENETDOWN
	#define ENETUNREACH             WSAENETUNREACH
	#define ENETRESET               WSAENETRESET
	#define ECONNABORTED            WSAECONNABORTED
	#define ECONNRESET              WSAECONNRESET
	#define ENOBUFS                 WSAENOBUFS
	#define EISCONN                 WSAEISCONN
	#define ENOTCONN                WSAENOTCONN
	#define ESHUTDOWN               WSAESHUTDOWN
	#define ETOOMANYREFS            WSAETOOMANYREFS
	#define ETIMEDOUT               WSAETIMEDOUT
	#define ECONNREFUSED            WSAECONNREFUSED
	#define ELOOP                   WSAELOOP
	#define ENAMETOOLONG            WSAENAMETOOLONG
	#define EHOSTDOWN               WSAEHOSTDOWN
	#define EHOSTUNREACH            WSAEHOSTUNREACH
	#define ENOTEMPTY               WSAENOTEMPTY
	#define EPROCLIM                WSAEPROCLIM
	#define EUSERS                  WSAEUSERS
	#define EDQUOT                  WSAEDQUOT
	#define ESTALE                  WSAESTALE
	#define EREMOTE                 WSAEREMOTE
#endif

// 调试宏重定义
#if defined(WIN) && !defined(__clang__)
	#define _FUNC_ __FUNCTION__
#else
	#define _FUNC_ << __FUNCTION__ <<
#endif

#ifndef MAX
	#define MAX(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
	#define MIN(a,b)            (((a) < (b)) ? (a) : (b))
#endif

namespace google {namespace protobuf { class Message; }}
typedef google::protobuf::Message Message;

#include <def/global.h>

#endif //_platform_h_