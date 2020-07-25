#include "TTUITrooperButton.h"
#include "TTBaseLevel.h"

UTTUITrooperButton::UTTUITrooperButton()
{
	OnClicked.AddDynamic(this, &UTTUITrooperButton::OnClickEvent);
}

void UTTUITrooperButton::OnClickEvent()
{
	ATTBaseLevel* Level{ Cast<ATTBaseLevel>(WorldContext->GetLevelScriptActor()) };
	if (!Level || Level->GetMonsterCount() > 0)
		return;

	Super::OnClickEvent();
	UGameplayStatics::OpenLevel(WorldContext, FName{"Trooper_Battle"});
}