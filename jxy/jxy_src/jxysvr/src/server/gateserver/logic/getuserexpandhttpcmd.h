#ifndef _GETUSEREXPAND_HTTP_CMD_H_
#define _GETUSEREXPAND_HTTP_CMD_H_


#include <sdframework.h>
#include <curl/curl.h>
#include "basehttpcmd.h"
#include "protocol/server/protoclils.h"
#include <algorithm/sdmd5.h>


struct SGetUserExpandInfo
{
	UINT32 dwUserID;
	UINT32 dwPlayerID;
};

class CGetUserExpandHttpCMD : public CBaseHttpCMD
{
public:
	CGetUserExpandHttpCMD();
	~CGetUserExpandHttpCMD();

	virtual BOOL  Init();
	 /**
    * @brief 设置用户特定数据
    * @param pUserData 设置的用户数据
    * @param dwLen 设置的用户数据长度
    * @param dwTransID 设置的TransID
    */
    virtual VOID SetUserData(VOID* pUserData, UINT32 dwLen, UINT32 dwTransID);
public:
	virtual EHttpCmdType GetHttpCmdType(){ return EHCT_GET_USER_EXPAND; }

protected:
    /**
    * @brief 发送回馈消息
    * @param pBuf 需要回馈的数据
    * @return VOID
    */
    //数据执行完后回调
    virtual BOOL OnExecuted();
	BOOL SendMessage(UINT16 wErrcode, DT_EXPAND_FUNC_PARAM_LST* stExpandLst);
private:
	SGetUserExpandInfo m_stGetUserExpandInfo;
};

#endif