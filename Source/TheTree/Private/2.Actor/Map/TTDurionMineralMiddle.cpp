#include "TTDurionMineralMiddle.h"

ATTDurionMineralMiddle::ATTDurionMineralMiddle()
{
	PrimaryActorTick.bCanEverTick = true;

	MineralMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DURIONMAPMESH3"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_MESH
	{ TEXT("/Game/Assets/StaticMesh/Durion/Mineral/SM_Durion_Mineral_05.SM_Durion_Mineral_05") };

	if (SM_MESH.Succeeded())
		MineralMesh->SetStaticMesh(SM_MESH.Object);
	RootComponent = MineralMesh;

	static FRandomStream RandomStream{};
	RandomStream.GenerateNewSeed();
	float Scale = RandomStream.FRandRange(0.1f, 0.5f);
	float Rotate = RandomStream.FRandRange(0.0f, 360.0f);

	SetActorScale3D(FVector(Scale, Scale, Scale));
	SetActorRotation(FRotator{ 0.0f, Rotate, 0.0f });
	MineralMesh->SetMobility(EComponentMobility::Static);
}

void ATTDurionMineralMiddle::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATTDurionMineralMiddle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

