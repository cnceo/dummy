// DealFrame19.cpp: implementation of the CDealFrame19 class.
//
//////////////////////////////////////////////////////////////////////


#include "dealframe19.h"
#include "net/ks/ksprocessor.h"
#include "net/recharge/rcclient.h"
#include "logic/player/playermgr.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealFrame19::CDealFrame19()
{

}

CDealFrame19::~CDealFrame19()
{

}

void CDealFrame19::Release()
{
	
}

UINT16 CDealFrame19::GetFrameType()
{
	return 19;
}

void CDealFrame19::Deal(CRCClient* poClinet, UINT32 dwSerialNumber, UINT8 byAction, UINT32 dwAreaID, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, TCHAR* pszParam4, TCHAR* pszParam5, TCHAR *pszDesc)
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

void CDealFrame19::Deal( CRCClient* poClinet,UINT32 dwSerialNumber,UINT32 dwAreaID,UINT32 dwPlayerID,map<string,string> &mapField )
{
	if (NULL == poClinet)
	{
		return;
	}

	if (mapField[ALL_STORY_COUNT_PARAM].empty())
	{
		string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PARAM_ERR);
		poClinet->Rspone(strErrorMsg.c_str());
		return;
	}

	UINT32 qwParam1 = SDAtou(mapField[ALL_STORY_COUNT_PARAM].c_str());
	UINT16 wErrCode = CGMProcessor::GmReq(dwSerialNumber,(UINT8)GetFrameType(), 0,qwParam1,0,0,"",poClinet->GetCliSessionID(), mapField["desc"].c_str());
	if (ERR_GM_PARM_INFO::ID_PLAYER_NOT_MEMORT == wErrCode)
		return;
	const CID2PlayerMap & mapID2PlayerMap = CPlayerMgr::Instance()->GetID2PlayerMap();

	if (0 == mapID2PlayerMap.size())
	{
		string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_NOT_DATA);
		poClinet->Rspone(strErrorMsg.c_str());
		return;
	}

	CID2PlayerMapConstItr iter;

	for (iter = mapID2PlayerMap.begin();iter != mapID2PlayerMap.end();iter++)
	{
		CPlayer *pPlayer = iter->second;

		if (NULL == pPlayer)
		{
			continue;
		}

		pPlayer->AddStory(qwParam1, CRecordMgr::EASTT_GETGIVEGM);
	}


	string strRetInfo = GetRsponeResult(ERR_GM_PARM_INFO::ID_SUCCESS);
	poClinet->Rspone(strRetInfo.c_str());
}

string CDealFrame19::GetStrFrameType()
{
	return  ALL_STORY;
}

void CDealFrame19::Interface( CRCClient* poClinet )
{
	if (NULL == poClinet)
	{
		return;
	}

	map<string,string> mapField;
	mapField[ALL_STORY_COUNT_PARAM] = SDGBK2UTF8(ALL_STORY_GBK) + "|" + GM_PARAM_INT;

	RsponeMsg(poClinet,mapField);
}

string CDealFrame19::GetStrFrameDesc()
{
	return SDGBK2UTF8(ALL_STORY_DESC);
}

