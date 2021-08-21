// Copyright Epic Games, Inc. All Rights Reserved.


#include "CGameMode.h"
#include "Global.h"

ACGameMode::ACGameMode()
{
	CHelpers::GetClass(&DefaultPawnClass, "Blueprint'/Game/Player/BP_CPlayer.BP_CPlayer_C'");
}