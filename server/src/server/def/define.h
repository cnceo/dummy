///<------------------------------------------------------------------------------
//< @file:   server\def\define.h
//< @author: 洪坤安
//< @date:   2015年4月7日 20:32:19
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _define_h_
#define _define_h_

#include "basic/objectpool.h"
#include "basic/buffer.h"

#define MAX_PACKET_LEN (1 * 1024 * 1024) // 2MB

namespace global
{
// 网络
	// 预先申请的消息包内存：用于构造接收到的消息包
	extern char* g_recvPacketBuf;
	extern uint32 g_recvPacketBufSize;

	// 预先申请的消息包内存: 用于构造返回的消息包
	extern char* g_packetBuf;
	extern uint32 g_packetBufSize;
	extern Message *g_lastMessage;

	// 预先申请的网络数据缓冲区: 用于发送和接收数据时进行加解密运算
	extern thread_local char g_encryptBuf[MAX_PACKET_LEN];

	//
	extern thread_local char g_recvBuf[MAX_PACKET_LEN];
	extern thread_local char g_sendBuf[MAX_PACKET_LEN];

// 数据库
	extern char g_sql[2048];

// 时间
	extern char g_formatBuf[32];

// 打印
	extern char g_sprintfBuf[4096];

	// 全局初始化
	void init();

	// 全局回收资源
	void uninit();
}

#endif //_define_h_