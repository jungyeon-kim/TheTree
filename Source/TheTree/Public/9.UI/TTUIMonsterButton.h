#pragma once

#include "9.UI/TTUIMapButton.h"
#include "TTUIMonsterButton.generated.h"

UCLASS()
class THETREE_API UTTUIMonsterButton : public UTTUIMapButton
{
	GENERATED_BODY()

	UTTUIMonsterButton();
public:
	virtual void OnClickEvent() override;
};
