#include "TTParticleSystemComponent.h"

UTTParticleSystemComponent::UTTParticleSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = false;
}

void UTTParticleSystemComponent::AddEffect(FName EffectName, const TCHAR* EffectPath)
{
	const auto& EffectCheck{ LoadObject<UParticleSystem>(this, EffectPath) };

	if (EffectCheck->IsValidLowLevel()) Effect.Emplace(EffectName, EffectCheck);
	else TTLOG(Error, TEXT("Can't find EffectPath (%s)"), EffectPath);
}

void UTTParticleSystemComponent::AddManagedEffect(FName EffectName, UParticleSystemComponent* TargetEffect)
{
	if (!ManagedEffect.Find(EffectName)) ManagedEffect.Emplace(EffectName, TargetEffect);
	else TTLOG(Error, TEXT("EffectName is duplicated. (%s)"), *EffectName.ToString());
}

void UTTParticleSystemComponent::DeleteManagedEffect(FName EffectName)
{
	if (ManagedEffect.Find(EffectName))
	{
		if (ManagedEffect[EffectName]) ManagedEffect[EffectName]->Deactivate();
		ManagedEffect.Remove(EffectName);
	}
	else TTLOG(Error, TEXT("Can't find EffectName (%s)"), *EffectName.ToString());
}

UParticleSystemComponent* UTTParticleSystemComponent::PlayEffectAtLocation(FName EffectName, FVector Location)
{
	if (Effect.Find(EffectName))
	{
		const auto& Particle{ UGameplayStatics::SpawnEmitterAtLocation(this, Effect[EffectName], Location) };
		return Particle;
	}
	else
	{
		TTLOG(Error, TEXT("Can't find EffectName (%s)"), *EffectName.ToString());
		return nullptr;
	}
}

UParticleSystemComponent* UTTParticleSystemComponent::PlayEffectAtLocation(FName EffectName, FVector Location, float Scale)
{
	if (Effect.Find(EffectName))
	{
		const auto& Particle{ UGameplayStatics::SpawnEmitterAtLocation(
			this, Effect[EffectName], Location, FRotator::ZeroRotator, FVector(Scale, Scale, Scale)) };
		return Particle;
	}
	else
	{
		TTLOG(Error, TEXT("Can't find EffectName (%s)"), *EffectName.ToString());
		return nullptr;
	}
}

UParticleSystemComponent* UTTParticleSystemComponent::PlayEffectAtLocation(FName EffectName, FVector Location, FVector Scale)
{
	if (Effect.Find(EffectName))
	{
		const auto& Particle{ UGameplayStatics::SpawnEmitterAtLocation(
			this, Effect[EffectName], Location, FRotator::ZeroRotator, Scale) };
		return Particle;
	}
	else
	{
		TTLOG(Error, TEXT("Can't find EffectName (%s)"), *EffectName.ToString());
		return nullptr;
	}
}

UParticleSystemComponent* UTTParticleSystemComponent::PlayEffectAtLocation(FName EffectName, FVector Location, FRotator Rotation)
{
	if (Effect.Find(EffectName))
	{
		const auto& Particle{ UGameplayStatics::SpawnEmitterAtLocation(this, Effect[EffectName], Location, Rotation) };
		return Particle;
	}
	else
	{
		TTLOG(Error, TEXT("Can't find EffectName (%s)"), *EffectName.ToString());
		return nullptr;
	}
}

UParticleSystemComponent* UTTParticleSystemComponent::PlayEffectAtLocation(FName EffectName, FVector Location, FRotator Rotation, float Scale)
{
	if (Effect.Find(EffectName))
	{
		const auto& Particle{ UGameplayStatics::SpawnEmitterAtLocation(
			this, Effect[EffectName], Location, Rotation, FVector(Scale, Scale, Scale)) };
		return Particle;
	}
	else
	{
		TTLOG(Error, TEXT("Can't find EffectName (%s)"), *EffectName.ToString());
		return nullptr;
	}
}

UParticleSystemComponent* UTTParticleSystemComponent::PlayEffectAtLocation(FName EffectName, FVector Location, FRotator Rotation, FVector Scale)
{
	if (Effect.Find(EffectName))
	{
		const auto& Particle{ UGameplayStatics::SpawnEmitterAtLocation(
			this, Effect[EffectName], Location, Rotation, Scale) };
		return Particle;
	}
	else
	{
		TTLOG(Error, TEXT("Can't find EffectName (%s)"), *EffectName.ToString());
		return nullptr;
	}
}

UParticleSystemComponent* UTTParticleSystemComponent::PlayEffectAttached(FName EffectName, USceneComponent* AttachToComponent, FVector Location)
{
	if (Effect.Find(EffectName))
	{
		const auto& Particle{ UGameplayStatics::SpawnEmitterAttached(Effect[EffectName], AttachToComponent,
			NAME_None, Location) };
		return Particle;
	}
	else
	{
		TTLOG(Error, TEXT("Can't find EffectName (%s)"), *EffectName.ToString());
		return nullptr;
	}
}

UParticleSystemComponent* UTTParticleSystemComponent::PlayEffectAttached(FName EffectName, USceneComponent* AttachToComponent, FVector Location, float Scale)
{
	if (Effect.Find(EffectName))
	{
		const auto& Particle{ UGameplayStatics::SpawnEmitterAttached(Effect[EffectName], AttachToComponent,
			NAME_None, Location, FRotator::ZeroRotator, FVector(Scale, Scale, Scale)) };
		return Particle;
	}
	else
	{
		TTLOG(Error, TEXT("Can't find EffectName (%s)"), *EffectName.ToString());
		return nullptr;
	}
}

UParticleSystemComponent* UTTParticleSystemComponent::PlayEffectAttached(FName EffectName, USceneComponent* AttachToComponent, FVector Location, FVector Scale)
{
	if (Effect.Find(EffectName))
	{
		const auto& Particle{ UGameplayStatics::SpawnEmitterAttached(Effect[EffectName], AttachToComponent,
			NAME_None, Location, FRotator::ZeroRotator, Scale) };
		return Particle;
	}
	else
	{
		TTLOG(Error, TEXT("Can't find EffectName (%s)"), *EffectName.ToString());
		return nullptr;
	}
}

UParticleSystemComponent* UTTParticleSystemComponent::PlayEffectAttached(FName EffectName, USceneComponent* AttachToComponent, FVector Location, FRotator Rotation)
{
	if (Effect.Find(EffectName))
	{
		const auto& Particle{ UGameplayStatics::SpawnEmitterAttached(Effect[EffectName], AttachToComponent,
			NAME_None, Location, Rotation) };
		return Particle;
	}
	else
	{
		TTLOG(Error, TEXT("Can't find EffectName (%s)"), *EffectName.ToString());
		return nullptr;
	}
}

UParticleSystemComponent* UTTParticleSystemComponent::PlayEffectAttached(FName EffectName, USceneComponent* AttachToComponent, FVector Location, FRotator Rotation, float Scale)
{
	if (Effect.Find(EffectName))
	{
		const auto& Particle{ UGameplayStatics::SpawnEmitterAttached(Effect[EffectName], AttachToComponent,
			NAME_None, Location, Rotation, FVector(Scale, Scale, Scale)) };
		return Particle;
	}
	else
	{
		TTLOG(Error, TEXT("Can't find EffectName (%s)"), *EffectName.ToString());
		return nullptr;
	}
}

UParticleSystemComponent* UTTParticleSystemComponent::PlayEffectAttached(FName EffectName, USceneComponent* AttachToComponent, FVector Location, FRotator Rotation, FVector Scale)
{
	if (Effect.Find(EffectName))
	{
		const auto& Particle{ UGameplayStatics::SpawnEmitterAttached(Effect[EffectName], AttachToComponent,
			NAME_None, Location, Rotation, Scale) };
		return Particle;
	}
	else
	{
		TTLOG(Error, TEXT("Can't find EffectName (%s)"), *EffectName.ToString());
		return nullptr;
	}
}