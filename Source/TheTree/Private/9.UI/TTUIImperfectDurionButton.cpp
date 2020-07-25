#include "TTUIImperfectDurionButton.h"
#include "TTBaseLevel.h"

UTTUIImperfectDurionButton::UTTUIImperfectDurionButton()
{
	OnClicked.AddDynamic(this, &UTTUIImperfectDurionButton::OnClickEvent);
}

void UTTUIImperfectDurionButton::OnClickEvent()
{
	ATTBaseLevel* Level{ Cast<ATTBaseLevel>(WorldContext->GetLevelScriptActor()) };
	if (!Level || Level->GetMonsterCount() > 0)
		return;

	Super::OnClickEvent();
	UGameplayStatics::OpenLevel(WorldContext, FName{ "ImperfectDurion_Battle" });
}