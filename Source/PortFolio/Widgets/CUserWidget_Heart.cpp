#include "CUserWidget_Heart.h"
#include "Components/GridPanel.h"
#include "Components/Image.h"

void UCUserWidget_Heart::NativeConstruct()
{
	TArray<UWidget*> children = Grid->GetAllChildren();

	for (UWidget* child : children)
		Hearts.Add(Cast<UImage>(child));

	Super::NativeConstruct();
}

void UCUserWidget_Heart::SetVisible(int32 index, EVisibleType Type)
{
	switch (Type)
	{
	case EVisibleType::Visible:
		Hearts[index]->SetVisibility(ESlateVisibility::Visible);
		break;
	case EVisibleType::None:
		Hearts[index]->SetVisibility(ESlateVisibility::Collapsed);
		break;
	}
}
