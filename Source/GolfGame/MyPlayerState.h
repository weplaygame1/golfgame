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
	// ���� ġ���� ��ġ
	FVector FormerBallLocation;

	// ���� �迭
	TArray<int32> ScoreTable;

	// ���� �������� Ȧ �ε���
	int32 CurrentHoleIndex; 
	/*
	// Set CurrentHoleName
	CurrentHoleNumber = 0;
	CurrentHoleName = FString(TEXT("Hole")) + FString::FromInt(CurrentHoleNumber);
	*/

	// Ŭ�� ����

	// ���� Ŭ�� or ���� Ŭ�� �ε���
};
