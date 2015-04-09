

#ifndef _USER_DEVICE_INFO_H_
#define _USER_DEVICE_INFO_H_

#include "basedbccmd.h"
#include <sddb.h>
#include "protocol/server/protoclizs.h"


/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CUserDeviceInfoCMD : public CBaseDBCMD
{
public:
	CUserDeviceInfoCMD();
	~CUserDeviceInfoCMD();

	/**
	* @brief 异步执行SQL命令
	* @poDBConn 异步执行SQL命令的Connection连接
	* @return VOID
	*/						
	VOID SDAPI OnExecuteSql(SGDP::ISDDBConnection* poDBConn);

	/**
	* @brief 同步执行SQL命令相关的逻辑部分
	* @return VOID
	*/
    VOID SDAPI OnExecuted();

public:
	virtual EDBCmdType GetDBCmdType() { return EDBCT_USER_DEVICE_INFO ; }

private:
	PKT_CLIZS_REPORT_USER_DEVICE_INFO_ACK m_stAck;
};

#endif //#ifndef _LOGINCMD_H_

