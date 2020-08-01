#pragma once

#include "TheTree.h"
#include "GameFramework/Actor.h"
#include "TTCommonBattleLevel.h"
#include "TTAIController.h"
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

	UPROPERTY()
	class APlayerStart* PlayerStart;

	TArray<bool> MapTexture{};
	TArray<UClass*> ClassCluster{};

	TArray<bool> MakeMapTexture(int GenerationCount);
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

		while (NumOfMonster--)
			InPlaceActorRandom(GetRandomMonsterClass(ClassCluster));
		
		ATTCommonBattleLevel* Level{ Cast<ATTCommonBattleLevel>(GetWorld()->GetLevelScriptActor()) };

		if (Level)
			Level->SetMonsterCount(NumOfMonster);
		TurnToMonster();
	}
	template <typename T, typename ... Args>
	FORCEINLINE constexpr void SpawnMonstersImpl(T Class, Args ... LeftClass)
	{
		ClassCluster.Emplace(Class);
		SpawnMonstersImpl(LeftClass...);
	}

	void SpawnMonstersImpl();
	void InPlaceActorRandom(UClass* MonsterClass);
	UClass* GetRandomMonsterClass(const TArray<UClass*>& MonsterCluster);
	void SetMapTileActorClass(UClass* Class);
	void BuildObjects(TArray<bool>& Texture, bool bSetTorch);
	void TurnToMonster();
	void RebuildNavigation();
};
