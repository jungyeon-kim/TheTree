#pragma once

#include "9.UI/TTUIMapButton.h"
#include "TTUIShelterButton.generated.h"

UCLASS()
class THETREE_API UTTUIShelterButton : public UTTUIMapButton
{
	GENERATED_BODY()

	UTTUIShelterButton();
public:
	virtual void OnClickEvent() override;
};
