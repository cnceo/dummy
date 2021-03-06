///<------------------------------------------------------------------------------
//< @file:   server\tool\timetool.h
//< @author: 洪坤安
//< @date:   2015年1月16日 22:54:23
//< @brief:	 获取时间
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _timetool_h_
#define _timetool_h_

typedef uint64 Timestamp; // utc格式时间，从1900起经过的时间，单位毫秒
typedef int32 TimeInMs; // in ms毫秒单位

#define MsPerSecond 1000 // 毫秒
#define UsPerSecond (1000 * 1000) // 微秒

namespace timetool
{
	// 获取当前时间，单位: 毫秒
	Timestamp now();

	// 获取本地时区时间，单位: 毫秒
	Timestamp localnow();

	const char* timeToText(const Timestamp& time);

	const char* nowText();
}

#endif //_timetool_h_