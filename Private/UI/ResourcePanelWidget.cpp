#include "UI/ResourcePanelWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/TextBlock.h"

UResourcePanelWidget::UResourcePanelWidget(const FObjectInitializer& ObjectInitializer): UUserWidget(ObjectInitializer)
{
}

void UResourcePanelWidget::UpdateResourcesEvent(const FResources Resources)
{
	Food->SetText(FText::FromString(FString::FromInt(Resources.Food)));
	Wood->SetText(FText::FromString(FString::FromInt(Resources.Wood)));
	Gold->SetText(FText::FromString(FString::FromInt(Resources.Stone)));
	Stone->SetText(FText::FromString(FString::FromInt(Resources.Gold)));
	Population->SetText(FText::FromString(FString::FromInt(Resources.Population)));
}

void UResourcePanelWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Food = WidgetTree->FindWidget<UTextBlock>(TEXT("FoodCount"));
	Wood = WidgetTree->FindWidget<UTextBlock>(TEXT("WoodCount"));
	Gold = WidgetTree->FindWidget<UTextBlock>(TEXT("GoldCount"));
	Stone = WidgetTree->FindWidget<UTextBlock>(TEXT("StoneCount"));
	Population = WidgetTree->FindWidget<UTextBlock>(TEXT("PopulationCount"));
}
