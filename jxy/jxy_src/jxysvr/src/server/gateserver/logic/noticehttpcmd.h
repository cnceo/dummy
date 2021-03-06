

#ifndef _NOTICE_HTTP_CMD_H_
#define _NOTICE_HTTP_CMD_H_

#include <sdframework.h>
#include <curl/curl.h>
#include "basehttpcmd.h"
#include "protocol/server/protoclils.h"
#include <algorithm/sdmd5.h>



struct SNoticeInfo
{
    SNoticeInfo()
    {
        wZoneID = 0;
    }
    UINT16	wZoneID; //认证类型
};


/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CNoticeHttpCMD : public CBaseHttpCMD
{
public:
    CNoticeHttpCMD();
    ~CNoticeHttpCMD();

	virtual BOOL  Init();

    //数据执行完后回调
    virtual BOOL OnExecuted();

    virtual EHttpCmdType GetHttpCmdType()
    {
        return EHCT_NOTICE;
    }
    /**
    * @brief 设置用户特定数据
    * @param pUserData 设置的用户数据
    * @param dwLen 设置的用户数据长度
    * @param dwTransID 设置的TransID
    */
    virtual VOID SetUserData(VOID* pUserData, UINT32 dwLen, UINT32 dwTransID);

private:
    VOID	ProcessResult();
private:
    SNoticeInfo stNoticeInfo;

};

#endif //#ifndef _BASEDBCMD_H_

