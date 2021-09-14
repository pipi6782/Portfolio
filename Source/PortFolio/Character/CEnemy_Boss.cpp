#include "CEnemy_Boss.h"
#include "Global.h"

ACEnemy_Boss::ACEnemy_Boss()
{
	CHelpers::GetClass(&AIControllerClass, "Blueprint'/Game/Enemies/Boss/BP_CAIController_Boss.BP_CAIController_Boss_C'");
}
