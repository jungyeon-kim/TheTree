#include "BTDecorator_IsMontagePlaying.h"
#include "TTAIController.h"

UBTDecorator_IsMontagePlaying::UBTDecorator_IsMontagePlaying()
{
	NodeName = TEXT("IsMontagePlaying");
}

bool UBTDecorator_IsMontagePlaying::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult{ Super::CalculateRawConditionValue(OwnerComp, NodeMemory) };

	const auto& ControllingCharacter{ OwnerComp.GetAIOwner()->GetCharacter() };
	if (!ControllingCharacter) return false;

	bResult = StaticCast<bool>(ControllingCharacter->GetCurrentMontage());

	return bResult;
}
