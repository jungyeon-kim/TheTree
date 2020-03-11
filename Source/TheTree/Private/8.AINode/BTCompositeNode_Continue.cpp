#include "BTCompositeNode_Continue.h"

UBTCompositeNode_Continue::UBTCompositeNode_Continue(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Continue";
}

int32 UBTCompositeNode_Continue::GetNextChildHandler(FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const
{
	int32 NextChildIdx = BTSpecialChild::ReturnToParent;

	if (PrevChild == BTSpecialChild::NotInitialized)
	{
		NextChildIdx = 0;
	}
	else if (LastResult == EBTNodeResult::Succeeded && (PrevChild + 1) < GetChildrenNum() ||
		LastResult == EBTNodeResult::Failed && (PrevChild + 1) < GetChildrenNum())
	{
		NextChildIdx = PrevChild + 1;
	}

	return NextChildIdx;
}