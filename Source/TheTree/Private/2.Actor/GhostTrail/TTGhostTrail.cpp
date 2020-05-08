#include "TTGhostTrail.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/PoseableMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

ATTGhostTrail::ATTGhostTrail()
{
	PrimaryActorTick.bCanEverTick = true;
	TrailMesh = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("TRAILMESH"));
	RootComponent = TrailMesh;

	TrailCurve = CreateDefaultSubobject<UCurveFloat>(TEXT("TRAILCURVE"));
	TrailCurve->FloatCurve.AddKey(0.0f, 1.0f);
	TrailCurve->FloatCurve.AddKey(0.2f, 0.0f);
}

void ATTGhostTrail::BeginPlay()
{
	Super::BeginPlay();
}

void ATTGhostTrail::BeginDestroy()
{
	if (TrailTimeline.IsPlaying())
		TrailTimeline.Stop();
	Super::BeginDestroy();
}

void ATTGhostTrail::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TrailTimeline.TickTimeline(DeltaTime);
}

void ATTGhostTrail::SetGhostTrail(USkeletalMeshComponent* Pose, UMaterialInterface* Mat)
{
	TrailMesh->SetSkeletalMesh(Pose->SkeletalMesh);
	TrailMesh->CopyPoseFromSkeletalComponent(Pose);
	MatInstanceDynamic = UMaterialInstanceDynamic::Create(Mat, this);
	TArray<FSkeletalMaterial> Materials = Pose->SkeletalMesh->Materials;

	for (int Index = 0; Index < Materials.Num(); ++Index) 
		TrailMesh->SetMaterial(Index, MatInstanceDynamic);
}
void ATTGhostTrail::StartTrail()
{
	if (TrailCurve && TrailMesh)
	{
		FOnTimelineFloat TimelineCallback{};
		FOnTimelineEventStatic TimelineEndCallback{};

		TimelineCallback.BindUFunction(this, FName{ "PlayingTimeline" });
		TimelineEndCallback.BindUFunction(this, FName{ "EndTimeline" });

		TrailTimeline.SetLooping(false);
		TrailTimeline.AddInterpFloat(TrailCurve, TimelineCallback);
		TrailTimeline.SetTimelineFinishedFunc(TimelineEndCallback);

		TrailTimeline.PlayFromStart();
	}
	else
		TTLOG(Error, TEXT("Can't Start Trail ( Not Initialize )"));
}

void ATTGhostTrail::StopTimeline()
{
	TrailTimeline.Stop();
}

void ATTGhostTrail::PlayingTimeline(float CurrentTime)
{
	MatInstanceDynamic->SetScalarParameterValue(TEXT("Opacity"), CurrentTime);
}

void ATTGhostTrail::EndTimeline()
{
	Destroy();
}