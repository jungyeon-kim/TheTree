#include "BTDecorator_IsRandomValue.h"

UBTDecorator_IsRandomValue::UBTDecorator_IsRandomValue()
{
	NodeName = TEXT("IsRandomValue");
}

bool UBTDecorator_IsRandomValue::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult{ Super::CalculateRawConditionValue(OwnerComp, NodeMemory) };
	
	int32 Value{ FMath::RandRange(0, RandomRange) };
	bResult = RandomValue == Value;

	return bResult;
}
