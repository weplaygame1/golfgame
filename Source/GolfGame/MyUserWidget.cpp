// Fill out your copyright notice in the Description page of Project Settings.


#include "MyUserWidget.h"

void UMyUserWidget::SetCurrentBallState(ABall* ball) 
{
	CurrentBallState = ball; 
	ball->GetPowerGaugeOnWidget.AddUObject(this, &UMyUserWidget::UpdatePower);
	ball->SetPowerZeroOnWidget.AddUObject(this,&UMyUserWidget::SetPowerZero);
}

void UMyUserWidget::SetCurrentPlayerState(AMyPlayerState* state)
{
	CurrentPlayerState = state; 
	state->GetDistanceOnWidget.AddUObject(this, &UMyUserWidget::UpdateDistance);
	state->GetParOnWidget.AddUObject(this, &UMyUserWidget::UpdatePar);
	state->GetScoreOnWidget.AddUObject(this, &UMyUserWidget::UpdateScore);
	state->GetWholeDistanceOnWidget.AddUObject(this,&UMyUserWidget::UpdateWholeDistance);
	state->GetMinimapOnWidget.AddUObject(this, &UMyUserWidget::SetMinimapImage);


	SetMinimapImage();
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
	int index = CurrentPlayerState->GetCurrentHoleIndex();
	FString Path = FString::Printf(TEXT("/Game/Blueprints/Widget/MinimapImage/MnimapIndex0.MnimapIndex%d"), index);
	UTexture2D* Texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *Path));
	Minimap->SetBrushFromTexture(Texture);
}

