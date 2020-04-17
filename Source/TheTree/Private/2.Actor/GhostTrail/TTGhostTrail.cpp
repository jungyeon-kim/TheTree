#include "TTGhostTrail.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/PoseableMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

ATTGhostTrail::ATTGhostTrail()
{
	PrimaryActorTick.bCanEverTick = true;

	TrailMesh = CreateDefaultSubobject <UPoseableMeshComponent>(TEXT("TRAILMESH"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> M_POSEMATERIAL
	{ TEXT("/Game/Assets/Effect/Material/M_Player_Ghost_Trail.M_Player_Ghost_Trail") };

	if (M_POSEMATERIAL.Succeeded())
		TrailMaterial = M_POSEMATERIAL.Object;

	RootComponent = TrailMesh;

	TrailCurve = CreateDefaultSubobject<UCurveFloat>(TEXT("TRAILCURVE"));
	TrailCurve->FloatCurve.AddKey(0.0f, 1.0f);
	TrailCurve->FloatCurve.AddKey(0.2f, 0.0f);
}

void ATTGhostTrail::BeginPlay()
{
	Super::BeginPlay();
}

void ATTGhostTrail::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TrailTimeline.TickTimeline(DeltaTime);
}

void ATTGhostTrail::SetSkeletalMesh(class USkeletalMeshComponent* Target)
{
	TrailMesh->SetSkeletalMesh(Target->SkeletalMesh);
	TrailMesh->CopyPoseFromSkeletalComponent(Target);
	MatInstanceDynamic = UMaterialInstanceDynamic::Create(TrailMaterial, this);
	TArray<FSkeletalMaterial> Materials = Target->SkeletalMesh->Materials;

	for (int Index = 0; Index < Materials.Num(); ++Index) 
		TrailMesh->SetMaterial(Index, MatInstanceDynamic);
}

void ATTGhostTrail::StartTrail()
{
	if (TrailCurve && TrailMaterial)
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

void ATTGhostTrail::PlayingTimeline(float CurrentTime)
{
	MatInstanceDynamic->SetScalarParameterValue(TEXT("Opacity"), CurrentTime);
}

void ATTGhostTrail::EndTimeline()
{
	Destroy();
}

void PlayGhostTrail(USkeletalMeshComponent* Component)
{
	FActorSpawnParameters Param{};
	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ATTGhostTrail* Trail{ Component->GetWorld()->SpawnActor<ATTGhostTrail>
		(ATTGhostTrail::StaticClass(), Component->GetComponentTransform(), Param) };
	Trail->SetSkeletalMesh(Component); 
	Trail->StartTrail();
}

void PlayGhostTrail(USkeletalMeshComponent* Component, float Interval, float Length)
{
	ATTGhostTrailLoop* TrailLoop{ Component->GetWorld()->SpawnActor<ATTGhostTrailLoop>
		(ATTGhostTrailLoop::StaticClass()) };
	TrailLoop->SetGhostTrail(Component, Interval, Length);
}

ATTGhostTrailLoop::ATTGhostTrailLoop()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATTGhostTrailLoop::SetGhostTrail(USkeletalMeshComponent* Component, float Interval, float Length)
{
	SkeletalMesh = Component;
	LoopInterval = Interval;
	LoopLength = Length;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ATTGhostTrailLoop::DoWork, Interval, true);
}

void ATTGhostTrailLoop::DoWork()
{
	if (LoopLength > 0.0f) {
		FActorSpawnParameters Param{};
		Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ATTGhostTrail* Trail{ GetWorld()->SpawnActor<ATTGhostTrail>
		(ATTGhostTrail::StaticClass(), SkeletalMesh->GetComponentTransform(), Param) };
		Trail->SetSkeletalMesh(SkeletalMesh);
		Trail->StartTrail();
	}
	else {
		GetWorldTimerManager().ClearTimer(TimerHandle);
		Destroy();
	}
	LoopLength -= LoopInterval;
}
