#include "TTMapGenerator.h"
#include "TTMapTile.h"
#include "TTTorch.h"
#include "GameFramework/PlayerStart.h"

ATTMapGenerator::ATTMapGenerator() : BirthLimits{ 5 }, DeathLimits{ 4 }
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATTMapGenerator::BeginPlay()
{
	Super::BeginPlay();

}

void ATTMapGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATTMapGenerator::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	TArray<AActor*> PlayerStarts{};
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);
	APlayerStart* StartActor{ Cast<APlayerStart>(PlayerStarts[0]) };

	if (!StartActor)
		return;

	StartActor->SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	StartActor->GetRootComponent()->SetMobility(EComponentMobility::Movable);
	
	TArray<bool> Map{ MakeMapTexture() };

	for (int i = 0; i < 15; ++i)
		CelluarAutomata(Map);

	FinalWork(Map);

	bool bOnce{ false };
	int32 TorchCount{};

	for (int y = 0; y < MapYSize; ++y)
	{
		for (int x = 0; x < MapXSize; ++x)
		{
			if (Map[GetIndexFromXY(x, y)])
			{
				GetWorld()->SpawnActor<ATTMapTile>(ATTMapTile::StaticClass(),
					FVector(MapOffsetX + (x * 300.0f), MapOffsetY + (y * 300.0f), MapOffsetZ), FRotator{});
			}
			else 
			{
				if (!bOnce && CountNeighbours(Map, x, y) < 1)
				{
					StartActor->SetActorLocation(FVector(MapOffsetX + (x * 300.0f), MapOffsetY + (y * 300.0f), 88.0f));
					bOnce ^= true;
				}
				if (TorchCount > 5 && CountNeighbours(Map, x, y) > 3)
				{
					GetWorld()->SpawnActor<ATTTorch>(ATTTorch::StaticClass(),
						FVector(MapOffsetX + (x * 300.0f), MapOffsetY + (y * 300.0f), MapOffsetZ), FRotator{});
					TorchCount = 0;
				}
			}
			++TorchCount;
		}
	}
}

TArray<bool> ATTMapGenerator::MakeMapTexture()
{
	FRandomStream RandomStream{ StaticCast<int32>(FDateTime::Now().GetTicks()) };
	TArray<bool> NewMap{};
	for (int y = 0; y < MapYSize; ++y)
	{
		for (int x = 0; x < MapXSize; ++x)
		{
			int32 RandomSeed{ RandomStream.RandRange(1, 100) };
			bool bMapLive{ false };
			if (RandomSeed <= GenerateChance)
				bMapLive = true;
			NewMap.Add(bMapLive);
		}
	}	//Generate
	return NewMap;
}

int32 ATTMapGenerator::CountNeighbours(const TArray<bool>& Texture, int x, int y)
{
	int32 Count{};
	for (int i = -1; i < 2; ++i) {
		for (int j = -1; j < 2; ++j) {
			int NeighbourX{ x + j };
			int NeighbourY{ y + i };
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
	TArray<bool> NewMap{};
	NewMap.SetNum(MapXSize * MapYSize);

	for (int y = 0; y < MapYSize; ++y)
	{
		for (int x = 0; x < MapXSize; ++x)
		{
			int32 Nbs{ CountNeighbours(Texture, x, y) };
			int32 MapOffsetXY{ GetIndexFromXY(x, y) };
			bool bTempMapLive{ false };
			if (Texture[MapOffsetXY] && Nbs >= DeathLimits)
				bTempMapLive = true;
			// 이웃의 수에 의해 태어나기가 적절한 지 판단
			else if (!Texture[MapOffsetXY] && Nbs > BirthLimits)
				bTempMapLive = true;

			NewMap[MapOffsetXY] = bTempMapLive;
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