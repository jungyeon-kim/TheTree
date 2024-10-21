#include "TTBaseMapGenerator.h"
#include "TTTorch.h"
#include "TTChandelier.h"
#include "TTEnemyBase.h"
#include "TTPortal.h"
#include "TTGameInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerStart.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "TTPlayer.h"

ATTBaseMapGenerator::ATTBaseMapGenerator() : BirthLimits{ 5 }, DeathLimits{ 4 }
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATTBaseMapGenerator::BeginPlay()
{
	Super::BeginPlay();
}

void ATTBaseMapGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATTBaseMapGenerator::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	TTGameInstance = Cast<UTTGameInstance>(GetGameInstance());
}

TArray<ETTTextureType> ATTBaseMapGenerator::MakeMapTexture(int GenerationCount)
{
	FRandomStream RandomStream{ StaticCast<int32>(FDateTime::Now().GetTicks()) };
	TArray<ETTTextureType> NewMap{};
	for (int y = 0; y < MapYSize; ++y)
	{
		for (int x = 0; x < MapXSize; ++x)
		{
			int32 RandomSeed{ RandomStream.RandRange(1, 100) };
			ETTTextureType Type{ ETTTextureType::OPENED };
			if (RandomSeed <= GenerateChance)
				Type = ETTTextureType::BLOCKED;
			NewMap.Add(Type);
		}
	}	//Generate

	CelluarAutomata(NewMap, GenerationCount);
	FinalWork(NewMap);
	return NewMap;
}

int32 ATTBaseMapGenerator::CountNeighboursWithoutThis(const TArray<ETTTextureType>& Texture, int x, int y, int Start, int End)
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

			else if (Texture[GetIndexFromXY(NeighbourX, NeighbourY)] == ETTTextureType::BLOCKED)
				++Count;
		}
	}
	return Count;
}

int32 ATTBaseMapGenerator::CountNeighbours(const TArray<ETTTextureType>& Texture, int x, int y)
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

			else if (Texture[GetIndexFromXY(NeighbourX, NeighbourY)] == ETTTextureType::BLOCKED)
				Count = Count + 1;
		}
	}
	return Count;
}

void ATTBaseMapGenerator::CelluarAutomata(TArray<ETTTextureType>& Texture, int GenerationCount)
{
	TArray<ETTTextureType> NewMap{};
	
	while (GenerationCount--)
	{
		NewMap.SetNum(MapXSize * MapYSize);
		for (int y = 0; y < MapYSize; ++y)
		{
			for (int x = 0; x < MapXSize; ++x)
			{
				int32 Nbs{ CountNeighbours(Texture, x, y) };
				int32 MapOffsetXY{ GetIndexFromXY(x, y) };
				ETTTextureType Type{ ETTTextureType::OPENED };

				if (Texture[MapOffsetXY] == ETTTextureType::BLOCKED && Nbs >= DeathLimits)
					Type = ETTTextureType::BLOCKED;
				// 이웃의 수에 의해 태어나기가 적절한 지 판단
				else if (Texture[MapOffsetXY] == ETTTextureType::OPENED && Nbs > BirthLimits)
					Type = ETTTextureType::BLOCKED;

				NewMap[MapOffsetXY] = Type;
			}
		}
		Texture = std::move(NewMap);
	}
}

int32 ATTBaseMapGenerator::GetIndexFromXY(int x, int y)
{
	return (y * MapXSize) + x;
}

void ATTBaseMapGenerator::FinalWork(TArray<ETTTextureType>& Texture)
{
	for (int y = 0; y < MapYSize; ++y)
	{
		for (int x = 0; x < MapXSize; ++x)
		{
			if (x == 0 || y == 0 || x == MapXSize - 1 || y == MapYSize - 1)
				Texture[GetIndexFromXY(x, y)] = ETTTextureType::BLOCKED;
		}
	}
}

void ATTBaseMapGenerator::SetChandelier(TArray<ETTTextureType>& Texture, int x, int y, bool bSetChandelier)
{
	FActorSpawnParameters Param;
	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for (int i = y; i < MapYSize; ++i)
	{
		for (int j = x; j < MapXSize; ++j)
		{
			if (Texture[GetIndexFromXY(j, i)] == ETTTextureType::BLOCKED)
				continue;

			if (!CountNeighboursWithoutThis(Texture, j, i, -3, 4))
			{
				if(bSetChandelier)
				GetWorld()->SpawnActor<ATTChandelier>(ATTChandelier::StaticClass(),
					FVector(MapOffsetX + (j * 300.0f), MapOffsetY + (i * 300.0f), MapOffsetZ + 1020.0f), FRotator{ 0.0f, 0.0f, 0.0f }, Param);
				
				ATTPortal* Portal{ GetWorld()->SpawnActor<ATTPortal>(ATTPortal::StaticClass(),
					FVector{ MapOffsetX + (j * 300.0f), MapOffsetY + (i * 300.0f), MapOffsetZ + 240.0f }, FRotator{ 0.0f, 0.0f, 0.0f }) };
				Portal->SetActorHiddenInGame(true);
				Portal->SetActorEnableCollision(false);
				Texture[GetIndexFromXY(j, i)] = ETTTextureType::RESERVED;
				return;
			}
		}
	}

	for (int i = y - 1; i < 0; --i)
	{
		for (int j = x - 1; j < 0; --j)
		{
			if (Texture[GetIndexFromXY(j, i)] == ETTTextureType::BLOCKED)
				continue;

			if (!CountNeighboursWithoutThis(Texture, j, i, -3, 4))
			{
				if (bSetChandelier)
				GetWorld()->SpawnActor<ATTChandelier>(ATTChandelier::StaticClass(),
					FVector(MapOffsetX + (j * 300.0f), MapOffsetY + (i * 300.0f), MapOffsetZ + 1020.0f), FRotator{ 0.0f, 0.0f, 0.0f }, Param);

				ATTPortal* Portal{ GetWorld()->SpawnActor<ATTPortal>(ATTPortal::StaticClass(),
				FVector{ MapOffsetX + (j * 300.0f), MapOffsetY + (i * 300.0f), MapOffsetZ + 240.0f }, FRotator{ 0.0f, 0.0f, 0.0f }) };
				Portal->SetActorHiddenInGame(true);
				Portal->SetActorEnableCollision(false);
				Texture[GetIndexFromXY(j, i)] = ETTTextureType::RESERVED;

				return;
			}
		}
	}
}

void ATTBaseMapGenerator::SpawnMonsters(TArray<FActorDistElement>& DistElements, int NumOfMonster)
{
	for (int i = 0 ; i < NumOfMonster; ++i)
		InPlaceCharacterRandom(ProbAlgorithm(DistElements));

	ATTCommonBattleLevel* Level{ Cast<ATTCommonBattleLevel>(GetWorld()->GetLevelScriptActor()) };

	if (Level)
		Level->SetMonsterCount(NumOfMonster);

	TurnToMonster();
}

void ATTBaseMapGenerator::SpawnMonsters(UDataTable* MonsterDataTable, int Row)
{
	// Monsters in DataTable must be spawned at least once.
	TArray<FTTLevelDesign*> Elements{};
	MonsterDataTable->GetAllRows<FTTLevelDesign>(TEXT(""), Elements);

	const int Index{ Row };
	const int SpawnCount{ Elements[Index]->HowManySpawn };
	const int MonsterClassCount{ Elements[Index]->Monsters.Num() };

	for (int i = 0; i < SpawnCount; ++i)
		InPlaceCharacterRandom(Elements[Index]->Monsters[i % MonsterClassCount]);

	ATTCommonBattleLevel* Level { Cast<ATTCommonBattleLevel>(GetWorld()->GetLevelScriptActor())};

	if (Level)
		Level->SetMonsterCount(SpawnCount);

	TurnToMonster();
}

void ATTBaseMapGenerator::InPlaceCharacterRandom(UClass* CharacterClass)
{
	FActorSpawnParameters Param;
	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	static FRandomStream RandomStream{};
	RandomStream.GenerateNewSeed();

	ACharacter* Char{ GetWorld()->SpawnActor<ACharacter>(CharacterClass, FVector{ 0.0f, 0.0f, 200.0f },
	FRotator{ 0.0f, 0.0f, 0.0f }, Param) };
	UNavigationPath* Path{};
	do
	{
		int32 RandX{ RandomStream.RandRange(0, MapXSize - 1)};
		int32 RandY{ RandomStream.RandRange(0, MapYSize - 1)};

		if (MapTexture[GetIndexFromXY(RandX, RandY)] == ETTTextureType::BLOCKED 
			|| CountNeighboursWithoutThis(MapTexture, RandX, RandY, -2, 3) > 1)
			continue;

		Char->SetActorLocation(FVector{ MapOffsetX + (RandX * 300.0f), MapOffsetY + (RandY * 300.0f), 200.0f });
		Path = UNavigationSystemV1::FindPathToActorSynchronously(GetWorld(), Char->GetActorLocation(), PlayerStart);
	}
	while (!Path || Path->IsPartial());
}

void ATTBaseMapGenerator::InPlaceActorRandom(UClass* ActorClass, int32 SpawnCount, float OffsetZ, ETTTextureType PossibleBlocking)
{
	FActorSpawnParameters Param;
	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	static FRandomStream RandomStream{};
	RandomStream.GenerateNewSeed();

	for (int i = 0; i < SpawnCount; ++i)
	{	
		int32 RandX{};
		int32 RandY{};
		int32 Index{};

		do
		{
			RandX = RandomStream.RandRange(0, MapXSize - 1);
			RandY = RandomStream.RandRange(0, MapYSize - 1);
			Index = GetIndexFromXY(RandX, RandY);
		} while (MapTexture[Index] == ETTTextureType::BLOCKED || MapTexture[Index] == ETTTextureType::RESERVED);
		GetWorld()->SpawnActor<AActor>(ActorClass, 
			FVector{ MapOffsetX + (RandX * 300.0f), MapOffsetY + (RandY * 300.0f), OffsetZ }, 
			FRotator::ZeroRotator, Param);
		MapTexture[Index] = PossibleBlocking;
	}
}

void ATTBaseMapGenerator::InPlaceActorRandom(const TArray<FActorDistElement>& DistElements, int NumOfSpawning, float OffsetZ, ETTTextureType PossibleBlocking)
{
	for (int i = 0; i < NumOfSpawning; ++i)
		InPlaceActorRandom(ProbAlgorithm(DistElements), 1, OffsetZ, PossibleBlocking);
}


void ATTBaseMapGenerator::BuildObjects(TArray<ETTTextureType>& Texture, bool bSetTorch)
{
	static FRandomStream RandomStream{};
	RandomStream.GenerateNewSeed();

	TArray<AActor*> PlayerStarts{};
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);
	PlayerStart = Cast<APlayerStart>(PlayerStarts[0]);

	
	if (!PlayerStart)
		return;

	PlayerStart->SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	PlayerStart->GetRootComponent()->SetMobility(EComponentMobility::Movable);

	bool bOnce{ false };
	int32 TorchCount{};
	FActorSpawnParameters Param;
	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	for (int y = 0; y < MapYSize; ++y)
	{
		for (int x = 0; x < MapXSize; ++x)
		{
			if (Texture[GetIndexFromXY(x, y)] == ETTTextureType::BLOCKED)
			{
				if (CountNeighboursWithoutThis(Texture, x, y) < 8)
					GetWorld()->SpawnActor<AActor>(MeshClass,
						FVector(MapOffsetX + (x * 300.0f), MapOffsetY + (y * 300.0f), MapOffsetZ), 
						FRotator{ 0.0f, RandomStream.FRandRange(0.0f, 360.0f), 0.0f}, Param);
			}
			else
			{
				if (!bOnce && CountNeighboursWithoutThis(Texture, x, y, -4, 5) < 1)
				{
					PlayerStart->SetActorLocation(FVector(MapOffsetX + (x * 300.0f), MapOffsetY + (y * 300.0f), 198.0f));
					bOnce ^= true;
				}
				if (bSetTorch)
				{
					if (TorchCount > 5 && CountNeighboursWithoutThis(Texture, x, y) > 3)
					{
						GetWorld()->SpawnActor<ATTTorch>(ATTTorch::StaticClass(),
							FVector(MapOffsetX + (x * 300.0f), MapOffsetY + (y * 300.0f), MapOffsetZ), FRotator{ 0.0f, 0.0f, 0.0f });
						TorchCount = 0;
					}
				}
			}
			++TorchCount;
		}
	}
	SetChandelier(Texture, MapXSize / 2, MapYSize / 2, bSetTorch);

	MapTexture = std::move(Texture);

	RebuildNavigation();
}

void ATTBaseMapGenerator::SetMapTileActorClass(UClass* Class)
{
	MeshClass = Class;
}

void ATTBaseMapGenerator::TurnToMonster()
{
	TArray<AActor*> Monsters{}, Player{};
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATTEnemyBase::StaticClass(), Monsters);
	if (Monsters.Num() && Player.Num())
	{
		FRotator Rot{ UKismetMathLibrary::FindLookAtRotation(Player[0]->GetActorLocation(), Monsters[0]->GetActorLocation()) };
		PlayerStart->SetActorRotation(Rot);
	}
}

void ATTBaseMapGenerator::RebuildNavigation()
{
	UNavigationSystemV1* NavSystem{ UNavigationSystemV1::GetCurrent(GetWorld()) };
	if (NavSystem)
	{
		NavSystem->SetGeometryGatheringMode(ENavDataGatheringModeConfig::Instant);
		NavSystem->Tick(UGameplayStatics::GetTimeSeconds(GetWorld()));
		for (int32 i = 0; i < NavSystem->NavDataSet.Num(); ++i)
			NavSystem->NavDataSet[i]->EnsureBuildCompletion();
	}
}

UClass* ATTBaseMapGenerator::ProbAlgorithm(const TArray<FActorDistElement>& Items)
{
	static FRandomStream RandomStream{};
	RandomStream.GenerateNewSeed();
	float RandomSeed = RandomStream.FRandRange(1.0f, 100.0f);
	float Cumulative{};

	for (auto& Item : Items)
	{
		Cumulative += Item.Percentage;
		if (RandomSeed <= Cumulative)
			return Item.Type;
	}
	return Items.Last().Type;
}