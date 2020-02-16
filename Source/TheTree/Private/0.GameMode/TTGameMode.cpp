#include "TTGameMode.h"
#include "TTPlayerController.h"
#include "TTPlayer.h"

ATTGameMode::ATTGameMode()
{
	PlayerControllerClass = ATTPlayerController::StaticClass();
	DefaultPawnClass = ATTPlayer::StaticClass();
}

void ATTGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ATTGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}
