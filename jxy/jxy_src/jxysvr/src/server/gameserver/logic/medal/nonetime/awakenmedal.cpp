///<------------------------------------------------------------------------------
//< @file:   medal\nonetime\awakenmedal.cpp
//< @author: Kang_zhiquan
//< @date:   2014��6��26�� 21:28:45
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "logic/medal/nonetime/awakenmedal.h"
#include "logic/medal/playermedal.h"
BOOL CAwakenMedal::CheckActivate()
{
	return FALSE;
	if ( m_stMedalData.byIsActivate)
	{
		return TRUE;
	}

	const SMedalProp* poMedalProp = NULL;
	if ( !GetProp(poMedalProp))
	{
		return FALSE;
	}
	CPlayerMedal* poPlayerMedal = m_poOwner->GetPlayerMedal();
	if(NULL == poPlayerMedal)
	{
		return FALSE;
	}
	if ( (poMedalProp->qwFirstCondition <= poPlayerMedal->GetDT_MEDAL_BASE_DATA().dwZTSNum) && (poPlayerMedal->GetDT_MEDAL_BASE_DATA().dwZTSNum <= poMedalProp->qwSecondCondition))
	{
		return TRUE;
	}

	return FALSE;
}