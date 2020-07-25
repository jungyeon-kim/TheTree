#include "TTUITrooperButton.h"

UTTUITrooperButton::UTTUITrooperButton()
{
	OnClicked.AddDynamic(this, &UTTUITrooperButton::OnClickEvent);
}

void UTTUITrooperButton::OnClickEvent()
{
	Super::OnClickEvent();
	UGameplayStatics::OpenLevel(WorldContext, FName{"Trooper_Battle"});
}