#include "TTUIShelterButton.h"
#include "TTBaseLevel.h"
UTTUIShelterButton::UTTUIShelterButton()
{
	OnClicked.AddDynamic(this, &UTTUIShelterButton::OnClickEvent);
}

void UTTUIShelterButton::OnClickEvent()
{
	ATTBaseLevel* Level{ Cast<ATTBaseLevel>(WorldContext->GetLevelScriptActor()) };
	if (!Level || Level->GetMonsterCount() > 0)
		return;

	Super::OnClickEvent();
	UGameplayStatics::OpenLevel(GetWorld(), FName{ "Common_Battle_00" });
}