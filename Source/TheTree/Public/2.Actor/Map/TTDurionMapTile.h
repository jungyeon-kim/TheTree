#pragma once

#include "TheTree.h"
#include "GameFramework/Actor.h"
#include "TTDurionMapTile.generated.h"

UCLASS()
class THETREE_API ATTDurionMapTile : public AActor
{
	GENERATED_BODY()
	
public:	
	ATTDurionMapTile();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* MapMesh;
};
