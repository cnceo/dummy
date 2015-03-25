///<------------------------------------------------------------------------------
//< @file:   server\net\serverlink.h
//< @author: 洪坤安
//< @date:   2015年3月9日 11:42:53
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _serverlink_h_
#define _serverlink_h_

#include "net/netreactor.h"
#include "protocol/protocol.h"
#include "tool/servertool.h"

class ServerLink : public INetReactor
{
public:
	// @override
	virtual void OnDisconnect(Link*, const NetAddress& localAddr, const NetAddress& peerAddr);

	virtual void OnRecv(Link*, Buffer&);

	virtual TaskQueue& GetTaskQueue() { return *m_taskQueue; }

public:
	virtual void onEstablish() {}

	virtual void onTerminate() {}

public:
	Link *m_link;

	ServerType m_remoteSvrType;
	int m_svrId;

	TaskQueue *m_taskQueue;
};

#endif //_serverlink_h_