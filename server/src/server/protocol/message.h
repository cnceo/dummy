///<------------------------------------------------------------------------------
//< @file:   server\protocol\message.h
//< @author: 洪坤安
//< @date:   2015年3月8日 15:53:38
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _message_h_
#define _message_h_

#include <google/protobuf/message.h>

// namespace google {namespace protobuf { class Message; }}

// 外网消息头
struct NetMsgHead {
	uint16 msgId;
	uint32 msgLen;
};

// 内网消息头
struct LanMsgHead {
	uint32 clientId;
	uint16 msgId;
	uint32 msgLen;
};

namespace msgtool
{
	// 获取msg的详细内容
	string getMsgDebugString(const Message &msg);

	// 构建网络包头
	int buildNetHeader(NetMsgHead *msgHead, uint16 msgId, uint32 msgLen);

	// 构建内网消息包头
	int buildLanMsgHeader(LanMsgHead *msgHead, uint32 clientId, uint16 msgId, uint32 msgLen);

	// 在预先分配好的接收消息包内存上申请一个Message
	template<typename T>
	T* allocRecvPacket()
	{
		if (global::g_recvPacketBufSize < sizeof(T)) {
			LOG_WARN << "global::g_recvPacketBufSize old = " << global::g_recvPacketBufSize << ", new = " << sizeof(T);

			delete[] global::g_recvPacketBuf;
			global::g_recvPacketBuf		= new char[sizeof(T)];
			global::g_recvPacketBufSize = sizeof(T);
		}

		T* t = new ((T*)global::g_recvPacketBuf)T();
		return t;
	}


	// 在预先分配好的内存上申请一个Message
	template<typename T>
	T* allocPacket()
	{
		if (global::g_lastMessage) {
			global::g_lastMessage->~Message();
		}

		if (global::g_packetBufSize < sizeof(T)) {
			LOG_WARN << "global::g_packetBufSize old = " << global::g_packetBufSize << ", new = " << sizeof(T);

			delete[] global::g_packetBuf;
			global::g_packetBuf		= new char[sizeof(T)];
			global::g_packetBufSize = sizeof(T);
		}

		T* t = new ((T*)global::g_packetBuf)T();
		global::g_lastMessage = t;
		return t;
	}

	template<typename T>
	inline void freePacket(T *obj)
	{
		if (obj) {
			obj->~T();
		}
	}
}

#endif //_message_h_