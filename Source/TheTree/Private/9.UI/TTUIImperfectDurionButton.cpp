#include "TTUIImperfectDurionButton.h"

UTTUIImperfectDurionButton::UTTUIImperfectDurionButton()
{
	OnClicked.AddDynamic(this, &UTTUIImperfectDurionButton::OnClickEvent);
}

void UTTUIImperfectDurionButton::OnClickEvent()
{
	Super::OnClickEvent();
	UGameplayStatics::OpenLevel(WorldContext, FName{ "ImperfectDurion_Battle" });
}