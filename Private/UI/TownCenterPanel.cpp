#include "UI/TownCenterPanel.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "DataStructures/Upgrades.h"
#include "GamePlay/GameStatics.h"
#include "GamePlay/RTSGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/UpgradeManager.h"

UTownCenterPanel::UTownCenterPanel(const FObjectInitializer& ObjectInitializer) : UBuildingsPanelWidget(
	ObjectInitializer)
{
}

void UTownCenterPanel::NativeConstruct()
{
	Super::NativeConstruct();
	WidgetTree->FindWidget<UButton>(TEXT("Ageup"))->SetIsEnabled(false);
	WidgetTree->FindWidget<UButton>(TEXT("Villager"))->OnClicked.AddDynamic(this, &UTownCenterPanel::CreateVillager);
	WidgetTree->FindWidget<UButton>(TEXT("Loom"))->OnClicked.AddDynamic(this, &UTownCenterPanel::GetLoom);
	WidgetTree->FindWidget<UButton>(TEXT("Queue_Loom"))->OnClicked.AddDynamic(this, &UTownCenterPanel::LoomCanceled);
	WidgetTree->FindWidget<UButton>(TEXT("Queue_Loom"))->SetVisibility(ESlateVisibility::Hidden);
	WidgetTree->FindWidget<UButton>(TEXT("AgeUp"))->OnClicked.AddDynamic(this, &UTownCenterPanel::AgeUp);
	ProgressBar = WidgetTree->FindWidget<UProgressBar>(TEXT("Progress_Bar"));
	ProgressBar->SetPercent(0);
	ProgressBar->SetVisibility(ESlateVisibility::Hidden);
}

void UTownCenterPanel::CreateVillager()
{
	UE_LOG(LogTemp, Warning, TEXT("Villager Created"));
}

void UTownCenterPanel::GetLoom()
{
	if (ARTSGameMode* GameMode = Cast<ARTSGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this, "LoomFinished");
		GameMode->GetUpgradeManager()->CallPrepare(1, TimerDelegate);
		WidgetTree->FindWidget<UButton>(TEXT("Queue_Loom"))->SetVisibility(ESlateVisibility::Visible);
		WidgetTree->FindWidget<UButton>(TEXT("Loom"))->SetVisibility(ESlateVisibility::Hidden);
		StartProgressBar(GameStatics::GetUpgradeByID(1)->TimeToComplete);
	}
}

void UTownCenterPanel::AgeUp()
{
	UE_LOG(LogTemp, Warning, TEXT("Age up"));
}

void UTownCenterPanel::LoomFinished()
{
	if (ARTSGameMode* GameMode = Cast<ARTSGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		GameMode->GetUpgradeManager()->Call(1);
		WidgetTree->FindWidget<UButton>(TEXT("Queue_Loom"))->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UTownCenterPanel::LoomCanceled()
{
	if (ARTSGameMode* GameMode = Cast<ARTSGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		GameMode->GetUpgradeManager()->Cancel(1);
		WidgetTree->FindWidget<UButton>(TEXT("Loom"))->SetVisibility(ESlateVisibility::Visible);
		WidgetTree->FindWidget<UButton>(TEXT("Queue_Loom"))->SetVisibility(ESlateVisibility::Hidden);
		ClearProgressBar();
	}
}

void UTownCenterPanel::ClearProgressBar()
{
	if (ProgressTimerHandle.IsValid())
	{
		GEngine->GetCurrentPlayWorld()->GetTimerManager().ClearTimer(ProgressTimerHandle);
	}
	ProgressBar->SetPercent(0);
	ProgressBar->SetVisibility(ESlateVisibility::Hidden);
	CurrentProgressBarSecond = 0;
}

void UTownCenterPanel::RunProgressBar(const uint32& Seconds)
{
	CurrentProgressBarSecond += 1;
	ProgressBar->SetPercent(CurrentProgressBarSecond/static_cast<float>(Seconds));
	if (ProgressBar->GetPercent() >= 1)
	{
		ClearProgressBar();
	}
}

void UTownCenterPanel::StartProgressBar(uint32 Seconds)
{
	ClearProgressBar();
	FTimerDelegate ProgressBarDelegate;
	ProgressBarDelegate.BindUFunction(this, "RunProgressBar", Seconds);
	GEngine->GetCurrentPlayWorld()->GetTimerManager().SetTimer(ProgressTimerHandle, ProgressBarDelegate,1, true);
	ProgressBar->SetVisibility(ESlateVisibility::Visible);
}

