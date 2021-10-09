#include "CMeshCapture.h"
#include "Global.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"

ACMeshCapture::ACMeshCapture()
{
	CHelpers::CreateComponent(this, &Scene, "Scene");
	CHelpers::CreateComponent(this, &SpringArm, "SpringArm",Scene);
	CHelpers::CreateComponent(this, &Mesh, "Mesh",Scene);
	CHelpers::CreateComponent(this, &SceneCapture, "SceneCapture", SpringArm);

	SpringArm->SetRelativeRotation(FRotator(0, -90.0f, 0));
	SpringArm->TargetArmLength = 25.0f;

	SceneCapture->ShowOnlyActorComponents(this);
}

