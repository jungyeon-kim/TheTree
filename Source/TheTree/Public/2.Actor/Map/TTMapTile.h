#pragma once

#include "TheTree.h"
#include "GameFramework/Actor.h"
#include "TTMapTile.generated.h"

UCLASS()
class THETREE_API ATTMapTile : public AActor
{
	GENERATED_BODY()
	
public:	
	ATTMapTile();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* MapMesh;
};
