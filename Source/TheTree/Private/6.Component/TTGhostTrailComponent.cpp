#include "TTGhostTrailComponent.h"
#include "TTGhostTrail.h"

void UTTGhostTrailComponent::SetGhostTrail(const TCHAR * MaterialPath, USkeletalMeshComponent * TargetPose)
{
	if (!MaterialPath || !TargetPose)
		return;

	UMaterialInterface* TempMat{ Cast<UMaterialInterface>
	(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, MaterialPath)) };
	
	if (!TempMat)
		return;
	
	Material = TempMat;
	Pose = TargetPose;
}

void UTTGhostTrailComponent::PlayGhostTrail(float Interval, float Length)
{
	CurrentLength = Length;
	LoopInterval = Interval;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UTTGhostTrailComponent::DoWork, Interval, true);
}

void UTTGhostTrailComponent::StopGhostTrail()
{
	FTimerManager& Manager{ GetWorld()->GetTimerManager() };
	if (Manager.IsTimerActive(TimerHandle))
		Manager.ClearTimer(TimerHandle);	
}

void UTTGhostTrailComponent::DoWork()
{
	if (CurrentLength > 0.0f)
	{
		FActorSpawnParameters Param{};
		Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ATTGhostTrail* Trail{ GetWorld()->SpawnActor<ATTGhostTrail>(ATTGhostTrail::StaticClass(),
			Pose->GetComponentTransform(), Param) };
		Trail->SetGhostTrail(Pose, Material);
		Trail->StartTrail();
		CurrentLength -= LoopInterval;
	}
	else 
		StopGhostTrail();
}