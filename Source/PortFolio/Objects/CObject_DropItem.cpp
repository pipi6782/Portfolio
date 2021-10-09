#include "CObject_DropItem.h"
#include "Global.h"

ACObject_DropItem::ACObject_DropItem()
{
	//CreateComponent
	CHelpers::CreateComponent(this, &Mesh, "Mesh", Scene);
}


void ACObject_DropItem::SetDrop(bool InOnGround)
{
	bOnGround = InOnGround;
}
