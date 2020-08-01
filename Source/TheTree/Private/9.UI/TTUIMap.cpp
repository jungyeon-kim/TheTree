#include "TTUIMap.h"
#include "TTUIMonsterButton.h"
#include "TTUIShelterButton.h"
#include "TTUITrooperButton.h"
#include "TTUIImperfectDurionButton.h"
#include "TTUIStoreButton.h"
#include "TTGameInstance.h"

void UTTUIMap::NativeConstruct()
{
	Super::NativeConstruct();
	Panel = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("TTUIMapPanel")));
	Slider = Cast<USlider>(GetWidgetFromName(TEXT("TTUIMapSlider")));

	Slider->OnValueChanged.AddDynamic(this, &UTTUIMap::ChangeSliderValue);
	ButtonCluster = Cast<UTTGameInstance>(GetGameInstance())->GetButtonCluster();

	if (ButtonCluster.Num())
	{
		//Replace Buttons
		for (auto& Button : ButtonCluster)
			Panel->AddChild(Button);
	}
	else
	{
		WidgetCluster.Add(Cast<USlateWidgetStyleAsset>(StaticLoadObject(USlateWidgetStyleAsset::StaticClass(),
			nullptr, TEXT("/Game/Assets/UI/Slate/UI_MapMonsterButton.UI_MapMonsterButton"))));
		WidgetCluster.Add(Cast<USlateWidgetStyleAsset>(StaticLoadObject(USlateWidgetStyleAsset::StaticClass(),
			nullptr, TEXT("/Game/Assets/UI/Slate/UI_MapShelterButton.UI_MapShelterButton"))));
		WidgetCluster.Add(Cast<USlateWidgetStyleAsset>(StaticLoadObject(USlateWidgetStyleAsset::StaticClass(),
			nullptr, TEXT("/Game/Assets/UI/Slate/UI_MapTrooperButton.UI_MapTrooperButton"))));
		WidgetCluster.Add(Cast<USlateWidgetStyleAsset>(StaticLoadObject(USlateWidgetStyleAsset::StaticClass(),
			nullptr, TEXT("/Game/Assets/UI/Slate/UI_MapDurionButton.UI_MapDurionButton"))));
		WidgetCluster.Add(Cast<USlateWidgetStyleAsset>(StaticLoadObject(USlateWidgetStyleAsset::StaticClass(),
			nullptr, TEXT("/Game/Assets/UI/Slate/UI_MapStoreButton.UI_MapStoreButton"))));

		Dist = TArray<FButtonDistElement>{ {EButtonType::STORE, 15.0f}, {EButtonType::SHELTER, 15.0f} ,{EButtonType::MONSTER, 70.0f} };
		Dist.Sort([](const FButtonDistElement& lhs, const FButtonDistElement& rhs) {return lhs.Percentage < rhs.Percentage; });

		GenerateMapRecursive(10, 960, 100, 960, 2100);
		Dist.Empty();
		
		ButtonCluster.Sort([](const UTTUIMapButton& lhs, const UTTUIMapButton& rhs)
			{
				if (static_cast<int>(lhs.GetOriginPosition().Y) == static_cast<int>(rhs.GetOriginPosition().Y))
					return lhs.GetOriginPosition().X < rhs.GetOriginPosition().X;
				else
					return lhs.GetOriginPosition().Y < rhs.GetOriginPosition().Y;
			});
		ButtonCluster[0]->SetIsEnabled(true);
		ButtonCluster[0]->RegistChild(ButtonCluster[1]);
		ButtonCluster[0]->RegistChild(ButtonCluster[2]);
		RegistryEachMapChild(10);
	}
	for (auto& Button : ButtonCluster)
		Button->SetWorldContext(WorldContext);
}

bool UTTUIMap::Initialize()
{
	Super::Initialize();
	return true;
}

void UTTUIMap::NativeDestruct()
{
	ChangeSliderValue(1.0f);

	UTTGameInstance* GameInstance{ Cast<UTTGameInstance>(GetGameInstance()) };
	if (GameInstance && !GameInstance->GetButtonCluster().Num())
		Cast<UTTGameInstance>(GetGameInstance())->SetButtonCluster(ButtonCluster);
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
	// Order : Monster ~ Trooper ~ Monster ~ Durion
	CreateButton(EButtonType::MONSTER, StartX - HalfButtonXSize, StartY);
	int DivideLayer{ Layer / 2 };
	GenerateMapRecursiveImpl(DivideLayer, StartX, StartY);
	
	// Trooper
	CreateButton(EButtonType::TROOPER, StartX - HalfButtonXSize, StartY + (DivideLayer * StrideY));
	// Trooper ~ Before the durion
	GenerateMapRecursiveImpl(DivideLayer, StartX, StartY + (DivideLayer * StrideY));
	
	// Durion
	CreateButton(EButtonType::DURION, EndX - HalfButtonXSize, EndY);
}

void UTTUIMap::GenerateMapRecursiveImpl(int GenerateLayer, int StartX, int StartY)
{
	TArray<int> PrevElement{};
	const int StartLayer{ GenerateLayer };
	while (GenerateLayer > 1)
	{
		if (StartLayer == GenerateLayer)
		{
			const int ElementCount{ static_cast<int>((pow(2, GenerateLayer - 1) + 0.5)) };
			const int StrideX{ ((StartX * 2) / ElementCount)  };
			const int HalfElementCount{ ElementCount / 2 };
			for (int i = 0; i < ElementCount; ++i)
			{
				int PosX{ StrideX * i };
				CreateButton(ProbAlgorithm(Dist), PosX + HalfElementCount, StartY + ((GenerateLayer - 1) * StrideY));
				PrevElement.Add(PosX + HalfElementCount);
			}
		}
		else
		{
			int PrevElementCount{ PrevElement.Num() };	//Caching
			TArray<int> LastElement{};
			for (int i = 0; i < PrevElementCount; i += 2)	
			{
				int PosX{ (PrevElement[i] + PrevElement[i + 1]) / 2 };
				CreateButton(ProbAlgorithm(Dist), PosX, StartY + ((GenerateLayer-1) * StrideY));
				LastElement.Add(PosX);
			}
			PrevElement = std::move(LastElement);
		}
		--GenerateLayer;
	}
}

void UTTUIMap::CreateButton(const EButtonType& ButtonType, int PosX, int PosY)
{
	UTTUIMapButton* NewButton{};
	switch (ButtonType)
	{
	case EButtonType::MONSTER:
		NewButton = NewObject<UTTUIMonsterButton>(this);
		break;
	case EButtonType::SHELTER:
		NewButton = NewObject<UTTUIShelterButton>(this);
		break;
	case EButtonType::TROOPER:
		NewButton = NewObject<UTTUITrooperButton>(this);
		break;
	case EButtonType::DURION:
		NewButton = NewObject<UTTUIImperfectDurionButton>(this);
		break;
	case EButtonType::STORE:
		NewButton = NewObject<UTTUIStoreButton>(this);
		break;
	default:
		NewButton = NewObject<UTTUIMapButton>(this);
		break;
	}

	NewButton->SetRenderScale(FVector2D{ 1.0f, 3.0f });
	NewButton->SetRenderTranslation(
		FVector2D{ static_cast<float>(PosX), static_cast<float>(PosY) });
	NewButton->SetButtonType(WidgetCluster[static_cast<int>(ButtonType)]);
	NewButton->SetOriginPosition(FVector2D{ static_cast<float>(PosX), static_cast<float>(PosY) - OriginPostionStrideY });
	NewButton->SetWorldContext(WorldContext);
	ButtonCluster.Add(NewButton);

	Panel->AddChild(NewButton);
}

void UTTUIMap::ChangeSliderValue(float Value)
{
	// 1.0 => Top, 0.0 = Bottom
	for (auto& Button : ButtonCluster)
	{
		FVector2D OriginPosition{ Button->GetOriginPosition() };
		Button->SetRenderTranslation(FVector2D{ OriginPosition.X, OriginPosition.Y + (Value * OriginPostionStrideY)});
		float YPos{ Button->RenderTransform.Translation.Y };
		if (YPos > 20.0f && YPos < 1040.0f)
			Button->SetVisibility(ESlateVisibility::Visible);
		else
			Button->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UTTUIMap::RegistryEachMapChild(int Layer)
{
	const int LastRowElementCount{ static_cast<int>((pow(2, (Layer / 2) - 1) + 0.5)) };
	const int ElementCount{ ButtonCluster.Num() - 1 };
	const int HalfElementCount{ ButtonCluster.Num() / 2 };

	for (int Start = 0; Start < 2; ++Start) 
	{
		const int Offset{ (HalfElementCount ) * Start };
		for (int i = 0; i < HalfElementCount - LastRowElementCount ; ++i)
		{
			const int LeftChild{ 2 * i + 1 + Offset};
			const int RightChild{ LeftChild + 1};

			ButtonCluster[i + Offset]->RegistChild(ButtonCluster[LeftChild]);
			ButtonCluster[i + Offset]->RegistChild(ButtonCluster[RightChild]);
			ButtonCluster[LeftChild]->SetParent(ButtonCluster[i + Offset]);
			ButtonCluster[RightChild]->SetParent(ButtonCluster[i + Offset]);
		}
	}

	// At last row elements must have Trooper or Durion
	// ButtonCluster[ElementCount] -> Durion
	// ButtonCluster[HalfElementCount] -> Trooper
	for (int i = 0; i < LastRowElementCount; ++i)
	{
		const int FirstRow{ HalfElementCount - 1 - i };
		const int SecondRow{ ElementCount - 1 - i };
		ButtonCluster[FirstRow]->RegistChild(ButtonCluster[HalfElementCount]);
		ButtonCluster[SecondRow]->RegistChild(ButtonCluster[ElementCount]);
	}
}

void UTTUIMap::SetWorldContext(UWorld* World)
{
	WorldContext = World;
}

void UTTUIMap::ClearAllWidget()
{
	ButtonCluster.Empty();
	UTTGameInstance* GameInstance{ Cast<UTTGameInstance>(GetGameInstance()) };
	if (GameInstance)
		Cast<UTTGameInstance>(GetGameInstance())->SetButtonCluster(ButtonCluster);
}

EButtonType ProbAlgorithm(const TArray<FButtonDistElement>& Items)
{
	FRandomStream RandomStream{};
	RandomStream.GenerateNewSeed();
	float RandomSeed{ RandomStream.FRandRange(1.0f, 100.0f) };
	float Cumulative{};

	for (auto& Item : Items)
	{
		Cumulative += Item.Percentage;
		if (RandomSeed <= Cumulative)
			return Item.Type;
	}
	return EButtonType::MONSTER;
}
