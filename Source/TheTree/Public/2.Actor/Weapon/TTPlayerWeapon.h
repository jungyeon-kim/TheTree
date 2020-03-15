#pragma once

#include "TheTree.h"
#include "GameFramework/Actor.h"
#include "TTPlayerWeapon.generated.h"

UCLASS()
class THETREE_API ATTPlayerWeapon : public AActor
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
public:
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	UStaticMeshComponent* Weapon;
public:	
	ATTPlayerWeapon();

	virtual void Tick(float DeltaTime) override;
};
