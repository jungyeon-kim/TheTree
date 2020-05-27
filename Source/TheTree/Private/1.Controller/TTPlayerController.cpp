#include "TTPlayerController.h"
#include "TTUIPlayerInGame.h"

ATTPlayerController::ATTPlayerController()
{
	static ConstructorHelpers::FClassFinder<UTTUIPlayerInGame> UI_PLAYER_INGAME(TEXT("/Game/Blueprints/UI/UI_Player_InGame/UI_Player_InGame.UI_Player_InGame_C"));
	if (UI_PLAYER_INGAME.Succeeded()) TTUIPlayerInGameClass = UI_PLAYER_INGAME.Class;
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
	
	TTUIPlayerInGame = CreateWidget<UTTUIPlayerInGame>(this, TTUIPlayerInGameClass);
	TTUIPlayerInGame->AddToViewport();

	OnSyncDelegate.Broadcast();
}

void ATTPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

UTTUIPlayerInGame* ATTPlayerController::GetUIPlayerInGame() const
{
	return TTUIPlayerInGame;
}

void ATTPlayerController::SwapDebugMode()
{
	if (!FTTWorld::bIsDebugging) FTTWorld::bIsDebugging = true;
	else FTTWorld::bIsDebugging = false;
}
