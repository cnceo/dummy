///<------------------------------------------------------------------------------
//< @file:   src\robot\robotmgr.cpp
//< @author: 洪坤安
//< @date:   2015年3月16日 11:43:52
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "robotmgr.h"

#include <signal.h>

#include "robot.h"
#include "robotmsghandler.h"

void registerSignal();

void handleSignal(int sig)
{
	SAFE_LOG("%s pid = %u, handle signal = %u", RobotMgr::Instance().name().c_str(), getpid(), sig);
	if (sig == SIGTERM) {
		RobotMgr::Instance().stop();
	} else if(sig == SIGINT) {
		RobotMgr::Instance().stop();
	}

	// 重新注册，因为捕捉到信号后已经恢复成原来的信号处理行为
	// registerSignal();
}

void registerSignal()
{
#ifdef WIN
	signal(SIGTERM, &handleSignal);
	signal(SIGINT,	&handleSignal);
#else
	struct sigaction act;
	act.sa_handler = handleSignal;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	sigaction(SIGINT, &act, 0);
	sigaction(SIGTERM, &act, 0);
#endif
}

bool RobotMgr::init(const char* jsonConfig)
{
	// registerSignal();

	global::init();

	logging::init("robot", "log_robot_");

	LOG_WARN << "starting robotmgr ...";

	if (!m_config.load(jsonConfig)) {
		LOG_INFO << name() << "load config failed! aborted!";
		return false;
	}

	m_wan.init(m_config.m_wanThreadNum);

	if (!m_httpMgr.init()) {
		LOG_ERROR << "robothttpmgr init failed, aborted";
		return false;
	}

	creareRobots();
	m_timers.runEvery(boost::bind(&RobotMgr::creareRobots, this), m_config.m_robotsInterval);

// 	for(int i = 0; i < m_config.m_robotNum; i++) {
// 		Robot *robot = createRobot();
// 		// robot->start();
//
// 		// LOG_INFO << "robot " << i << " start connecting to server";
// 		m_wan.connect(m_config.m_gateserver.ip, m_config.m_gateserver.port, *robot, "gateserver");
// 		// m_wan.connect("127.0.0.1", 80, *robot);
// 	}

	m_dispatcher.addMsgHandler(new RobotMsgHandler(&m_dispatcher));

	m_run = true;

	LOG_WARN << "start robotmgr successfully!";
	return true;
}

bool RobotMgr::uninit()
{
	m_httpMgr.uninit();

	m_dispatcher.clear();
	global::uninit();

	return true;
}

Robot* RobotMgr::createRobot()
{
	Robot *robot = new Robot;

	robot->m_robotMgr	= this;
	robot->m_robotId	= allocRobotId();

	m_robotMap[robot->m_robotId]  = robot;
	return robot;
}

void RobotMgr::creareRobots()
{
	for(int i = 0; i < m_config.m_robotsPerSecond; i++) {
		Robot *robot = createRobot();
		// robot->start();

		// LOG_INFO << "robot " << i << " start connecting to server";
		m_wan.connect(m_config.m_gateserver.ip, m_config.m_gateserver.port, *robot, "gateserver");
		// m_wan.connect("127.0.0.1", 80, *robot);
	}
}

void RobotMgr::start()
{
	m_wan.start();

	while(m_run) {
		run();
	}

	uninit();
	LOG_WARN << "close robotmgr successfully!";
}

void RobotMgr::stop()
{
	if (!m_run) {
		return;
	}

	LOG_WARN << "start closing robotmgr ...";
	LOG_WARN << "	<m_taskQueue.size() = " << m_taskQueue.size() << ">";

	m_taskQueue.put(boost::bind(&RobotMgr::stopping, this));
	// run();
}

void RobotMgr::stopping()
{
	m_run = false;

	LOG_WARN << "stopping robotmgr ...";

	m_wan.stop();

	// 将关闭网络时产生的网络任务执行完
	run();
}

void RobotMgr::run()
{
	m_taskQueue.run();
	m_timers.run();
	m_httpMgr.run();
	sleep(10);
}

void RobotMgr::onRobotDisconnect(Robot *robot)
{
	m_robotMap.erase(robot->m_robotId);
	delete robot;

	if (m_robotMap.empty()) {
		// stop();
	}

// 	if (m_robotMap.size() == 1) {
// 		Robot *robot = m_robotMap.begin()->second;
// 		LOG_INFO << robot->name() << " sendbuf.size = " << robot->m_link->m_sendBuf.readableBytes();
// 	}
}
