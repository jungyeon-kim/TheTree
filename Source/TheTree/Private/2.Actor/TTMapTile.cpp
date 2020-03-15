// Fill out your copyright notice in the Description page of Project Settings.


#include "TTMapTile.h"

// Sets default values
ATTMapTile::ATTMapTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	MapMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MAPMESH"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_MESH
	(TEXT("/Game/Assets/StaticMesh/CastleTower/SM_Plains_CastleTower_01.SM_Plains_CastleTower_01"));

	if (SM_MESH.Succeeded())
		MapMesh->SetStaticMesh(SM_MESH.Object);
	RootComponent = MapMesh;
}

// Called when the game starts or when spawned
void ATTMapTile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATTMapTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

