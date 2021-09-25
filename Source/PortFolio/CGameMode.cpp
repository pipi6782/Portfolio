#include "CGameMode.h"
#include "Global.h"
#include "GameFramework/Hud.h"

ACGameMode::ACGameMode()
{
	CHelpers::GetClass(&DefaultPawnClass, "Blueprint'/Game/Player/BP_CPlayer.BP_CPlayer_C'");
	CHelpers::GetClass(&HUDClass, "Blueprint'/Game/Player/BP_CHUD.BP_CHUD_C'");
}