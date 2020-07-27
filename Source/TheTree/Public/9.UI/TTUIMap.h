#pragma once

#include "TheTree.h"
#include "Blueprint/UserWidget.h"
#include "TTUIMap.generated.h"

UENUM()
enum class EButtonType
{
	MONSTER,
	SHELTER,
	TROOPER,
	DURION,
	SHOP,
	CURRENT
};

USTRUCT()
struct FDistElement
{
	GENERATED_USTRUCT_BODY()
	EButtonType Type;
	float Percentage;
};

UCLASS()
class THETREE_API UTTUIMap : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual bool Initialize() override;

	virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeDestruct() override;

private:

	UPROPERTY()
	TArray<class UTTUIMapButton*> ButtonCluster;

	UPROPERTY()
	TArray<class USlateWidgetStyleAsset*> WidgetCluster;

	UPROPERTY()
	class UScrollBox* ScrollBox;

	UPROPERTY()
	class UCanvasPanel* Panel;

	UPROPERTY()
	class USlider* Slider;

	UPROPERTY()
	UWorld* WorldContext;

	/*
	 * Generate UI Map
	 *
	 * @param StartX : X Coordinate On 2nd Dimension 
	 * @param StartY : Y Coordinate On 2nd Dimension => On the Viewport
	 */
	void GenerateMapRecursive(int Layer, int StartX, int StartY, int EndX, int EndY);
	void GenerateMapRecursiveImpl(int GenerateLayer, int StartX, int StartY);
	void CreateButton(const EButtonType& ButtonType, int PosX, int PosY);
	void RegistryEachMapChild(int Layer);

	UFUNCTION()
	void ChangeSliderValue(float Value);

	UPROPERTY()
	TArray<FDistElement> Dist;

	const int HalfButtonXSize{ 50 };
	const int StrideY{ 200 };
	const float OriginPostionStrideY{ 1500.0f };

public:
	void SetWorldContext(UWorld* World);
	void ClearAllWidget();
};

EButtonType ProbAlgorithm(const TArray<FDistElement>& Items);
