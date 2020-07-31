#include "TTPlayerController.h"
#include "TTCharacterStatComponent.h"
#include "TTUIPlayerInGame.h"
#include "TTUIPlayerStatus.h"
#include "TTUIReinforce.h"
#include "TTUIRecovery.h"
#include "TTUIMap.h"
#include "TTUIQuitGame.h"

ATTPlayerController::ATTPlayerController()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_MANUAL{ TEXT("/Game/Blueprints/UI/UI_Manual/UI_KeyManual.UI_KeyManual_C") };
	static ConstructorHelpers::FClassFinder<UTTUIPlayerInGame> UI_PLAYER_INGAME{ TEXT("/Game/Blueprints/UI/UI_Player_InGame/UI_Player_InGame.UI_Player_InGame_C") };
	static ConstructorHelpers::FClassFinder<UTTUIPlayerStatus> UI_PLAYER_STATUS{ TEXT("/Game/Blueprints/UI/UI_Player_Status/UI_Player_Status.UI_Player_Status_C") };
	static ConstructorHelpers::FClassFinder<UTTUIReinforce> UI_REINFORCE{ TEXT("/Game/Blueprints/UI/UI_Reinforce/UI_Reinforce.UI_Reinforce_C") };
	static ConstructorHelpers::FClassFinder<UTTUIRecovery> UI_RECOVERY{ TEXT("/Game/Blueprints/UI/UI_Recovery/UI_Recovery.UI_Recovery_C") };
	static ConstructorHelpers::FClassFinder<UTTUIMap> UI_MAP{ TEXT("/Game/Blueprints/UI/UI_Map/UI_Map.UI_Map_C") };
	static ConstructorHelpers::FClassFinder<UTTUIQuitGame> UI_QUITGAME{ TEXT("/Game/Blueprints/UI/UI_Quit/UI_QuitGame.UI_QuitGame_C") };
	if (UI_MANUAL.Succeeded()) TTUIManualClass = UI_MANUAL.Class;
	if (UI_PLAYER_INGAME.Succeeded()) TTUIPlayerInGameClass = UI_PLAYER_INGAME.Class;
	if (UI_PLAYER_STATUS.Succeeded()) TTUIPlayerStatusClass = UI_PLAYER_STATUS.Class;
	if (UI_REINFORCE.Succeeded()) TTUIReinforceClass = UI_REINFORCE.Class;
	if (UI_RECOVERY.Succeeded()) TTUIRecoveryClass = UI_RECOVERY.Class;
	if (UI_MAP.Succeeded()) TTUIMapClass = UI_MAP.Class;
	if (UI_QUITGAME.Succeeded()) TTUIQuitGameClass = UI_QUITGAME.Class;
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

	SetInputMode(FInputModeGameOnly{});
}

void ATTPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

UUserWidget* ATTPlayerController::GetUIManual() const
{
	return TTUIManual;
}

UTTUIPlayerInGame* ATTPlayerController::GetUIPlayerInGame() const
{
	return TTUIPlayerInGame;
}

UTTUIPlayerStatus* ATTPlayerController::GetUIPlayerStatus() const
{
	return TTUIPlayerStatus;
}

UTTUIReinforce* ATTPlayerController::GetUIReinforce() const
{
	return TTUIReinforce;
}

UTTUIRecovery* ATTPlayerController::GetUIRecovery() const
{
	return TTUIRecovery;
}

UTTUIMap* ATTPlayerController::GetUIMap() const
{
	return TTUIMap;
}

UTTUIQuitGame* ATTPlayerController::GetUIQuitGame() const
{
	return TTUIQuitGame;
}

void ATTPlayerController::SetUIManual()
{
	TTUIManual = CreateWidget<UUserWidget>(this, TTUIManualClass);
	TTUIManual->AddToViewport();
	TTUIManual->SetVisibility(ESlateVisibility::Hidden);
}

void ATTPlayerController::SetUIPlayerInGame(UTTCharacterStatComponent* NewCharacterStat)
{
	TTUIPlayerInGame = CreateWidget<UTTUIPlayerInGame>(this, TTUIPlayerInGameClass);
	TTUIPlayerInGame->AddToViewport();
	TTUIPlayerInGame->SetVisibility(ESlateVisibility::Hidden);

	TTUIPlayerInGame->BindCharacterStat(NewCharacterStat);
}

void ATTPlayerController::SetUIPlayerStatus(UTTCharacterStatComponent* NewCharacterStat)
{
	TTUIPlayerStatus = CreateWidget<UTTUIPlayerStatus>(this, TTUIPlayerStatusClass);
	TTUIPlayerStatus->AddToViewport();
	TTUIPlayerStatus->SetVisibility(ESlateVisibility::Hidden);

	TTUIPlayerStatus->BindCharacterStat(NewCharacterStat);
}

void ATTPlayerController::SetUIReinforce(UTTCharacterStatComponent* NewCharacterStat)
{
	TTUIReinforce = CreateWidget<UTTUIReinforce>(this, TTUIReinforceClass);
	TTUIReinforce->AddToViewport();
	TTUIReinforce->SetVisibility(ESlateVisibility::Hidden);

	TTUIReinforce->Init();
	TTUIReinforce->BindCharacterStat(NewCharacterStat);
}

void ATTPlayerController::SetUIRecovery(UTTCharacterStatComponent* NewCharacterStat)
{
	TTUIRecovery = CreateWidget<UTTUIRecovery>(this, TTUIRecoveryClass);
	TTUIRecovery->AddToViewport();
	TTUIRecovery->SetVisibility(ESlateVisibility::Hidden);

	TTUIRecovery->Init();
	TTUIRecovery->BindCharacterStat(NewCharacterStat);
}

void ATTPlayerController::SetUIMap()
{
	TTUIMap = CreateWidget<UTTUIMap>(this, TTUIMapClass);
	TTUIMap->SetWorldContext(GetWorld());
	TTUIMap->AddToViewport();
	TTUIMap->SetVisibility(ESlateVisibility::Hidden);
}

void ATTPlayerController::SetUIQuitGame()
{
	TTUIQuitGame = CreateWidget<UTTUIQuitGame>(this, TTUIQuitGameClass);
	TTUIQuitGame->AddToViewport();
	TTUIQuitGame->SetVisibility(ESlateVisibility::Hidden);
}

void ATTPlayerController::SwapDebugMode()
{
	if (!FTTWorld::bIsDebugging) FTTWorld::bIsDebugging = true;
	else FTTWorld::bIsDebugging = false;
}
