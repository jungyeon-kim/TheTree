#include "BTDecorator_IsInAttackRange.h"
#include "TTAIController.h"
#include "TTPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{
	NodeName = TEXT("CanAttack");
	InAttackRange = 400.0f;
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult{ Super::CalculateRawConditionValue(OwnerComp, NodeMemory) };

	const auto& ControllingPawn{ OwnerComp.GetAIOwner()->GetPawn() };
	if (!ControllingPawn) return false;

	const auto& Target{ Cast<ATTPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ATTAIController::TargetKey)) };
	if (!Target) return false;

	bResult = Target->GetDistanceTo(ControllingPawn) <= InAttackRange;
	return bResult;
}
