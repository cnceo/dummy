// DealFrame0x001A.cpp: implementation of the CDealFrame0x001A class.
//
//////////////////////////////////////////////////////////////////////


#include "dealframecoin.h"
#include <logic/player/player.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealFrameCoin::CDealFrameCoin()
{

}

CDealFrameCoin::~CDealFrameCoin()
{

}

void CDealFrameCoin::Release()
{

}

string CDealFrameCoin::GetFrameType()
{
    return "coin";
}

BOOL CDealFrameCoin::Deal(CPlayer* poPlayer, vector<string> vecOpValue, UINT8 byOpType, UINT32 dwNum1, UINT32 dwNum2, UINT32 dwNum3, TCHAR *pszDesc, DT_PLAYER_RES_DATA* pstResInfo1, DT_PLAYER_RES_DATA* pstResInfo2)
{
    if(NULL == poPlayer)
    {
        return FALSE;
    }

    //��������
    if (vecOpValue.size() < 2)
    {
        return FALSE;
    }

    UINT32 dwOpValue = SDAtou(vecOpValue[1].c_str());
    switch(byOpType)
    {
    case RESOPTYPE_CKNUM:
    {
        return poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin >= dwOpValue ? TRUE : FALSE;
    }
    break;
    case RESOPTYPE_DECNUM:
    {
        poPlayer->DecCoin(dwOpValue, CRecordMgr::EDCT_GETGIVEGM, 0, 0, 0, 0, pszDesc);
    }
    break;
    case RESOPTYPE_ADDNUM:
    {
        poPlayer->AddCoin(dwOpValue, CRecordMgr::EACT_GETGIVEGM, 0, 0, 0, 0, pszDesc);
    }
    break;
	case RESOPTYPE_CK_ADD_LIMIT:
	{
		return CkAddLimit(vecOpValue[0],dwOpValue);
	}
	break;
    default:
        return FALSE;
        break;
    }

    if(pstResInfo1)
    {
        pstResInfo1->byResKind = COIN;
        pstResInfo1->qwResValue = poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin;
    }

    return TRUE;
}

