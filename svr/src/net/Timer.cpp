// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include "Timer.h"

#include "EventLoop.h"

using namespace net;

void Timer::Restart()
{
    EventLoop &loop = EventLoop::getEventLoopOfCurrentThread();
    loop.runAfter(m_interval, m_callback);
}