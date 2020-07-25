#include "TTUIShelterButton.h"

UTTUIShelterButton::UTTUIShelterButton()
{
	OnClicked.AddDynamic(this, &UTTUIShelterButton::OnClickEvent);
}

void UTTUIShelterButton::OnClickEvent()
{
	Super::OnClickEvent();
	UGameplayStatics::OpenLevel(GetWorld(), FName{ "Common_Battle_00" });
}