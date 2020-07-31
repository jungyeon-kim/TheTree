#include "TTDurionMapTile.h"

ATTDurionMapTile::ATTDurionMapTile()
{
	PrimaryActorTick.bCanEverTick = false;
	MapMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DURIONMAPMESH"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_MESH
	{ TEXT("/Game/Assets/StaticMesh/CastleTower/SM_Plains_CastleTower_01.SM_Plains_CastleTower_01") };

	if (SM_MESH.Succeeded())
		MapMesh->SetStaticMesh(SM_MESH.Object);
	RootComponent = MapMesh;
	SetActorScale3D(FVector(0.5f, 0.5f, 0.5f));
	MapMesh->SetMobility(EComponentMobility::Static);
}

void ATTDurionMapTile::BeginPlay()
{
	Super::BeginPlay();
}

void ATTDurionMapTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

