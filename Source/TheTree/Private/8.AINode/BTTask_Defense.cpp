#include "BTTask_Defense.h"
#include "TTAIController.h"
#include "TTEnemyBase.h"
#include "TTCharacterStatComponent.h"

UBTTask_Defense::UBTTask_Defense()
{
	NodeName = TEXT("Defense");
	bCreateNodeInstance = true;
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Defense::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result{ Super::ExecuteTask(OwnerComp, NodeMemory) };

	TTEnemy = Cast<ATTEnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (!TTEnemy) return EBTNodeResult::Failed;

	float PrevDef{ TTEnemy->CharacterStat->GetDef() };

	TTEnemy->CharacterStat->SetDef(Def);
	TTEnemy->PlayMontage(DefenseTypeName);

	bIsDefensing = true;
	TTEnemy->OnDefenseEnded.AddLambda([=]()
	{
		TTEnemy->CharacterStat->SetDef(PrevDef);
		bIsDefensing = false;
	});

	return EBTNodeResult::InProgress;
}

void UBTTask_Defense::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!bIsDefensing && !TTEnemy->GetCurrentMontage())
	{
		TTEnemy->OnDefenseEnded.Clear();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}