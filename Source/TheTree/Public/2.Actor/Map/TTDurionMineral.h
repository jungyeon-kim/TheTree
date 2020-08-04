#pragma once

#include "TheTree.h"
#include "GameFramework/Actor.h"
#include "TTDurionMineral.generated.h"

UCLASS()
class THETREE_API ATTDurionMineral : public AActor
{
	GENERATED_BODY()
	
public:	
	ATTDurionMineral();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* MineralMesh;
};
