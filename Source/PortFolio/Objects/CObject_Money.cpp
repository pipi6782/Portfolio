#include "CObject_Money.h"
#include "CObject_Money.h"
#include "Global.h"
#include "Components/StaticMeshComponent.h"

ACObject_Money::ACObject_Money()
{
	CHelpers::CreateComponent(this, &Mesh, "Mesh", Scene);

	UStaticMesh* mesh;
	CHelpers::GetAsset(&mesh, "StaticMesh'/Game/Meshes/SM_Money.SM_Money'");
	Mesh->SetStaticMesh(mesh);

	Mesh->SetRelativeRotation(FRotator(0, 90, 0));
	Mesh->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.5f));

}

void ACObject_Money::BeginPlay()
{
	Super::BeginPlay();
}
