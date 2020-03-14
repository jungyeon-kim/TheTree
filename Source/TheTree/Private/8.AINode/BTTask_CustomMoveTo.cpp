#include "BTTask_CustomMoveTo.h"
#include "TTAIController.h"
#include "TTPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_CustomMoveTo::UBTTask_CustomMoveTo()
{
	NodeName = TEXT("CustomMoveTo");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_CustomMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result{ Super::ExecuteTask(OwnerComp, NodeMemory) };

	const auto& ControllingPawn{ OwnerComp.GetAIOwner()->GetPawn() };
	if (!ControllingPawn) return EBTNodeResult::Failed;

	const auto& Target{ Cast<ATTPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ATTAIController::TargetKey)) };
	if (!Target) return EBTNodeResult::Failed;
	
	OwnerComp.GetAIOwner()->MoveToActor(Target);

	return EBTNodeResult::InProgress;
}

void UBTTask_CustomMoveTo::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	const auto& ControllingPawn{ OwnerComp.GetAIOwner()->GetPawn() };
	if (!ControllingPawn) FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

	const auto& Target{ Cast<ATTPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ATTAIController::TargetKey)) };
	if (!Target) return FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	
	if (ControllingPawn->GetHorizontalDistanceTo(Target) <= AcceptableDistance)
	{
		OwnerComp.GetAIOwner()->StopMovement();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}