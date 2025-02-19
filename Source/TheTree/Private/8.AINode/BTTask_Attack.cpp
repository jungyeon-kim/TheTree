#include "BTTask_Attack.h"
#include "TTAIController.h"
#include "TTEnemyBase.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("Attack");
	bCreateNodeInstance = true;
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result{ Super::ExecuteTask(OwnerComp, NodeMemory) };
	
	TTEnemy = Cast<ATTEnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (!TTEnemy) return EBTNodeResult::Failed;

	TTEnemy->PlayMontage(AttackTypeName);

	bIsAttacking = true;
	TTEnemy->OnAttackEnded.AddLambda([&]()
	{
		bIsAttacking = false;
	});

	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!bIsAttacking && !TTEnemy->GetCurrentMontage())
	{
		TTEnemy->OnAttackEnded.Clear();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
