#include "TTUIStoreButton.h"
#include "TTBaseLevel.h"

UTTUIStoreButton::UTTUIStoreButton()
{
	OnClicked.AddDynamic(this, &UTTUIStoreButton::OnClickEvent);
}

void UTTUIStoreButton::OnClickEvent()
{
	ATTBaseLevel* Level{ Cast<ATTBaseLevel>(WorldContext->GetLevelScriptActor()) };
	if (!Level || Level->GetMonsterCount() > 0)
		return;

	Super::OnClickEvent();
	Level->PlayCinematic(nullptr, false, FName{ "Store" });
}