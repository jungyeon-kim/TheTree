#include "BTTask_TurnToTarget.h"
#include "TTAIController.h"
#include "TTBasicEnemy.h"
#include "TTPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("Turn");
	InterpSpeed = 10.0f;
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result{ Super::ExecuteTask(OwnerComp, NodeMemory) };

	const auto& TTEnemy{ Cast<ATTBasicEnemy>(OwnerComp.GetAIOwner()->GetPawn()) };
	if (!TTEnemy) return EBTNodeResult::Failed;

	const auto& Target{ Cast<ATTPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ATTAIController::TargetKey)) };
	if (!Target) return EBTNodeResult::Failed;

	FVector ForwardVector{ Target->GetActorLocation() - TTEnemy->GetActorLocation() };
	FRotator TargetRot{ FRotationMatrix::MakeFromX(ForwardVector.GetSafeNormal2D()).Rotator() };
	TTEnemy->SetActorRotation(
		FMath::RInterpTo(TTEnemy->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), InterpSpeed));

	return EBTNodeResult::Succeeded;
}