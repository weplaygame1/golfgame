// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerState.h"

AMyPlayerState::AMyPlayerState()
{
	CurrentHoleIndex = 0;

	// 게임모드에서 score table을 가져옴
}

void AMyPlayerState::SetFormerLocation(FVector location)
{
	FormerBallLocation = location;
	FormerBallLocation.Z = 50;
}

void AMyPlayerState::NextHole()
{
	CurrentHoleIndex++;
}

void AMyPlayerState::PlusScore()
{
	//ScoreTable[CurrentHoleIndex]++;
}


FVector AMyPlayerState::GetFormerLocation() const
{
	return FormerBallLocation;
}

int32 AMyPlayerState::GetCurrentHoleIndex() const
{
	return CurrentHoleIndex;
}
