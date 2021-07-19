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
	void SetFormerLocation(FVector location);
	void NextHole();
	void PlusScore();

public:
	FVector GetFormerLocation() const;
	int32 GetCurrentHoleIndex() const;

public:
	// 공을 치기전 위치
	FVector FormerBallLocation;

	// 점수 배열
	TArray<int32> ScoreTable;

	// 현재 진행중인 홀 인덱스
	int32 CurrentHoleIndex; 
	/*
	// Set CurrentHoleName
	CurrentHoleNumber = 0;
	CurrentHoleName = FString(TEXT("Hole")) + FString::FromInt(CurrentHoleNumber);
	*/

	// 클럽 종류

	// 현재 클럽 or 현재 클럽 인덱스
};
