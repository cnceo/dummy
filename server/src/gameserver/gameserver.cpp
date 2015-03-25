///<------------------------------------------------------------------------------
//< @file:   src\gameserver\gameserver.cpp
//< @author: 洪坤安
//< @date:   2015年3月2日 21:11:22
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "gameserver.h"

#include "net/gatelink.h"

GameServer::GameServer()
	: Server()
	, m_gateLink(NULL)
{
	m_svrType = eGameServer;
}

bool GameServer::init()
{
	if (!Server::init()) {
		return false;
	}

	logging::init("gameserver", "log_gamesvr_");

	m_lan.init(1);
	m_lan.listen("127.0.0.1", 10000, *this);

	for(int i = 0; i < 2; i++) {
		// m_wan.connect("115.239.211.112", 20001, *this);
		m_lan.connect("127.0.0.1", 10001, *this);
	}

	return true;
}

void GameServer::start()
{
	Server::start();
}

void GameServer::run()
{
	Server::run();
	Sleep(10);
}

ServerLink* GameServer::OnAcceptServer(Link &tcpLink, ServerType peerSvrType, int peerSvrId)
{
	ServerLink *svrLink = NULL;

	switch(peerSvrType) {
	case eGateServer:
		svrLink = m_gateLink = new GateLink;
		LOG_INFO << "gameserver -> gateserver <svrId = " << peerSvrId << "> connection established";
		break;

	default:
		LOG_ERROR << "gameserver -> unknown server <svrType=" << peerSvrType << ",svrId=" << peerSvrId << "> connection established";
		break;
	}

	return svrLink;
}

void GameServer::OnDisconnectServer(Link &tcpLink, ServerType svrType, int serverId)
{
	ServerLink *link = NULL;

	switch(svrType) {
	case eGateServer:
		link = m_gateLink;
		m_gateLink = NULL;

		LOG_INFO << "gameserver -> gateserver<svrId = " << serverId << "> connection broken";
		break;

	default:
		LOG_ERROR << "gameserver -> unknown server <svrType=" << svrType << ",svrId=" << serverId << "> connection broken";
		break;
	}

	delete link;
}
