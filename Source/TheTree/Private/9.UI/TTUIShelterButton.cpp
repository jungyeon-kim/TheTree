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
	Level->PlayCinematic(nullptr, false, FName{ "Shelter" });
}