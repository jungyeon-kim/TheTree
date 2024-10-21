#pragma once

#include "TheTree.h"
#include "GameFramework/Actor.h"
#include "TTCommonBattleLevel.h"
#include "TTAIController.h"
#include "Engine/DataTable.h"
#include "TTBaseMapGenerator.generated.h"

USTRUCT()
struct FActorDistElement
{
	GENERATED_USTRUCT_BODY()
	UClass* Type;
	float Percentage;
};

USTRUCT(BlueprintType)
struct FTTLevelDesign : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<class ATTEnemyBase>> Monsters;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int HowManySpawn;
};

UENUM()
enum class ETTTextureType
{
	OPENED,
	BLOCKED,
	ONUPPER,
	RESERVED
};

UCLASS(Abstract)
class THETREE_API ATTBaseMapGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	ATTBaseMapGenerator();
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maker", Meta = (AllowPrivateAccess = true))
	int32 BirthLimits;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maker", Meta = (AllowPrivateAccess = true))
	int32 DeathLimits;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maker", Meta = (AllowPrivateAccess = true))
	int32 MapXSize { 30 };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maker", Meta = (AllowPrivateAccess = true))
	int32 MapYSize { 30 };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maker", Meta = (AllowPrivateAccess = true))
	float MapOffsetX{ -4500.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maker", Meta = (AllowPrivateAccess = true))
	float MapOffsetY{ -4500.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maker", Meta = (AllowPrivateAccess = true))
	float MapOffsetZ{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maker", Meta = (AllowPrivateAccess = true))
	int32 GenerateChance { 45 };

	UPROPERTY()
	class UTTGameInstance* TTGameInstance;

	UPROPERTY()
	UClass* MeshClass;

	UPROPERTY()
	class APlayerStart* PlayerStart;

	TArray<ETTTextureType> MapTexture{};
	TArray<UClass*> ClassCluster{};

	TArray<ETTTextureType> MakeMapTexture(int GenerationCount);
	void CelluarAutomata(TArray<ETTTextureType>& Texture, int GenerationCount);
	int32 CountNeighbours(const TArray<ETTTextureType>& Texture, int x, int y);
	int32 CountNeighboursWithoutThis(const TArray<ETTTextureType>& Texture, int x, int y,
		int Start = -1, int End = 2);
	int32 GetIndexFromXY(int x, int y);
	void FinalWork(TArray<ETTTextureType>& Texture);
	void SetChandelier(TArray<ETTTextureType>& Texture, int x = 15, int y = 15, bool bSetChandelier = true);

	void SpawnMonsters(TArray<FActorDistElement>& DistElements, int NumOfMonster);
	void SpawnMonsters(UDataTable* MonsterDataTable, int Row);

	void InPlaceCharacterRandom(UClass* CharacterClass);
	void InPlaceActorRandom(UClass* ActorClass, int32 SpawnCount, float OffsetZ, ETTTextureType PossibleBlocking = ETTTextureType::BLOCKED);
	void InPlaceActorRandom(const TArray<FActorDistElement>& DistElements, int NumOfSpawning, float OffsetZ, ETTTextureType PossibleBlocking = ETTTextureType::BLOCKED);
	void SetMapTileActorClass(UClass* Class);
	void BuildObjects(TArray<ETTTextureType>& Texture, bool bSetTorch);
	void TurnToMonster();
	void RebuildNavigation();

private:
	UClass* ProbAlgorithm(const TArray<FActorDistElement>& Items);
};