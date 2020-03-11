#pragma once

#include "TheTree.h"
#include "BehaviorTree/BTCompositeNode.h"
#include "BTCompositeNode_Continue.generated.h"

UCLASS()
class THETREE_API UBTCompositeNode_Continue : public UBTCompositeNode
{
	GENERATED_BODY()
private:
	virtual int32 GetNextChildHandler(struct FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const override;
public:
	UBTCompositeNode_Continue(const FObjectInitializer& ObjectInitializer);
};
