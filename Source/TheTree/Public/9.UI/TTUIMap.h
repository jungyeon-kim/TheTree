#pragma once

#include "TheTree.h"
#include "Blueprint/UserWidget.h"
#include "TTUIMap.generated.h"

UCLASS()
class THETREE_API UTTUIMap : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual bool Initialize() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	//UPROPERTY()
	//TArray<class UButton*> ButtonCluster{};
};
