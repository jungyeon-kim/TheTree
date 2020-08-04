#include "TTDurionMineral.h"

ATTDurionMineral::ATTDurionMineral()
{
	PrimaryActorTick.bCanEverTick = false;

	MineralMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DURIONMAPMESH"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_MESH
	{ TEXT("/Game/Assets/StaticMesh/Durion/Mineral/SM_Durion_Mineral_03.SM_Durion_Mineral_03") };

	if (SM_MESH.Succeeded())
		MineralMesh->SetStaticMesh(SM_MESH.Object);
	RootComponent = MineralMesh;
	SetActorScale3D(FVector(0.5f, 0.5f, 0.5f));
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

