#pragma once

#include "TheTree.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsRandomValue.generated.h"

UCLASS()
class THETREE_API UBTDecorator_IsRandomValue : public UBTDecorator
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "Option")
	int32 RandomValue;
	UPROPERTY(EditAnywhere, Category = "Option")
	int32 RandomRange;
public:
	UBTDecorator_IsRandomValue();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
