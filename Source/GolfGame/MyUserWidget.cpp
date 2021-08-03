// Fill out your copyright notice in the Description page of Project Settings.


#include "MyUserWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"

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
	FString fstr = TEXT("남은거리 : ") + FString::Printf(TEXT("%.1fm"), dis);
	DistanceRemaining->SetText(FText::FromString(fstr));
}

void UMyUserWidget::UpdatePar()
{
	int32 inum = CurrentPlayerState->GetDoublePar();
	FString fstr = TEXT("Par ") + FString::Printf(TEXT("%d"), inum);
	ShowPar->SetText(FText::FromString(fstr));
}

void UMyUserWidget::UpdateScore()
{
	int32 inum = CurrentPlayerState->GetNowHoleScore();
	FString fstr = FString::Printf(TEXT("%d"), inum);
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
	IconLocation.X = (FlagLocation.Y + 1500) / 15000 * 500;
	IconLocation.Y = 500 - (FlagLocation.X / 15000 * 500);

	FlagIcon->SetRenderTranslation(IconLocation);
}

void UMyUserWidget::UpdateBallIcon()
{
	FVector BallLocation = CurrentBallState->GetActorLocation();

	FVector2D IconLocation;
	IconLocation.X = (BallLocation.Y + 1500) / 15000 * 500;
	IconLocation.Y = 500 - (BallLocation.X / 15000 * 500);

	BallIcon->SetRenderTranslation(IconLocation);
}

// 좌표변환 부분에서
// 1. 카메라 좌표
// 2. width
// 따로 저장해서 해당 변수들을 불러와 계산하는 식으로
