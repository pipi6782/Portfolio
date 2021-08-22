#include "CDrawLine.h"
#include "Global.h"
#include "Engine/Canvas.h"

ACDrawLine::ACDrawLine()
{

}

void ACDrawLine::BeginPlay()
{
	Super::BeginPlay();
	

	//RenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(GetWorld());
}

void ACDrawLine::Draw()
{
	//UKismetRenderingLibrary::ClearRenderTarget2D(GetWorld(), RenderTarget);
	//UCanvas* canvas = GetWorld()->GetCanvasForDrawMaterialToRenderTarget();
	//
	//FDrawToRenderTargetContext context;
	//context.RenderTarget = RenderTarget;
	//
	//UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(GetWorld(), RenderTarget, canvas, FVector2D(canvas->SizeX, canvas->SizeY), context);
}
