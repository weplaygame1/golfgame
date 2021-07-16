// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "GolfGame.h"



#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class GOLFGAME_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()

	AMyPlayerState();
public:
	void SetFomerLocation(FVector location);
	void NextHole();

public:
	FVector GetFomerLocation() const;
	int32 GetCurrentHoleNumber() const;

public:
	// 공을 치기전 위치
	FVector FormerBallLocation;

	// 점수 배열
	TArray<int32> PlayerScores;

	// 현재 진행 홀 번호
	int32 CurrentHoleNumber; 
	/*
	// Set CurrentHoleName
	CurrentHoleNumber = 0;
	CurrentHoleName = FString(TEXT("Hole")) + FString::FromInt(CurrentHoleNumber);
	*/


};
