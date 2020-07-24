#include "TTUIMap.h"
#include "TTUIMapButton.h"
void UTTUIMap::NativeConstruct()
{
	Super::NativeConstruct();
	Panel = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("TTUIMapPanel")));
	Slider = Cast<USlider>(GetWidgetFromName(TEXT("TTUIMapSlider")));

	WidgetCluster.Add(Cast<USlateWidgetStyleAsset>(StaticLoadObject(USlateWidgetStyleAsset::StaticClass(),
		nullptr, TEXT("/Game/Assets/UI/Slate/UI_MapMonsterButton.UI_MapMonsterButton"))));
	WidgetCluster.Add(Cast<USlateWidgetStyleAsset>(StaticLoadObject(USlateWidgetStyleAsset::StaticClass(),
		nullptr, TEXT("/Game/Assets/UI/Slate/UI_MapShelterButton.UI_MapShelterButton"))));
	WidgetCluster.Add(Cast<USlateWidgetStyleAsset>(StaticLoadObject(USlateWidgetStyleAsset::StaticClass(),
		nullptr, TEXT("/Game/Assets/UI/Slate/UI_MapTrooperButton.UI_MapTrooperButton"))));
	WidgetCluster.Add(Cast<USlateWidgetStyleAsset>(StaticLoadObject(USlateWidgetStyleAsset::StaticClass(),
		nullptr, TEXT("/Game/Assets/UI/Slate/UI_MapDurionButton.UI_MapDurionButton"))));

	// Bottom-Up
	GenerateMapRecursive(10, 900, 100, 900, 1000);
	Slider->OnValueChanged.AddDynamic(this, &UTTUIMap::ChangeSliderValue);
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

FReply UTTUIMap::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseWheel(InGeometry, InMouseEvent);
	Slider->SetValue(FMath::Clamp(Slider->GetValue() + (InMouseEvent.GetWheelDelta() * 0.05f), 0.0f, 1.0f));
	ChangeSliderValue(Slider->GetValue());
	return OnMouseWheel(InGeometry, InMouseEvent).NativeReply;
}

void UTTUIMap::GenerateMapRecursive(int Layer, int StartX, int StartY, int EndX, int EndY)
{
	MaxHeight = 1920 + (Layer * 200);
	// Order : Current ( The first tile must be cleared. )
	CreateButton(EButtonType::MONSTER, StartX, StartY);
	int DivideLayer{ Layer / 2 };
	GenerateMapRecursiveImpl(DivideLayer, StartX, StartY);
	// Trooper
	CreateButton(EButtonType::TROOPER, StartX, StartY + (DivideLayer * 100) + 100);
	// Trooper ~ Before the durion
	GenerateMapRecursiveImpl(DivideLayer, StartX, StartY + (DivideLayer * 100) + 100);
	// Durion
	//CreateButton(EButtonType::DURION, EndX, EndY);
}

void UTTUIMap::GenerateMapRecursiveImpl(int GenerateLayer, int StartX, int StartY)
{
	TArray<int> PrevElement{};
	const int StartLayer{ GenerateLayer };
	while (GenerateLayer > 1)
	{
		if (StartLayer == GenerateLayer)
		{
			int ElementCount{ static_cast<int>((pow(2, GenerateLayer - 1) + 0.5)) };
			int StrideX{ (1800 / ElementCount)  };
			int OffsetX{ (StartX / ElementCount) };
			for (int i = 0; i < ElementCount; ++i)
			{
				int PosX{ StrideX * i };
				CreateButton(EButtonType::MONSTER, PosX + OffsetX, StartY + ((GenerateLayer-1) * 100));
				PrevElement.Add(PosX + OffsetX);
			}
		}
		else
		{
			int PrevElementCount{ PrevElement.Num() };	//Caching
			TArray<int> LastElement{};
			for (int i = 0; i < PrevElementCount; i += 2)	
			{
				int PosX{ (PrevElement[i] + PrevElement[i + 1]) / 2 };
				CreateButton(EButtonType::MONSTER, PosX, StartY + ((GenerateLayer-1) * 100));
				LastElement.Add(PosX);
			}
			PrevElement = std::move(LastElement);
		}
		--GenerateLayer;
	}
}

void UTTUIMap::CreateButton(const EButtonType& ButtonType, int PosX, int PosY)
{
	UTTUIMapButton* NewButton{ NewObject<UTTUIMapButton>(this) };
	NewButton->SetRenderScale(FVector2D{ 1.0f, 2.0f });
	NewButton->SetRenderTranslation(
		FVector2D{ static_cast<float>(PosX), static_cast<float>(PosY) });
	NewButton->SetButtonType(WidgetCluster[static_cast<int>(ButtonType)]);
	NewButton->SetOriginPosition(FVector2D{ static_cast<float>(PosX), static_cast<float>(PosY) - 300.0f });
	ButtonCluster.Add(NewButton);

	Panel->AddChild(NewButton);
}

void UTTUIMap::ChangeSliderValue(float Value)
{
	// 1.0 => Top, 0.0 = Bottom
	for (auto& Button : ButtonCluster)
	{
		FVector2D OriginPosition{ Button->GetOriginPosition() };
		Button->SetRenderTranslation(FVector2D{ OriginPosition.X, OriginPosition.Y + (Value * 300.0f)});
		float YPos{ Button->RenderTransform.Translation.Y };
		if (YPos > 20.0f && YPos < 1040.0f)
			Button->SetVisibility(ESlateVisibility::Visible);
		else
			Button->SetVisibility(ESlateVisibility::Hidden);
	}
}
