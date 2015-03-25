///<------------------------------------------------------------------------------
//< @file:   server\net\msghandler.h
//< @author: 洪坤安
//< @date:   2015年3月17日 23:59:29
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _msghandler_h_
#define _msghandler_h_

#include <google/protobuf/message.h>

#include "basic/bind.h"
#include "tool/timetool.h"
#include "net/msgdispatcher.h"
#include "net/imsghandler.h"

class Link;
typedef google::protobuf::Message Message;

template <typename LinkType>
class Callback
{
public:
	virtual ~Callback() {};
	virtual void onMessage(LinkType&, const char* data, int len, Timestamp) const = 0;
};

template <typename LinkType, typename MessageType>
class CallbackT : public Callback<LinkType>
{
public:
	typedef void (*ProtobufMessageCallback)(LinkType*, MessageType* message, Timestamp);

	CallbackT(const ProtobufMessageCallback &callback)
		: m_callback(callback)
	{
	}

	virtual void onMessage(LinkType& link, const char* data, int len, Timestamp receiveTime) const
	{
		MessageType t;

		bool ok = t.ParseFromArray(data, len);
		if (!ok) {
			LOG_INFO << "decode packet <" << t.SerializeAsString() << "> error";
			return;
		}

		// assert(t != NULL);
		m_callback(&link, &t, receiveTime);
	}

private:
	ProtobufMessageCallback m_callback;
};

// 消息处理器的实现（这里之所以提取出IMsgHandler接口是为了防止模板文件互相包含导致编译不过）
// LinkType代表从谁身上得到的message，比如Client、Link等
template <typename LinkType>
class MsgHandler : public IMsgHandler<LinkType>
{
private:
	typedef std::map<int, Callback<LinkType>*> CallbackMap;

public:
	MsgHandler(MsgDispatcher<LinkType> *msgDispacher)
		: m_msgDispacher(msgDispacher)
	{
	}

	void onMessage(LinkType& link, int msgId, const char* data, int len, Timestamp receiveTime) const
	{
		CallbackMap::const_iterator it = m_callbackMap.find(msgId);
		if (it == m_callbackMap.end()) {
			//discardMessage(link, message, receiveTime);
			return;
		}

		Callback<LinkType> *callback = it->second;
		callback->onMessage(link, data, len, receiveTime);
	}

	template <typename MessageType>
	void registerMsg(int msgId, void (*callback)(LinkType*, MessageType* message, Timestamp))
	{
		CallbackT<LinkType, MessageType> *pd = new CallbackT<LinkType, MessageType>(callback);
		m_callbackMap[msgId] = pd;

		m_msgDispacher->Register(msgId, this);
	}

	void discardMessage(Link& link, const Message& message, Timestamp receiveTime) const
	{
		LOG_DEBUG << "Discarding " << msgtool::getMsgString(message);
	}

	CallbackMap m_callbackMap;
	MsgDispatcher<LinkType> *m_msgDispacher;
};

#endif //_msghandler_h_