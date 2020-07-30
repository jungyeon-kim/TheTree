#include "TTLobbyLevel.h"
#include "TTGameInstance.h"

ATTLobbyLevel::ATTLobbyLevel()
{

}

void ATTLobbyLevel::BeginPlay()
{
	Super::BeginPlay();

	UTTGameInstance* TTGameInstance{ Cast<UTTGameInstance>(GetGameInstance()) };
	if(TTGameInstance)
		TTGameInstance->SetClearTrooper(false);
}