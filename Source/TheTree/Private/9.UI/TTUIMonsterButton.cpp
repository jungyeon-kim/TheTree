#include "TTUIMonsterButton.h"
#include "TheTree.h"
UTTUIMonsterButton::UTTUIMonsterButton()
{
	OnClicked.AddDynamic(this, &UTTUIMonsterButton::OnClickEvent);
}

void UTTUIMonsterButton::OnClickEvent()
{
	Super::OnClickEvent();
	UGameplayStatics::OpenLevel(WorldContext, FName{"Common_Battle_00"});
}