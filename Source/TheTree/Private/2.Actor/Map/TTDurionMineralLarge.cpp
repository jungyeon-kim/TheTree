#include "TTDurionMineralLarge.h"

ATTDurionMineralLarge::ATTDurionMineralLarge()
{
	PrimaryActorTick.bCanEverTick = false;

	MineralMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DURIONMAPMESH2"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_MESH
	{ TEXT("/Game/Assets/StaticMesh/Durion/Mineral/SM_Durion_Mineral_04.SM_Durion_Mineral_04") };

	if (SM_MESH.Succeeded())
		MineralMesh->SetStaticMesh(SM_MESH.Object);
	RootComponent = MineralMesh;

	static FRandomStream RandomStream{};
	RandomStream.GenerateNewSeed();
	float Scale = RandomStream.FRandRange(0.5f, 1.5f);
	float Rotate = RandomStream.FRandRange(0.0f, 360.0f);

	SetActorScale3D(FVector(Scale, Scale, Scale));
	SetActorRotation(FRotator{ 0.0f, Rotate, 0.0f });
	MineralMesh->SetMobility(EComponentMobility::Static);
}

void ATTDurionMineralLarge::BeginPlay()
{
	Super::BeginPlay();
}

void ATTDurionMineralLarge::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

