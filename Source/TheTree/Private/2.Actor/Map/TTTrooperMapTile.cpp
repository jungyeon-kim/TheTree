#include "TTTrooperMapTile.h"

ATTTrooperMapTile::ATTTrooperMapTile()
{
	PrimaryActorTick.bCanEverTick = false;
	MapMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TROOPERMAPMESH"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_MESH
	{ TEXT("/Game/Assets/StaticMesh/CastleTower/SM_Plains_CastleTower_02.SM_Plains_CastleTower_02") };

	if (SM_MESH.Succeeded())
		MapMesh->SetStaticMesh(SM_MESH.Object);
	RootComponent = MapMesh;
	SetActorScale3D(FVector(2.7f, 2.7f, 3.0f));
	MapMesh->SetMobility(EComponentMobility::Static);
}

void ATTTrooperMapTile::BeginPlay()
{
	Super::BeginPlay();
}

void ATTTrooperMapTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


