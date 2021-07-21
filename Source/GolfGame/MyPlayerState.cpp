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

void AMyPlayerState::SetInitDoublePar(int32 num)
{
	iDoublePar = num;
}

void AMyPlayerState::SetInitEndHoleIndex(int32 num)
{
	EndHoleIndex = num;
}

void AMyPlayerState::PlusScore()
{
	// 더블파일때는 증가시키지 않음
	if (ScoreTable[CurrentHoleIndex] < iDoublePar)
	{
		ScoreTable[CurrentHoleIndex]++;
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


