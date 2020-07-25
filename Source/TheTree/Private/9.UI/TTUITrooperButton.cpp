#include "TTUITrooperButton.h"

UTTUITrooperButton::UTTUITrooperButton()
{
	OnClicked.AddDynamic(this, &UTTUITrooperButton::OnClickEvent);
}

void UTTUITrooperButton::OnClickEvent()
{
	Super::OnClickEvent();
	//UGameplayStatics::OpenLevel(WorldContext, FName{"Common_Battle_00"});
}