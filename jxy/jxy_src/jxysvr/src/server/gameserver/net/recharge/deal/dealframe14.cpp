// DealFrame14.cpp: implementation of the CDealFrame14 class.
//
//////////////////////////////////////////////////////////////////////


#include "dealframe14.h"
#include "net/ks/ksprocessor.h"
#include "net/recharge/rcclient.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealFrame14::CDealFrame14()
{

}

CDealFrame14::~CDealFrame14()
{

}

void CDealFrame14::Release()
{
	
}

UINT16 CDealFrame14::GetFrameType()
{
	return 14;
}

void CDealFrame14::Deal(CRCClient* poClinet, UINT32 dwSerialNumber, UINT8 byAction, UINT32 dwAreaID, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, TCHAR* pszParam4, TCHAR* pszParam5, TCHAR *pszDesc)
{
	if((NULL == poClinet) || (NULL == pszParam4))
	{
		return;
	}
	UINT16 wErrCode = CGMProcessor::GmReq(dwSerialNumber, byAction, dwPlayerID, dwParam1, qwParam2, qwParam3, pszParam4, poClinet->GetCliSessionID(), pszDesc);
	if (ERR_GM_PARM_INFO::ID_PLAYER_NOT_MEMORT == wErrCode)
		return;
	CHAR szRet[128] = {0};
	if(0 == wErrCode)
	{
		sprintf(szRet, "{\"status\":\"success\"}");
	}
	else
	{
		sprintf(szRet, "{\"status\":\"error_%d\"}", wErrCode);
	}
	poClinet->Rspone(szRet);
}

void CDealFrame14::Deal( CRCClient* poClinet,UINT32 dwSerialNumber,UINT32 dwAreaID,UINT32 dwPlayerID,map<string,string> &mapField )
{
	if (NULL == poClinet)
	{
		return;
	}

	if (mapField[PLAYER_ID].empty() || mapField[SET_VIP_LEVEL_COUNT_PARAM].empty() || mapField[SET_VIP_LEVEL_TOTAL_AMOUNT_PARAM].empty())
	{
		string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PARAM_ERR);
		poClinet->Rspone(strErrorMsg.c_str());
		return;
	}

	UINT32 qwParam1 = SDAtou(mapField[SET_VIP_LEVEL_COUNT_PARAM].c_str());
	UINT32 qwParam2 = SDAtou(mapField[SET_VIP_LEVEL_TOTAL_AMOUNT_PARAM].c_str());
	UINT32 unPlayerId = SDAtou(mapField[PLAYER_ID].c_str());
	UINT16 wErrCode = CGMProcessor::GmReq(dwSerialNumber,(UINT8)GetFrameType(), unPlayerId,qwParam1,qwParam2,0,"",poClinet->GetCliSessionID(), mapField["desc"].c_str());
	if (ERR_GM_PARM_INFO::ID_PLAYER_NOT_MEMORT == wErrCode)
		return;
	string strErrorMsg = GetRsponeResult(wErrCode);
	poClinet->Rspone(strErrorMsg.c_str());
}

string CDealFrame14::GetStrFrameType()
{
	return SET_VIP_LEVEL;
}

void CDealFrame14::Interface( CRCClient* poClinet )
{
	if (NULL == poClinet)
	{
		return;
	}

	map<string,string> mapField;
	mapField[PLAYER_ID] = SDGBK2UTF8(PLAYER_USER_ID) + "|" + GM_PARAM_PLAYER_ID;
	mapField[SET_VIP_LEVEL_COUNT_PARAM] = SDGBK2UTF8(SET_VIP_LEVLE_GBK) + "|" + GM_PARAM_INT;
	mapField[SET_VIP_LEVEL_TOTAL_AMOUNT_PARAM] = SDGBK2UTF8(SET_VIP_LEVEL_TOTAL_AMOUNT_GBK) + "|" + GM_PARAM_INT;

	RsponeMsg(poClinet,mapField);
}

string CDealFrame14::GetStrFrameDesc()
{
	return SDGBK2UTF8(SET_VIP_LEVEL_DESC);
}

