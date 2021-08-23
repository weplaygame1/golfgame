// Fill out your copyright notice in the Description page of Project Settings.

#include "MyUserWidget.h"

#include "Components/CanvasPanel.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"

#include "Blueprint/WidgetBlueprintLibrary.h"

#include "GolfGameGameModeBase.h"
#include "MyPlayerState.h"
#include "Ball.h"

void UMyUserWidget::SetCurrentGameMode(AGolfGameGameModeBase* mode)
{
	CurrentGameMode = mode;
}

void UMyUserWidget::SetCurrentBallState(ABall* ball) 
{
	CurrentBallState = ball; 
	ball->GetPowerGaugeOnWidget.AddUObject(this, &UMyUserWidget::UpdatePower);
	ball->SetPowerZeroOnWidget.AddUObject(this, &UMyUserWidget::SetPowerZero);
	ball->UpdateBallIconOnWidget.AddUObject(this,&UMyUserWidget::UpdateBallIcon);
	ball->UpdatePredictIconOnWidget.AddUObject(this, &UMyUserWidget::UpdatePredictIcon);
	ball->UpdateClubStateOnWidget.AddUObject(this, &UMyUserWidget::UpdateClubState);
	ball->UpdateMovingInfoOnWidget.AddUObject(this, &UMyUserWidget::UpdateMovingInformation);
	ball->UpdateGeoStateOnWidget.AddUObject(this, &UMyUserWidget::UpdateGeoState);
	ball->UpdateShotNumberthOnWidget.AddUObject(this, &UMyUserWidget::UpdateShotNumberth);
	ball->UpdateScoreResultOnWidget.AddUObject(this, &UMyUserWidget::UpdateScoreResult);

	ball->OnOffMainPanelOnWidget.AddUObject(this, &UMyUserWidget::OnOffMainPanel);
	ball->OnOffMovingPanelOnWidget.AddUObject(this, &UMyUserWidget::OnOffMovingPanel);
	ball->OnOffOBResultOnWidget.AddUObject(this, &UMyUserWidget::OnOffOBResult);
	ball->OnOffConcedeResultOnWidget.AddUObject(this, &UMyUserWidget::OnOffConcedeResult);
	ball->OnOffOnScoreResultOnWidget.AddUObject(this, &UMyUserWidget::OnOffScoreResult);
}

void UMyUserWidget::SetCurrentPlayerState(AMyPlayerState* state)
{
	CurrentPlayerState = state; 
	state->GetDistanceOnWidget.AddUObject(this, &UMyUserWidget::UpdateDistance);
	state->GetParOnWidget.AddUObject(this, &UMyUserWidget::UpdatePar);
	state->GetScoreOnWidget.AddUObject(this, &UMyUserWidget::UpdateScore);
	state->GetWholeDistanceOnWidget.AddUObject(this,&UMyUserWidget::UpdateWholeDistance);
	state->GetMinimapOnWidget.AddUObject(this, &UMyUserWidget::SetMinimapImage);

}

void UMyUserWidget::UpdatePower()
{
	Power->SetPercent(CurrentBallState->GetPower());
}

void UMyUserWidget::SetPowerZero()
{
	Power->SetPercent(0);
}

void UMyUserWidget::UpdateWholeDistance()
{
	int32 dis = CurrentPlayerState->GetWholeDistance();
	FString fstr = FString::Printf(TEXT("%dm"), dis);
	WholeDistance->SetText(FText::FromString(fstr));
}

void UMyUserWidget::UpdateDistance()
{
	float dis = CurrentPlayerState->GetDistanceRemaining();
	FString fstr = FString::Printf(TEXT("%.1fm"), dis);
	DistanceRemaining->SetText(FText::FromString(fstr));
}

void UMyUserWidget::UpdatePar()
{
	int32 inum = CurrentGameMode->GetScoreTable(CurrentPlayerState->GetCurrentHoleIndex());
	FString fstr = TEXT("Par ") + FString::Printf(TEXT("%d"), inum);
	ShowPar->SetText(FText::FromString(fstr));
}

void UMyUserWidget::UpdateScore()
{
	int32 inum = CurrentPlayerState->GetAllScore();
	FString fstr;
	if (inum > 0)
	{
		fstr = FString::Printf(TEXT("+%d"), inum);
	}
	else
	{
		fstr = FString::Printf(TEXT("%d"), inum);
	}
	NowScore->SetText(FText::FromString(fstr));
}

void UMyUserWidget::SetMinimapImage()
{
	// 미니맵 이미지 세팅
	int index = CurrentPlayerState->GetCurrentHoleIndex();
	FString Path = FString::Printf(TEXT("/Game/Blueprints/Widget/MinimapImage/MnimapIndex%d.MnimapIndex%d"), index, index);
	UTexture2D* Texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *Path));
	Minimap->SetBrushFromTexture(Texture);

	// 플래그 아이콘 세팅
	int index1 = CurrentGameMode->GetFlagIconIndex(index);
	FString Path1 = FString::Printf(TEXT("/Game/Blueprints/Widget/MinimapImage/FlagIndex%d.FlagIndex%d"), index1, index1);
	UTexture2D* Texture1 = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *Path1));
	FlagIcon->SetBrushFromTexture(Texture1);
	
	FVector FlagLocation = CurrentGameMode->GetHoleCupLocation(index);

	/* 좌표 변환 */
	// 카메라 중앙 좌표
	FVector Center = CurrentGameMode->GetMinimapCenterLocation(index);
	// 미니맵 너비
	int32 Width = CurrentGameMode->GetMinimapWidth(index);

	FVector2D IconLocation;
	IconLocation.X = (FlagLocation.Y - (Center.Y - (Width / 2))) / Width * 500;
	IconLocation.Y = 500 - ((FlagLocation.X - (Center.X - (Width / 2))) / Width * 500);

	FlagIcon->SetRenderTranslation(IconLocation);
}

void UMyUserWidget::UpdateBallIcon()
{
	FVector BallLocation = CurrentBallState->GetActorLocation();

	/* 좌표 변환 */
	int index = CurrentPlayerState->GetCurrentHoleIndex();
	// 카메라 중앙 좌표
	FVector Center = CurrentGameMode->GetMinimapCenterLocation(index);
	// 미니맵 너비
	int32 Width = CurrentGameMode->GetMinimapWidth(index);

	FVector2D IconLocation;
	IconLocation.X = (BallLocation.Y - (Center.Y - (Width / 2))) / Width * 500;
	IconLocation.Y = 500 - ((BallLocation.X - (Center.X - (Width / 2))) / Width * 500);

	BallIcon->SetRenderTranslation(IconLocation);
}

void UMyUserWidget::UpdatePredictIcon()
{
	FVector PredictLocation = CurrentBallState->GetPredictLocation();

	/* 좌표 변환 */
	int index = CurrentPlayerState->GetCurrentHoleIndex();
	// 카메라 중앙 좌표
	FVector Center = CurrentGameMode->GetMinimapCenterLocation(index);
	// 미니맵 너비
	int32 Width = CurrentGameMode->GetMinimapWidth(index);

	FVector2D IconLocation;
	IconLocation.X = (PredictLocation.Y - (Center.Y - (Width / 2))) / Width * 500;
	IconLocation.Y = 500 - ((PredictLocation.X - (Center.X - (Width / 2))) / Width * 500);

	PredictIcon->SetRenderTranslation(IconLocation);
}

void UMyUserWidget::UpdateClubState()
{
	FText txt = (FText)StaticEnum<EGolfClub>()->GetDisplayNameTextByIndex((int32)CurrentBallState->GetClubState());
	ClubState->SetText(txt);
}

void UMyUserWidget::UpdateMovingInformation()
{
	float Moving_Dis = CurrentBallState->GetMovingDis();
	FString fstr0 = FString::Printf(TEXT("%.2fm"), Moving_Dis);
	MovingDis->SetText(FText::FromString(fstr0));

	float Remaining_dis = CurrentPlayerState->GetDistanceRemaining();
	FString fstr1 = FString::Printf(TEXT("%.2fm"), Remaining_dis);
	RemainingDis->SetText(FText::FromString(fstr1));
}

void UMyUserWidget::UpdateGeoState()
{
	FText txt = StaticEnum<EGeographyState>()->GetDisplayNameTextByIndex((int32)CurrentBallState->GetGeographyState());
	GeoState_0->SetText(txt);
	GeoState_1->SetText(txt);
}

void UMyUserWidget::UpdateShotNumberth()
{
	int32 inum = CurrentPlayerState->GetNumberth() + 1;
	
	FString fstr = FString::Printf(TEXT("%d"), inum);

	if (inum%10 == 1)
	{
		fstr += TEXT("st");
	}
	else if (inum%10 == 2)
	{
		fstr += TEXT("nd");
	}
	else if (inum%10 == 3)
	{
		fstr += TEXT("rd");
	}
	else
	{
		fstr += TEXT("th");
	}
	fstr+= TEXT(" Shot");

	ShotNumberth->SetText(FText::FromString(fstr));
}

void UMyUserWidget::UpdateScoreResult()
{
	int32 Numberth = CurrentPlayerState->GetNumberth();
	int32 ParNum = CurrentGameMode->GetScoreTable(CurrentPlayerState->GetCurrentHoleIndex());
	int32 inum = Numberth - ParNum;

	FString fstr;

	if (Numberth == 1)
	{
		fstr = TEXT("HOLE IN ONE");
	}
	else if (inum == ParNum)
	{
		fstr = TEXT("DOUBLE PAR");
	}
	else
	{
		switch (inum)
		{
		case -3:
			fstr = TEXT("ALBATROSS");
			break;
		case -2:
			fstr = TEXT("EAGLE");
			break;
		case -1:
			fstr = TEXT("BIRDIE");
			break;
		case 0:
			fstr = TEXT("PAR");
			break;
		case 1:
			fstr = TEXT("BOGEY");
			break;
		case 2:
			fstr = TEXT("DOUBLE BOGEY");
			break;
		case 3:
			fstr = TEXT("TRIPLE BOGEY");
			break;
		case 4:
			fstr = TEXT("QUARDRUPLE BOGEY");
			break;
		default:
			break;
		}
	}
	Score_Result->SetText(FText::FromString(fstr));
}

void UMyUserWidget::OnOffMainPanel(bool on)
{
	if (on)
	{
		MainPanel->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		MainPanel->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UMyUserWidget::OnOffMovingPanel(bool on)
{
	if (on)
	{
		MovingPanel->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		MovingPanel->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UMyUserWidget::OnOffOBResult(bool on)
{
	if (on)
	{
		OB_Result->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		OB_Result->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UMyUserWidget::OnOffConcedeResult(bool on)
{
	if (on)
	{
		Concede_Result->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Concede_Result->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UMyUserWidget::OnOffScoreResult(bool on)
{
	if (on)
	{
		Score_Result->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Score_Result->SetVisibility(ESlateVisibility::Hidden);
	}
}