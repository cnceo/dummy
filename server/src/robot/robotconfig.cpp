#include "robotconfig.h"

#include <tool/filetool.h>

#include <rapidjson/document.h>
using namespace rapidjson;

bool RobotConfig::load(const char* jsonConfig)
{
	char *json = filetool::open(jsonConfig);
	if (NULL == json) {
		return false;
	}

	Document doc;

	if (doc.ParseInsitu(json).HasParseError()) {
		LOG_ERROR << "parse config<" << jsonConfig << "> failed, error code = " << doc.GetParseError() << ", error offset = " << doc.GetErrorOffset()
		          << ", err text = " << json[doc.GetErrorOffset()];

		delete[] json;
		return false;
	}

	if (!doc.IsObject()) {
		LOG_ERROR;
		return false;
	}

	{
		// 读取机器人数量等配置
		m_robotsPerSecond		= doc["robots-per-second"].GetInt();
		m_robotsInterval		= doc["robots-interval"].GetInt();
		m_speedTestPacketSize	= doc["robots-speedtest-packet-size"].GetInt();
		m_speedTestPacketNum	= doc["robots-speedtest-packet-num"].GetInt();
	}

	{
		// 读取网关服务器地址和网络配置
		const Value& wan			= doc["wan"];
		const Value& gateserver	= wan["connect"];

		m_gateserver.ip		= gateserver["ip"].GetString();
		m_gateserver.port	= gateserver["port"].GetInt();

		m_wanThreadNum		= wan["threads"].GetInt();
	}

	delete[] json;
	return true;
}
