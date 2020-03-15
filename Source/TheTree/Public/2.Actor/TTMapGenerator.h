// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TheTree.h"
#include "GameFramework/Actor.h"
#include "TTMapGenerator.generated.h"

UCLASS()
class THETREE_API ATTMapGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATTMapGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maker", Meta = (AllowPrivateAccess = true))
	int32 BirthLimits;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maker", Meta = (AllowPrivateAccess = true))
	int32 DeathLimits;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maker", Meta = (AllowPrivateAccess = true))
	int32 MapXSize { 30 };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maker", Meta = (AllowPrivateAccess = true))
	int32 MapYSize { 30 };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maker", Meta = (AllowPrivateAccess = true))
	float MapOffsetX{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maker", Meta = (AllowPrivateAccess = true))
	float MapOffsetY{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maker", Meta = (AllowPrivateAccess = true))
	float MapOffsetZ{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maker", Meta = (AllowPrivateAccess = true))
	int32 GenerateChance { 45 };

	TArray<bool> MakeMapTexture();
	void CelluarAutomata(TArray<bool>& Texture);
	int32 CountNeighbours(const TArray<bool>& Texture, int x, int y);
	int32 GetIndexFromXY(int x, int y);
	void FinalWork(TArray<bool>& Texture);
};
