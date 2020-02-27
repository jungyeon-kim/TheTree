#include "BTService_Detect.h"
#include "TTAIController.h"
#include "TTBasicEnemy.h"
#include "TTPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 0.5f;
	DetectRadius = 1200.0f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const auto& TTEnemy{ Cast<ATTBasicEnemy>(OwnerComp.GetAIOwner()->GetPawn()) };
	if (!TTEnemy) return;

	const auto& World{ TTEnemy->GetWorld() };
	FVector Center{ TTEnemy->GetActorLocation() };
	if (!World) return;

	TArray<FOverlapResult> OverlapResults{};
	FCollisionQueryParams CollisionQueryParam{ NAME_None, false, TTEnemy };

	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel4,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	if (bResult)
		for (const auto& OverlapResult : OverlapResults)
		{
			const auto& TTPlayer{ Cast<ATTPlayer>(OverlapResult.GetActor()) };

			if (TTPlayer)
			{
				TTEnemy->SetCharacterState(ECharacterState::BATTLE);
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(ATTAIController::TargetKey, TTPlayer);
				if (FTTDebug::bIsDebugging) DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Blue, false, 0.2f);
				return;
			}
		}
	else
	{
		TTEnemy->SetCharacterState(ECharacterState::NOBATTLE);
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(ATTAIController::TargetKey, nullptr);
		if (FTTDebug::bIsDebugging) DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
	}

}