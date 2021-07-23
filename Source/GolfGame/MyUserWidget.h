// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "GolfGame.h"

#include "MyPlayerState.h"
#include "Ball.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

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
	
	void SetCurrentBallState(ABall* ball);
	void SetCurrentPlayerState(AMyPlayerState* state);

	void UpdatePower();
	void SetPowerZero();

public:
	TWeakObjectPtr<ABall> CurrentBallState;
	TWeakObjectPtr<AMyPlayerState> CurrentPlayerState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* Power;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* DistanceRemaining;
	
	
};
