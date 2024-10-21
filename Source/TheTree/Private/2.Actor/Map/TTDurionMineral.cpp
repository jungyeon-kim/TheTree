#include "TTDurionMineral.h"

ATTDurionMineral::ATTDurionMineral()
{
	PrimaryActorTick.bCanEverTick = false;

	MineralMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DURIONMAPMESH1"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_MESH
	{ TEXT("/Game/Assets/StaticMesh/Durion/Mineral/SM_Durion_Mineral_03.SM_Durion_Mineral_03") };

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

void ATTDurionMineral::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATTDurionMineral::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

