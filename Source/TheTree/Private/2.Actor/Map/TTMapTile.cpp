#include "TTMapTile.h"

ATTMapTile::ATTMapTile()
{
	PrimaryActorTick.bCanEverTick = false;
	MapMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MAPMESH"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_MESH
	{ TEXT("/Game/Assets/StaticMesh/CastleTower/SM_Plains_CastleTower_01.SM_Plains_CastleTower_01") };

	if (SM_MESH.Succeeded())
		MapMesh->SetStaticMesh(SM_MESH.Object);
	RootComponent = MapMesh;
}

void ATTMapTile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATTMapTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

