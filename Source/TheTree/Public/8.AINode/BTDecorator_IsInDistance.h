#pragma once

#include "TheTree.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsInDistance.generated.h"

UCLASS()
class THETREE_API UBTDecorator_IsInDistance : public UBTDecorator
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "Option")
	float Distance;
public:
	UBTDecorator_IsInDistance();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
