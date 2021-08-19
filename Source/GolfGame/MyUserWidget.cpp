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
	FString Path = FString::Printf(TEXT("/Game/Blueprints/Widget/MinimapImage/MnimapIndex0.MnimapIndex%d"), index);
	UTexture2D* Texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *Path));
	Minimap->SetBrushFromTexture(Texture);

	// 홀컵 이미지 세팅
	FVector FlagLocation = CurrentGameMode->GetHoleCupLocation(index);

	FVector2D IconLocation;
	IconLocation.X = (FlagLocation.Y + 6500) / 25000 * 500;

	IconLocation.Y = 500 - ((FlagLocation.X + 1500) / 25000 * 500);

	FlagIcon->SetRenderTranslation(IconLocation);

	// 플래그 아이콘 조건추가
	// 좌표변환 식으로
}

void UMyUserWidget::UpdateBallIcon()
{
	FVector BallLocation = CurrentBallState->GetActorLocation();

	FVector2D IconLocation;
	IconLocation.X = (BallLocation.Y + 6500) / 25000 * 500;

	IconLocation.Y = 500 - ((BallLocation.X + 1500) / 25000 * 500);

	BallIcon->SetRenderTranslation(IconLocation);
	
	// 좌표변환 부분에서
	// 1. 카메라 좌표
	// 2. width
	// 따로 저장해서 해당 변수들을 불러와 계산하는 식으로
}

void UMyUserWidget::UpdatePredictIcon()
{
	FVector PredictLocation = CurrentBallState->GetPredictLocation();

	FVector2D IconLocation;
	IconLocation.X = (PredictLocation.Y + 6500) / 25000 * 500;

	IconLocation.Y = 500 - ((PredictLocation.X + 1500) / 25000 * 500);

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
		fstr = TEXT("Hole in One");
	}
	else if (inum == ParNum)
	{
		fstr = TEXT("Double Par");
	}
	else
	{
		switch (inum)
		{
		case -3:
			fstr = TEXT("Albatross");
			break;
		case -2:
			fstr = TEXT("Eagle");
			break;
		case -1:
			fstr = TEXT("Birdie");
			break;
		case 0:
			fstr = TEXT("Par");
			break;
		case 1:
			fstr = TEXT("Bogey");
			break;
		case 2:
			fstr = TEXT("Double Bogey");
			break;
		case 3:
			fstr = TEXT("Triple Bogey");
			break;
		case 4:
			fstr = TEXT("Quardruple Bogey");
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