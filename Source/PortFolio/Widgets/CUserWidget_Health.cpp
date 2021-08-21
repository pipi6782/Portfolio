#include "CUserWidget_Health.h"
#include "Global.h"
#include "Components/GridPanel.h"
#include "Components/Image.h"
#include "CUserWidget_Heart.h"

void UCUserWidget_Health::NativeConstruct()
{
	TArray<UWidget*> children = Grid->GetAllChildren();

	for (UWidget* child : children)
		Hearts.Add(Cast<UCUserWidget_Heart>(child));

	Super::NativeConstruct();
}

void UCUserWidget_Health::UpdateHealth(float InValue)
{
	bool bLeft = true;
	float health = 0;
	UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	while(health < Hearts.Num())
	{
		if (health < InValue)
		{
			if (bLeft)
			{
				Hearts[static_cast<int32>(health)]->SetLeftVisible();
				bLeft = false;
			}
			else
			{
				Hearts[static_cast<int32>(health)]->SetRightVisible();
				bLeft = true;
			}
		}
		else
		{
			if (bLeft)
			{
				Hearts[static_cast<int32>(health)]->SetLeftHidden();
				bLeft = false;
			}
			else
			{
				Hearts[static_cast<int32>(health)]->SetRightHidden();
				bLeft = true;
			}
		}
		health += 0.5f;
	}
}

void UCUserWidget_Health::UpdateMaxHealth(float InValue)
{
	for (int32 i = 0; i < Hearts.Num(); i++)
	{
		(i >= InValue) ? Hearts[i]->SetVisibility(ESlateVisibility::Collapsed) : Hearts[i]->SetVisibility(ESlateVisibility::Visible);
	}
}