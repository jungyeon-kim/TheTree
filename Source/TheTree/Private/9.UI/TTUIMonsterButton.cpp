#include "TTUIMonsterButton.h"
#include "TTGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "TTBaseLevel.h"

UTTUIMonsterButton::UTTUIMonsterButton()
{
	OnClicked.AddDynamic(this, &UTTUIMonsterButton::OnClickEvent);
}

void UTTUIMonsterButton::OnClickEvent()
{
	ATTBaseLevel* Level{ Cast<ATTBaseLevel>(WorldContext->GetLevelScriptActor()) };
	if (!Level || Level->GetMonsterCount() > 0)
		return;

	Super::OnClickEvent();
	Level->PlayCinematic(nullptr, false, FName{ "Common_Battle_Durion" });
}