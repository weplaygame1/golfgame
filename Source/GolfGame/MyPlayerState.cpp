// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerState.h"

AMyPlayerState::AMyPlayerState()
{
	ClubState = EGolfClub::E_Wood;
	CurrentHoleIndex = 0;
	iDoublePar = 0;
}

void AMyPlayerState::SetFormerLocation(FVector location)
{
	FormerBallLocation = location;
	FormerBallLocation.Z += 10;
}

void AMyPlayerState::SetInitSocreTable(TArray<int32> score)
{
	ScoreTable = score;
}

void AMyPlayerState::SetInitSpawnLocation(TArray<FVector> location)
{
	SpawnLocation = location;
}

void AMyPlayerState::SetInitHoleCupLocation(TArray<FVector> location)
{
	HoleCupLocation = location;
}

void AMyPlayerState::SetInitDoublePar(int32 num)
{
	iDoublePar = num;
}

void AMyPlayerState::SetInitEndHoleIndex(int32 num)
{
	EndHoleIndex = num;
	WholeDistacne = FVector::Dist(SpawnLocation[CurrentHoleIndex], HoleCupLocation[CurrentHoleIndex]);
}

void AMyPlayerState::SetDistanceRemaining()
{
	Distance = FVector::Dist(GetPawn()->GetActorLocation(), HoleCupLocation[CurrentHoleIndex]);
	GetDistanceOnWidget.Broadcast();
}

void AMyPlayerState::PlusScore()
{
	// 더블파일때는 증가시키지 않음
	if (ScoreTable[CurrentHoleIndex] < iDoublePar)
	{
		ScoreTable[CurrentHoleIndex]++;
		GetScoreOnWidget.Broadcast();
	}
}

bool AMyPlayerState::NextHole()
{
	CurrentHoleIndex++;

	// 다음 홀로 진행
	if (CurrentHoleIndex < EndHoleIndex)
	{
		FormerBallLocation = SpawnLocation[CurrentHoleIndex];
		iDoublePar = -ScoreTable[CurrentHoleIndex];
		WholeDistacne= FVector::Dist(SpawnLocation[CurrentHoleIndex],HoleCupLocation[CurrentHoleIndex]);
		
		GetParOnWidget.Broadcast();
		GetScoreOnWidget.Broadcast();
		GetWholeDistanceOnWidget.Broadcast();
		GetMinimapOnWidget.Broadcast();

		return true;
	}
	// 게임이 끝났을때
	else
	{
		return false;
	}
}

FVector AMyPlayerState::GetFormerLocation() const
{
	return FormerBallLocation;
}
FVector AMyPlayerState::GetNextSpawnLocation() const
{
	return SpawnLocation[CurrentHoleIndex];
}

int32 AMyPlayerState::GetNowHoleScore() const
{
	return ScoreTable[CurrentHoleIndex];
}

int32 AMyPlayerState::GetCurrentHoleIndex() const
{
	return CurrentHoleIndex;
}

int32 AMyPlayerState::GetDoublePar() const
{
	return iDoublePar;
}

int32 AMyPlayerState::GetWholeDistance() const
{
	return WholeDistacne;
}

float AMyPlayerState::GetDistanceRemaining() const
{
	return Distance;
}

