///<------------------------------------------------------------------------------
//< @file:   medal\nonetime\interceptmedal.cpp
//< @author: Kang_zhiquan
//< @date:   2014��6��27�� 17:52:51
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "logic/medal/nonetime/interceptmedal.h"
#include "logic/medal/playermedal.h"

BOOL CInterceptMedal::CheckActivate()
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
	if ( (poMedalProp->qwFirstCondition <= poPlayerMedal->GetDT_MEDAL_BASE_DATA().dwInterceptNum) && (poPlayerMedal->GetDT_MEDAL_BASE_DATA().dwInterceptNum <= poMedalProp->qwSecondCondition))
	{
		return TRUE;
	}

	return FALSE;
}