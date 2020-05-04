#include "TTPlayerController.h"

ATTPlayerController::ATTPlayerController()
{
}

void ATTPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ATTPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	InputComponent->BindAction(TEXT("SwapDebugMode"), EInputEvent::IE_Pressed, this, &ATTPlayerController::SwapDebugMode);
}

void ATTPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly InputModeGameOnly{};
	SetInputMode(InputModeGameOnly);
}

void ATTPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

void ATTPlayerController::SwapDebugMode()
{
	if (!FTTWorld::bIsDebugging) FTTWorld::bIsDebugging = true;
	else FTTWorld::bIsDebugging = false;
}
