#include "TTParticleSystemComponent.h"

UTTParticleSystemComponent::UTTParticleSystemComponent()
{
}

void UTTParticleSystemComponent::AddEffect(FName EffectName, const TCHAR* EffectPath)
{
	const auto& EffectCheck{ LoadObject<UParticleSystem>(this, EffectPath) };

	if (EffectCheck->IsValidLowLevel()) Effect.Emplace(EffectName, EffectCheck);
	else TTLOG(Error, TEXT("Can't find EffectPath (%s)"), EffectPath);
}

void UTTParticleSystemComponent::PlayEffect(FName EffectName, FVector Location)
{
	if (Effect.Find(EffectName)) UGameplayStatics::SpawnEmitterAtLocation(
		this, Effect[EffectName], Location);
	else TTLOG(Error, TEXT("Can't find EffectName (%s)"), *EffectName.ToString());
}

void UTTParticleSystemComponent::PlayEffect(FName EffectName, FVector Location, float Scale)
{
	if (Effect.Find(EffectName)) UGameplayStatics::SpawnEmitterAtLocation(
		this, Effect[EffectName], Location, FRotator::ZeroRotator, FVector(Scale, Scale, Scale));
	else TTLOG(Error, TEXT("Can't find EffectName (%s)"), *EffectName.ToString());
}

void UTTParticleSystemComponent::PlayEffect(FName EffectName, FVector Location, FVector Scale)
{
	if (Effect.Find(EffectName)) UGameplayStatics::SpawnEmitterAtLocation(
		this, Effect[EffectName], Location, FRotator::ZeroRotator, Scale);
	else TTLOG(Error, TEXT("Can't find EffectName (%s)"), *EffectName.ToString());
}

void UTTParticleSystemComponent::PlayEffect(FName EffectName, FVector Location, FRotator Rotation)
{
	if (Effect.Find(EffectName)) UGameplayStatics::SpawnEmitterAtLocation(
		this, Effect[EffectName], Location, Rotation);
	else TTLOG(Error, TEXT("Can't find EffectName (%s)"), *EffectName.ToString());
}

void UTTParticleSystemComponent::PlayEffect(FName EffectName, FVector Location, FRotator Rotation, float Scale)
{
	if (Effect.Find(EffectName)) UGameplayStatics::SpawnEmitterAtLocation(
		this, Effect[EffectName], Location, Rotation, FVector(Scale, Scale, Scale));
	else TTLOG(Error, TEXT("Can't find EffectName (%s)"), *EffectName.ToString());
}

void UTTParticleSystemComponent::PlayEffect(FName EffectName, FVector Location, FRotator Rotation, FVector Scale)
{
	if (Effect.Find(EffectName)) UGameplayStatics::SpawnEmitterAtLocation(
		this, Effect[EffectName], Location, Rotation, Scale);
	else TTLOG(Error, TEXT("Can't find EffectName (%s)"), *EffectName.ToString());
}
