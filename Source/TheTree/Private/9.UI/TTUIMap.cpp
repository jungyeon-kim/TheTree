#include "TTUIMap.h"
#include "TTUIMapButton.h"
void UTTUIMap::NativeConstruct()
{
	Super::NativeConstruct();
	Panel = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("CanvasPanel_0")));

	WidgetCluster.Add(Cast<USlateWidgetStyleAsset>(StaticLoadObject(USlateWidgetStyleAsset::StaticClass(),
		nullptr, TEXT("/Game/Assets/UI/Slate/UI_MapMonsterButton.UI_MapMonsterButton"))));
	WidgetCluster.Add(Cast<USlateWidgetStyleAsset>(StaticLoadObject(USlateWidgetStyleAsset::StaticClass(),
		nullptr, TEXT("/Game/Assets/UI/Slate/UI_MapShelterButton.UI_MapShelterButton"))));
	WidgetCluster.Add(Cast<USlateWidgetStyleAsset>(StaticLoadObject(USlateWidgetStyleAsset::StaticClass(),
		nullptr, TEXT("/Game/Assets/UI/Slate/UI_MapTrooperButton.UI_MapTrooperButton"))));
	WidgetCluster.Add(Cast<USlateWidgetStyleAsset>(StaticLoadObject(USlateWidgetStyleAsset::StaticClass(),
		nullptr, TEXT("/Game/Assets/UI/Slate/UI_MapDurionButton.UI_MapDurionButton"))));

	GenerateMapRecursive(4, 960, 100, 960, 920);
}

bool UTTUIMap::Initialize()
{
	Super::Initialize();
	return true;
}

void UTTUIMap::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UTTUIMap::GenerateMapRecursive(int Layer, int StartX, int StartY, int EndX, int EndY)
{
	// Order : Current ( The first tile is must be cleared.
	CreateButton(EButtonType::MONSTER, StartX, StartY);
	// Layer2 ~ Before the trooper
	GenerateMapRecursiveImpl(Layer - 1, StartX - 30, StartY + 100);
	GenerateMapRecursiveImpl(Layer - 1, StartX + 30, StartY + 100);
	// Trooper

	// Trooper ~ Before the durion
	CreateButton(EButtonType::DURION, EndX, EndY);
}

void UTTUIMap::GenerateMapRecursiveImpl(int Layer, int CurX, int CurY)
{
	if (Layer <= 0) return;
	CreateButton(EButtonType::MONSTER, CurX, CurY);
	GenerateMapRecursiveImpl(Layer - 1, CurX - 30*Layer, CurY + 100);
	GenerateMapRecursiveImpl(Layer - 1, CurX + 30*Layer, CurY + 100);
}

void UTTUIMap::CreateButton(const EButtonType& ButtonType, int PosX, int PosY)
{
	UTTUIMapButton* NewButton{ NewObject<UTTUIMapButton>(this) };
	NewButton->SetRenderScale(FVector2D{ 1.0f, 2.0f });
	NewButton->SetRenderTranslation(
		FVector2D{ static_cast<float>(PosX), static_cast<float>(PosY) });
	NewButton->SetButtonType(WidgetCluster[static_cast<int>(ButtonType)]);
	ButtonCluster.Add(NewButton);
	Panel->AddChild(NewButton);
}
