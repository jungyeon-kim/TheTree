#pragma once

#include "9.UI/TTUIMapButton.h"
#include "TTUITrooperButton.generated.h"

UCLASS()
class THETREE_API UTTUITrooperButton : public UTTUIMapButton
{
	GENERATED_BODY()

	UTTUITrooperButton();
public:
	virtual void OnClickEvent() override;
};
