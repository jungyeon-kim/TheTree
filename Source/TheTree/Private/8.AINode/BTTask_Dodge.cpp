#include "BTTask_Dodge.h"
#include "TTAIController.h"
#include "TTEnemyBase.h"

UBTTask_Dodge::UBTTask_Dodge()
{
	NodeName = TEXT("Dodge");
	bCreateNodeInstance = true;
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Dodge::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result{ Super::ExecuteTask(OwnerComp, NodeMemory) };

	TTEnemy = Cast<ATTEnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (!TTEnemy) return EBTNodeResult::Failed;

	TTEnemy->PlayMontage(DodgeTypeName);

	bIsDodging = true;
	TTEnemy->OnDodgeEnded.AddLambda([&]()
		{
			bIsDodging = false;
		});

	return EBTNodeResult::InProgress;
}

void UBTTask_Dodge::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!bIsDodging && !TTEnemy->GetCurrentMontage())
	{
		TTEnemy->OnDodgeEnded.Clear();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}