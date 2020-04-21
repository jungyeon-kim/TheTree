#include "TTGhostTrail.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/PoseableMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

static TArray<TPair<FName, ATTGhostTrailLoop*>> TrailCluster; // extern Singleton 

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

void ATTGhostTrail::SetSkeletalMesh(class USkeletalMeshComponent* Target)
{
	TrailMesh->SetSkeletalMesh(Target->SkeletalMesh);
	TrailMesh->CopyPoseFromSkeletalComponent(Target);
	MatInstanceDynamic = UMaterialInstanceDynamic::Create(TrailMaterial, this);
	TArray<FSkeletalMaterial> Materials = Target->SkeletalMesh->Materials;

	for (int Index = 0; Index < Materials.Num(); ++Index) 
		TrailMesh->SetMaterial(Index, MatInstanceDynamic);
}

void ATTGhostTrail::SetMaterial(const TCHAR* Direction)
{
	if(!Direction)
		return;

	UMaterialInterface* Material{ Cast<UMaterialInterface>
		(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, Direction)) };
	TrailMaterial = Material;
}

void ATTGhostTrail::SetMaterial(UMaterialInterface* Material)
{
	if (Material == nullptr)
		return;
	TrailMaterial = Material;
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

void PlayGhostTrail(USkeletalMeshComponent* Component, const TCHAR* MaterialPath)
{
	FActorSpawnParameters Param{};
	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ATTGhostTrail* Trail{ Component->GetWorld()->SpawnActor<ATTGhostTrail>
		(ATTGhostTrail::StaticClass(), Component->GetComponentTransform(), Param) };
	Trail->SetMaterial(MaterialPath);
	Trail->SetSkeletalMesh(Component);
	Trail->StartTrail();
}

void PlayGhostTrail(USkeletalMeshComponent* Component, const TCHAR* MaterialPath, float Interval)
{
	ATTGhostTrailLoop* TrailLoop{ Component->GetWorld()->SpawnActor<ATTGhostTrailLoop>
		(ATTGhostTrailLoop::StaticClass()) };
	TrailLoop->SetMaterial(MaterialPath);
	TrailLoop->SetGhostTrail(Component, Interval);
	TrailCluster.Add(MakeTuple(Component->GetFName(), TrailLoop));
}

void PlayGhostTrail(USkeletalMeshComponent* Component, UMaterialInterface* Material, float Interval)
{
	ATTGhostTrailLoop* TrailLoop{ Component->GetWorld()->SpawnActor<ATTGhostTrailLoop>
		(ATTGhostTrailLoop::StaticClass()) };
	TrailLoop->SetMaterial(Material);
	TrailLoop->SetGhostTrail(Component, Interval);
	TrailCluster.Add(MakeTuple(Component->GetFName(), TrailLoop));
}

void PlayGhostTrail(USkeletalMeshComponent* Component, float Interval)
{
	ATTGhostTrailLoop* TrailLoop{ Component->GetWorld()->SpawnActor<ATTGhostTrailLoop>
		(ATTGhostTrailLoop::StaticClass()) };
	TrailLoop->SetGhostTrail(Component, Interval);
	TrailCluster.Emplace(Component->GetFName(), TrailLoop);
}

void StopGhostTrail(USkeletalMeshComponent* Component)
{
	for (auto& Elem : TrailCluster)
	{
		if (Elem.Key == Component->GetFName() && IsValid(Elem.Value))
		{
			Elem.Value->StopTrail();
			Elem.Value->Destroy();
		}
	}
	
	TrailCluster.RemoveAllSwap([&](TPair<FName, ATTGhostTrailLoop*>& Elem) {
		return (Elem.Key == Component->GetFName());
		});
}

ATTGhostTrailLoop::ATTGhostTrailLoop()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATTGhostTrailLoop::SetGhostTrail(USkeletalMeshComponent* Component, float Interval)
{
	SkeletalMesh = Component;
	LoopInterval = Interval;

	GetWorldTimerManager().SetTimer(TimerHandle, this, &ATTGhostTrailLoop::DoWork, Interval, true);
}

void ATTGhostTrailLoop::SetMaterial(const TCHAR* Direction)
{
	if (!Direction)
		return;

	UMaterialInterface* Material{ Cast<UMaterialInterface>
		(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, Direction)) };
	TrailMaterial = Material;
}
void ATTGhostTrailLoop::SetMaterial(UMaterialInterface* Material)
{
	TrailMaterial = Material;
}
void ATTGhostTrailLoop::DoWork()
{
		ATTGhostTrail* Trail{ GetWorld()->SpawnActorDeferred<ATTGhostTrail>
			(ATTGhostTrail::StaticClass(), SkeletalMesh->GetComponentTransform(),
				this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn) };
		UGameplayStatics::FinishSpawningActor(Trail, SkeletalMesh->GetComponentTransform());
		Trail->SetMaterial(TrailMaterial);
		Trail->SetSkeletalMesh(SkeletalMesh);
		Trail->StartTrail();
}

void ATTGhostTrailLoop::StopTrail()
{
	if (TimerHandle.IsValid())
		GetWorldTimerManager().ClearTimer(TimerHandle);
}
