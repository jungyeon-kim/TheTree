#pragma once

#include "TheTree.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "TTGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FTTCharacterData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Gold;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float MaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float MaxSta;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float StaToGetPerHit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Atk;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Def;
public:
	FTTCharacterData() = default;
};

USTRUCT(BlueprintType)
struct FTTAIData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float MaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Atk;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Def;
public:
	FTTAIData() = default;
};

UCLASS()
class THETREE_API UTTGameInstance : public UGameInstance
{
	GENERATED_BODY()
private:
	UPROPERTY()
	class UDataTable* TTCharacterTable;
	UPROPERTY()
	class UDataTable* TTAITable;
	UPROPERTY()
	class UTTCharacterStatComponent* TTPlayerStat;
	UPROPERTY()
	TArray<class UTTUIMapButton*> UIMapButtonCluster;

	int32 ClearCount{};
	bool bIsClearTrooper{};
public:
	UTTGameInstance();

	virtual void Init() override;

	FTTCharacterData* GetTTCharacterData(FName ObjectName) const;
	FTTAIData* GetTTAIData(FName ObjectName) const;
	class UTTCharacterStatComponent* GetTTPlayerStat() const;

	int32 GetClearCount();
	void SetClearCount(int32 Count);

	void SetClearTrooper(bool bCleared);
	bool GetClearTrooper();	// ��ư ����ؼ� ����� ����~

	void SetButtonCluster(TArray<class UTTUIMapButton*>& ButtonCluster);
	TArray<class UTTUIMapButton*>& GetButtonCluster();
};
