// Fill out your copyright notice in the Description page of Project Settings.


#include "TTMapGenerator.h"
#include "TTMapTile.h"
#include "GameFramework/PlayerStart.h"

// Sets default values
ATTMapGenerator::ATTMapGenerator() : BirthLimits{ 5 }, DeathLimits{ 4 }
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ATTMapGenerator::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ATTMapGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATTMapGenerator::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	TArray<AActor*> PlayerStarts = {};
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);
	APlayerStart* StartActor = { Cast<APlayerStart>(PlayerStarts[0]) };
	if (!StartActor)
		return;
	StartActor->GetRootComponent()->SetMobility(EComponentMobility::Movable);

	TArray<bool> Map = { MakeMapTexture() };

	for (int i = 0; i < 7; ++i)
		CelluarAutomata(Map);

	FinalWork(Map);

	bool Once = { false };
	for (int y = 0; y < MapYSize; ++y)
	{
		for (int x = 0; x < MapXSize; ++x)
		{
			if (Map[GetIndexFromXY(x, y)])
			{
				GetWorld()->SpawnActor<ATTMapTile>(ATTMapTile::StaticClass(),
					FVector(MapOffsetX + (x * 600.0f), MapOffsetY + (y * 600.0f), MapOffsetZ),
					FRotator(0.0f, 0.0f, 0.0f));
			}
			else
				if (!Once)
				{
					TTLOG_S(Warning)
						StartActor->SetActorLocation(FVector(MapOffsetX + (x * 600.0f), MapOffsetY + (y * 600.0f), 88.0f));
					Once = true;
				}
		}
	}
}

TArray<bool> ATTMapGenerator::MakeMapTexture()
{
	FRandomStream RandomStream{ StaticCast<int32>(FDateTime::Now().GetTicks()) };
	TArray<bool> NewMap = {};
	for (int y = 0; y < MapYSize; ++y)
	{
		for (int x = 0; x < MapXSize; ++x)
		{
			int32 RandomSeed{ RandomStream.RandRange(1, 100) };
			bool MapLive{ false };
			if (RandomSeed <= GenerateChance)
				MapLive = true;
			NewMap.Add(MapLive);
		}
	}	//Generate
	return NewMap;
}

int32 ATTMapGenerator::CountNeighbours(const TArray<bool>& Texture, int x, int y)
{
	int32 Count = 0;
	for (int i = -1; i < 2; ++i) {
		for (int j = -1; j < 2; ++j) {
			int NeighbourX = x + j;
			int NeighbourY = y + i;
			// i == 0 && j == 0 은 현재 세포의 중앙점에 있다는 뜻, 그땐 넘겨야 함 
			if (i == 0 && j == 0)
				continue;

			// 지도 가장자리에서 index 보는 경우
			else if (NeighbourX < 0 || NeighbourY < 0 || NeighbourX >= MapXSize || NeighbourY >= MapYSize)
				Count = Count + 1;

			else if (Texture[GetIndexFromXY(NeighbourX, NeighbourY)])
				Count = Count + 1;
		}
	}
	return Count;
}

void ATTMapGenerator::CelluarAutomata(TArray<bool>& Texture)
{
	TArray<bool> NewMap = {};
	NewMap.SetNum(MapXSize * MapYSize);

	for (int y = 0; y < MapYSize; ++y)
	{
		for (int x = 0; x < MapXSize; ++x)
		{
			int32 Nbs = CountNeighbours(Texture, x, y);
			int32 MapOffsetXY = GetIndexFromXY(x, y);
			bool TempMapLive{ false };
			if (Texture[MapOffsetXY] && Nbs >= DeathLimits)
				TempMapLive = true;
			// 이웃의 수에 의해 태어나기가 적절한 지 판단
			else if (!Texture[MapOffsetXY] && Nbs > BirthLimits)
				TempMapLive = true;

			NewMap[MapOffsetXY] = TempMapLive;
		}
	}
	Texture = std::move(NewMap);
}

int32 ATTMapGenerator::GetIndexFromXY(int x, int y)
{
	return (y * MapXSize) + x;
}

void ATTMapGenerator::FinalWork(TArray<bool>& Texture)
{
	for (int y = 0; y < MapYSize; ++y)
	{
		for (int x = 0; x < MapXSize; ++x)
		{
			if (x == 0 || y == 0 || x == MapXSize - 1 || y == MapYSize - 1)
				Texture[GetIndexFromXY(x, y)] = true;
		}
	}
}