#include "BTDecorator_IsInDistance.h"
#include "TTAIController.h"
#include "TTPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsInDistance::UBTDecorator_IsInDistance()
{
	NodeName = TEXT("IsInDistance");
	Distance = 400.0f;
}

bool UBTDecorator_IsInDistance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult{ Super::CalculateRawConditionValue(OwnerComp, NodeMemory) };

	const auto& ControllingPawn{ OwnerComp.GetAIOwner()->GetPawn() };
	if (!ControllingPawn) return false;

	const auto& Target{ Cast<ATTPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ATTAIController::TargetKey)) };
	if (!Target) return false;

	bResult = Target->GetHorizontalDistanceTo(ControllingPawn) <= Distance;
	return bResult;
}
