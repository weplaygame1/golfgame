// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerState.h"

AMyPlayerState::AMyPlayerState()
{
	CurrentHoleNumber = 0;


}

void AMyPlayerState::SetFomerLocation(FVector location)
{
	FormerBallLocation = location;
	FormerBallLocation.Z = 50;
}

void AMyPlayerState::NextHole()
{
	CurrentHoleNumber++;
}

FVector AMyPlayerState::GetFomerLocation() const
{
	return FormerBallLocation;
}

int32 AMyPlayerState::GetCurrentHoleNumber() const
{
	return CurrentHoleNumber;
}
