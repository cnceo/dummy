///<------------------------------------------------------------------------------
//< @file:   server\net\connector.h
//< @author: 洪坤安
//< @date:   2015年3月4日 12:25:54
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _connector_h_
#define _connector_h_

#include "netaddress.h"
#include "net/net.h"

class INetReactor;

class Connector : public IFd
{
public:
	Connector(NetAddress &peerAddr, INetReactor*, INet*, task_queue_pool_t*);

	virtual socket_t socket() const {return m_sockfd;}
	virtual int handleRead();
	virtual int handleWrite(); // 对于非阻塞connect，当连接成功建立时，描述符变成可写
	virtual int handleError();
	virtual void close();

	bool connect();

	bool onConnected();

	bool connecting(socket_t);

	bool retry(socket_t);

private:
	Link* createLink(socket_t newfd, NetAddress &peerAddr);

private:
	static const int MaxRetryDelayMs = 30 * 1000;
	static const int InitRetryDelayMs = 500;

	enum States { kDisconnected, kConnecting, kConnected };

private:
	NetAddress m_peerAddr;
	INetReactor *m_pNetReactor;
	INet *m_net;
	task_queue_pool_t* m_taskQueuePool;

	socket_t m_sockfd;
	int m_retryDelayMs;
	States m_state;  // FIXME: use atomic variable
};

#endif //_connector_h_