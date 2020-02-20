#include "TTPlayerController.h"

ATTPlayerController::ATTPlayerController()
{
}

void ATTPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ATTPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//FInputModeGameOnly InputModeGameOnly{};
	//SetInputMode(InputModeGameOnly);
}

void ATTPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	//SetMouseLocation(0.0f, 0.0f);
}

void ATTPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}
