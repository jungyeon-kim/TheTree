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
	float MaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float MaxSta;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Atk;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Def;
public:
	FTTCharacterData() = default;
};

UCLASS()
class THETREE_API UTTGameInstance : public UGameInstance
{
	GENERATED_BODY()
private:
	UPROPERTY()
	class UDataTable* TTCharacterTable;

	int32 ClearCount{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LevelManage", Meta = (AllowPrivateAccess = true))
	int32 PlanOfClearCount{ 3 };
public:
	UTTGameInstance();

	virtual void Init() override;

	FTTCharacterData* GetTTCharacterData(FName ObjectName);

	int32 GetClearCount();
	void AddClearCount();
	const int32 GetPlanOfClearCount();
};
