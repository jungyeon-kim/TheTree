#pragma once

#include "TheTree.h"
#include "Particles/ParticleSystemComponent.h"
#include "TTParticleSystemComponent.generated.h"

UCLASS()
class THETREE_API UTTParticleSystemComponent : public UParticleSystemComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere, Category = "Effect")
	TMap<FName, UParticleSystem*> Effect;
	UPROPERTY(VisibleAnywhere, Category = "Effect")
	TMap<FName, UParticleSystemComponent*> ManagedEffect;
public:
	UTTParticleSystemComponent();
	
	void AddEffect(FName EffectName, const TCHAR* EffectPath);
	void AddManagedEffect(FName EffectName, UParticleSystemComponent* Effect);
	void DeleteManagedEffect(FName EffectName);

	UParticleSystemComponent* PlayEffectAtLocation(FName EffectName, FVector Location);
	UParticleSystemComponent* PlayEffectAtLocation(FName EffectName, FVector Location, float Scale);
	UParticleSystemComponent* PlayEffectAtLocation(FName EffectName, FVector Location, FVector Scale);
	UParticleSystemComponent* PlayEffectAtLocation(FName EffectName, FVector Location, FRotator Rotation);
	UParticleSystemComponent* PlayEffectAtLocation(FName EffectName, FVector Location, FRotator Rotation, float Scale);
	UParticleSystemComponent* PlayEffectAtLocation(FName EffectName, FVector Location, FRotator Rotation, FVector Scale);

	UParticleSystemComponent* PlayEffectAttached(FName EffectName, USceneComponent* AttachToComponent, FVector Location);
	UParticleSystemComponent* PlayEffectAttached(FName EffectName, USceneComponent* AttachToComponent, FVector Location, float Scale);
	UParticleSystemComponent* PlayEffectAttached(FName EffectName, USceneComponent* AttachToComponent, FVector Location, FVector Scale);
	UParticleSystemComponent* PlayEffectAttached(FName EffectName, USceneComponent* AttachToComponent, FVector Location, FRotator Rotation);
	UParticleSystemComponent* PlayEffectAttached(FName EffectName, USceneComponent* AttachToComponent, FVector Location, FRotator Rotation, float Scale);
	UParticleSystemComponent* PlayEffectAttached(FName EffectName, USceneComponent* AttachToComponent, FVector Location, FRotator Rotation, FVector Scale);
};