// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GolfGame.h"

#include "Blueprint/UserWidget.h"
#include "MyUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class GOLFGAME_API UMyUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetCurrentBallState(class ABall* ball);
	void SetCurrentPlayerState(class AMyPlayerState* state);
	void SetCurrentGameMode(class AGolfGameGameModeBase* mode);

	void UpdatePower();
	void SetPowerZero();

	void UpdateWholeDistance();

	void UpdateDistance();

	void UpdatePar();

	void UpdateScore();

	void SetMinimapImage();

	void UpdateBallIcon();
public:
	TWeakObjectPtr<class ABall> CurrentBallState;
	TWeakObjectPtr<class AMyPlayerState> CurrentPlayerState;
	TWeakObjectPtr<class AGolfGameGameModeBase> CurrentGameMode;

	// 게이지바
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* Power;

	// 텍스트로 보여주는 정보들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* WholeDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* DistanceRemaining;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* ShowPar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* NowScore;

	// 미니맵
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* Minimap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* BallIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* FlagIcon;
};
