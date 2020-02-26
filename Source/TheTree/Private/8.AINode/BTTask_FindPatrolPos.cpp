#include "BTTask_FindPatrolPos.h"
#include "TTAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
	NodeName = TEXT("FindPatrolPos");
	DetectRadius = 800.0f;
}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result{ Super::ExecuteTask(OwnerComp, NodeMemory) };

	const auto& ControllingPawn{ OwnerComp.GetAIOwner()->GetPawn() };
	if (!ControllingPawn) return EBTNodeResult::Failed;

	const auto& NavSystem{ UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld()) };
	if (!NavSystem) return EBTNodeResult::Failed;

	FVector Origin{ OwnerComp.GetBlackboardComponent()->GetValueAsVector(ATTAIController::HomePosKey) };
	FNavLocation NextPatrol{};

	if (NavSystem->GetRandomPointInNavigableRadius(Origin, DetectRadius, NextPatrol))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(ATTAIController::PatrolPosKey, NextPatrol.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
