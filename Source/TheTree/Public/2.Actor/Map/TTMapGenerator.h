#pragma once

#include "TheTree.h"
#include "GameFramework/Actor.h"
#include "TTMapGenerator.generated.h"

UCLASS()
class THETREE_API ATTMapGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	ATTMapGenerator();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maker", Meta = (AllowPrivateAccess = true))
	int32 BirthLimits;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maker", Meta = (AllowPrivateAccess = true))
	int32 DeathLimits;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maker", Meta = (AllowPrivateAccess = true))
	int32 MapXSize{ 30 };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maker", Meta = (AllowPrivateAccess = true))
	int32 MapYSize{ 30 };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maker", Meta = (AllowPrivateAccess = true))
	float MapOffsetX{ -4500.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maker", Meta = (AllowPrivateAccess = true))
	float MapOffsetY{ -4500.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maker", Meta = (AllowPrivateAccess = true))
	float MapOffsetZ{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maker", Meta = (AllowPrivateAccess = true))
	int32 GenerateChance{ 45 };

	TArray<bool> MapTexture{};
	TArray<UClass*> ClassCluster{};

	TArray<bool> MakeMapTexture();
	void CelluarAutomata(TArray<bool>& Texture);
	int32 CountNeighbours(const TArray<bool>& Texture, int x, int y);
	int32 CountNeighboursWithoutThis(const TArray<bool>& Texture, int x, int y,
		int Start = -1, int End = 2);
	int32 GetIndexFromXY(int x, int y);
	void FinalWork(TArray<bool>& Texture);
	void SetChandelier(const TArray<bool>& Texture, int x = 15, int y = 15);

	template <typename ... Args>
	constexpr void SetMonsters()
	{
		ClassCluster.Empty();
		SetMonstersImpl(Args::StaticClass()...);
		size_t size{ sizeof...(Args) };
		for (int y = MapYSize / 2; y < MapYSize && size != 0; ++y)
		{
			for (int x = MapXSize / 2; x < MapXSize && size != 0; ++x)
			{
				if (MapTexture[GetIndexFromXY(x, y)] || CountNeighboursWithoutThis(MapTexture, x, y) > 4)
					continue;
				InPlaceActor(ClassCluster.Last(), MapOffsetX +  (x * 300.0f), MapOffsetY + (y * 300.0f));
				ClassCluster.Pop();
				--size;
			}
		}
	}
	template <typename T, typename ... Args>
	constexpr void SetMonstersImpl(T Class, Args ... LeftClass)
	{
		ClassCluster.Emplace(Class);
		SetMonstersImpl(LeftClass...);
	}
	void SetMonstersImpl();
	void InPlaceActor(UClass* Class, float XPos, float YPos);
};
