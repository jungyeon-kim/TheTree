#include "BTTask_CustomMoveTo.h"
#include "TTAIController.h"
#include "TTPlayer.h"
#include "TTEnemyBase.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_CustomMoveTo::UBTTask_CustomMoveTo()
{
	NodeName = TEXT("CustomMoveTo");
	bCreateNodeInstance = true;
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_CustomMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result{ Super::ExecuteTask(OwnerComp, NodeMemory) };

	TTEnemy = Cast<ATTEnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (!TTEnemy) return EBTNodeResult::Failed;
	
	Target = Cast<ATTPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ATTAIController::TargetKey));
	if (!Target) return EBTNodeResult::Failed;

	if (!TTEnemy->GetCurrentMontage()) OwnerComp.GetAIOwner()->MoveToActor(Target);

	return EBTNodeResult::InProgress;
}

void UBTTask_CustomMoveTo::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!bIsMoving && !TTEnemy->GetCurrentMontage())
	{
		OwnerComp.GetAIOwner()->MoveToActor(Target);
		bIsMoving = true;
	}
	
	if (TTEnemy->GetHorizontalDistanceTo(Target) <= AcceptableDistance)
	{
		OwnerComp.GetAIOwner()->StopMovement();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}