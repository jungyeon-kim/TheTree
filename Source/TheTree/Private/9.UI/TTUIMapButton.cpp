#include "TTUIMapButton.h"

UTTUIMapButton::UTTUIMapButton()
{
	static ConstructorHelpers::FObjectFinder<USlateWidgetStyleAsset> MAPWIDGETSTYLE
	{ TEXT("/Game/Assets/UI/Slate/UI_MapMonsterButton.UI_MapMonsterButton") };

	SButton::FArguments Default{};
	Default.ButtonStyle(MAPWIDGETSTYLE.Object);
	WidgetStyle = *Default._ButtonStyle;
}

void UTTUIMapButton::SetButtonType()
{

}