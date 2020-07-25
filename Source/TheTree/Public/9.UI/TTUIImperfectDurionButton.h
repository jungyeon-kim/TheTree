#pragma once

#include "9.UI/TTUIMapButton.h"
#include "TTUIImperfectDurionButton.generated.h"

UCLASS()
class THETREE_API UTTUIImperfectDurionButton : public UTTUIMapButton
{
	GENERATED_BODY()

	UTTUIImperfectDurionButton();
public:
	virtual void OnClickEvent() override;
};
