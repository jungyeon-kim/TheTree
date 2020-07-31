#pragma once

#include "TheTree.h"
#include "GameFramework/Actor.h"
#include "TTCommonBattleLevel.h"
#include "TTBaseMapGenerator.generated.h"

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

	TArray<bool> MapTexture{};
	TArray<UClass*> ClassCluster{};

	TArray<bool> MakeMapTexture();
	void CelluarAutomata(TArray<bool>& Texture, int GenerationCount);
	int32 CountNeighbours(const TArray<bool>& Texture, int x, int y);
	int32 CountNeighboursWithoutThis(const TArray<bool>& Texture, int x, int y,
		int Start = -1, int End = 2);
	int32 GetIndexFromXY(int x, int y);
	void FinalWork(TArray<bool>& Texture);
	void SetChandelier(const TArray<bool>& Texture, int x = 15, int y = 15, bool bSetChandelier = true);

	template <typename ... Args>
	constexpr void SpawnMonsters(int NumOfMonster)
	{
		ClassCluster.Empty();
		SpawnMonstersImpl(Args::StaticClass()...);
		size_t size{ static_cast<size_t>(NumOfMonster) };

		for (int y = MapYSize / 2; y < MapYSize && size != 0; ++y)
		{
			for (int x = MapXSize / 2; x < MapXSize && size != 0; ++x)
			{
				if (MapTexture[GetIndexFromXY(x, y)] || CountNeighboursWithoutThis(MapTexture, x, y) > 2)
					continue;

				UClass* Class{ GetRandomMonsterClass(ClassCluster) };
				InPlaceActor(Class, MapOffsetX + (x * 300.0f), MapOffsetY + (y * 300.0f));
				--size;
			}
		}

		ATTCommonBattleLevel* Level{ Cast<ATTCommonBattleLevel>(GetWorld()->GetLevelScriptActor()) };
		if (Level)
			Level->SetMonsterCount(NumOfMonster);
	}
	template <typename T, typename ... Args>
	FORCEINLINE constexpr void SpawnMonstersImpl(T Class, Args ... LeftClass)
	{
		ClassCluster.Emplace(Class);
		SpawnMonstersImpl(LeftClass...);
	}

	void SpawnMonstersImpl();
	void InPlaceActor(UClass* Class, float XPos, float YPos);
	UClass* GetRandomMonsterClass(const TArray<UClass*>& MonsterCluster);
	void SetMapTileActorClass(UClass* Class);
	void BuildObjects(TArray<bool>& Texture, bool bSetTorch);
	void TurnToMonster();
};
