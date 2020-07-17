#include "TTPlayerController.h"
#include "TTCharacterStatComponent.h"
#include "TTUIPlayerInGame.h"
#include "TTUIReinforce.h"

ATTPlayerController::ATTPlayerController()
{
	static ConstructorHelpers::FClassFinder<UTTUIPlayerInGame> UI_PLAYER_INGAME{ TEXT("/Game/Blueprints/UI/UI_Player_InGame/UI_Player_InGame.UI_Player_InGame_C") };
	static ConstructorHelpers::FClassFinder<UTTUIReinforce> UI_REINFORCE{ TEXT("/Game/Blueprints/UI/UI_Reinforce/UI_Reinforce.UI_Reinforce_C") };
	if (UI_PLAYER_INGAME.Succeeded()) TTUIPlayerInGameClass = UI_PLAYER_INGAME.Class;
	if (UI_REINFORCE.Succeeded()) TTUIReinforceClass = UI_REINFORCE.Class;
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

UTTUIPlayerInGame* ATTPlayerController::GetUIPlayerInGame() const
{
	return TTUIPlayerInGame;
}

UTTUIReinforce* ATTPlayerController::GetUIReinforce() const
{
	return TTUIReinforce;
}

void ATTPlayerController::SetUIPlayerInGame(UTTCharacterStatComponent* NewCharacterStat)
{
	TTUIPlayerInGame = CreateWidget<UTTUIPlayerInGame>(this, TTUIPlayerInGameClass);
	TTUIPlayerInGame->AddToViewport();
	TTUIPlayerInGame->SetVisibility(ESlateVisibility::Hidden);

	TTUIPlayerInGame->BindCharacterStat(NewCharacterStat);
}

void ATTPlayerController::SetUIReinforce(UTTCharacterStatComponent* NewCharacterStat)
{
	TTUIReinforce = CreateWidget<UTTUIReinforce>(this, TTUIReinforceClass);
	TTUIReinforce->AddToViewport();
	TTUIReinforce->SetVisibility(ESlateVisibility::Hidden);

	TTUIReinforce->BindCharacterStat(NewCharacterStat);
}

void ATTPlayerController::SwapDebugMode()
{
	if (!FTTWorld::bIsDebugging) FTTWorld::bIsDebugging = true;
	else FTTWorld::bIsDebugging = false;
}
