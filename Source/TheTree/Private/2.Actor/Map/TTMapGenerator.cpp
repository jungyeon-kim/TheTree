#include "TTMapGenerator.h"
#include "TTMapTile.h"
#include "TTTorch.h"
#include "TTChandelier.h"
#include "GameFramework/PlayerStart.h"
#include "TTArcdevaArcher.h"
#include "TTArcdevaLancer.h"
#include "TTArcdevaWarrior.h"
#include "TTTrooper.h"
#include "TTPortal.h"

ATTMapGenerator::ATTMapGenerator() : BirthLimits{ 5 }, DeathLimits{ 4 }
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATTMapGenerator::BeginPlay()
{
	Super::BeginPlay();
	SetMonsters<ATTArcdevaArcher, ATTArcdevaWarrior, ATTArcdevaLancer>();
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

	for (int i = 0; i < 20; ++i)
		CelluarAutomata(Map);

	FinalWork(Map);

	bool bOnce{ false };
	int32 TorchCount{};
	FActorSpawnParameters Param;
	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	for (int y = 0; y < MapYSize; ++y)
	{
		for (int x = 0; x < MapXSize; ++x)
		{
			if (Map[GetIndexFromXY(x, y)])
			{
				if(CountNeighboursWithoutThis(Map, x, y) < 8)
				GetWorld()->SpawnActor<ATTMapTile>(ATTMapTile::StaticClass(),
					FVector(MapOffsetX + (x * 300.0f), MapOffsetY + (y * 300.0f), MapOffsetZ), FRotator{0.0f, 0.0f, 0.0f},Param);
			}
			else 
			{
				if (!bOnce && CountNeighboursWithoutThis(Map, x, y, -2, 3) < 1)
				{
					StartActor->SetActorLocation(FVector(MapOffsetX + (x * 300.0f), MapOffsetY + (y * 300.0f), 198.0f));
					bOnce ^= true;
				}
				if (TorchCount > 5 && CountNeighboursWithoutThis(Map, x, y) > 3)
				{
					GetWorld()->SpawnActor<ATTTorch>(ATTTorch::StaticClass(),
						FVector(MapOffsetX + (x * 300.0f), MapOffsetY + (y * 300.0f), MapOffsetZ), FRotator{0.0f, 0.0f, 0.0f});
					TorchCount = 0;
				}
			}
			++TorchCount;
		}
	}
	SetChandelier(Map, MapXSize / 2, MapYSize / 2);

	MapTexture = std::move(Map);
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

int32 ATTMapGenerator::CountNeighboursWithoutThis(const TArray<bool>& Texture, int x, int y, int Start, int End)
{
	int32 Count{};
	for (int i = Start; i < End; ++i) {
		for (int j = Start; j < End; ++j) {
			int NeighbourX{ x + j };
			int NeighbourY{ y + i };
			if (i == 0 && j == 0)
				continue;

			else if ((NeighbourX < 0 || NeighbourY < 0 || NeighbourX >= MapXSize || NeighbourY >= MapYSize))
				++Count;

			else if (Texture[GetIndexFromXY(NeighbourX, NeighbourY)])
				++Count;
		}
	}
	return Count;
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

void ATTMapGenerator::SetChandelier(const TArray<bool>& Texture, int x, int y)
{
	FActorSpawnParameters Param;
	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for (int i = y; i < MapYSize; ++i)
	{
		for (int j = x; j < MapXSize; ++j)
		{
			if (Texture[GetIndexFromXY(j, i)])
				continue;

			if (!CountNeighboursWithoutThis(Texture, j, i, -1, 2))
			{
				GetWorld()->SpawnActor<ATTChandelier>(ATTChandelier::StaticClass(),
					FVector(MapOffsetX + (x * 300.0f), MapOffsetY + (y * 300.0f), MapOffsetZ + 1020.0f), FRotator{ 0.0f, 0.0f, 0.0f }, Param);
				ATTPortal* Portal{ GetWorld()->SpawnActor<ATTPortal>(ATTPortal::StaticClass(),
					FVector{ MapOffsetX + (x * 300.0f), MapOffsetY + (y * 300.0f), MapOffsetZ + 240.0f }, FRotator{ 0.0f, 0.0f, 0.0f }) };
				Portal->SetActorHiddenInGame(true);
				Portal->SetActorEnableCollision(false);
				
				return;
			}
		}
	}

	for (int i = y-1; i < 0; --i)
	{
		for (int j = x - 1; j < 0; --j)
		{
			if (Texture[GetIndexFromXY(j, i)])
				continue;

			if (!CountNeighboursWithoutThis(Texture, j, i, -1, 2))
			{
				GetWorld()->SpawnActor<ATTMapTile>(ATTMapTile::StaticClass(),
					FVector(MapOffsetX + (x * 300.0f), MapOffsetY + (y * 300.0f), MapOffsetZ + 1020.0f), FRotator{ 0.0f, 0.0f, 0.0f }, Param);
				return;
			}
		}
	}
}

void ATTMapGenerator::SetMonstersImpl()
{
	// End
}

void ATTMapGenerator::InPlaceActor(UClass* Class, float XPos, float YPos)
{
	FActorSpawnParameters Param;
	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	TTLOG(Warning, TEXT("%s"), *Class->GetName());
	GetWorld()->SpawnActor<ACharacter>(Class, FVector{ XPos, YPos, 200.0f },
		FRotator{ 0.0f, 0.0f, 0.0f }, Param);
}