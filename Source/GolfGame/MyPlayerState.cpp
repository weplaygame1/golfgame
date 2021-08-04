// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerState.h"

#include "GolfGameGameModeBase.h"

AMyPlayerState::AMyPlayerState()
{
	CurrentHoleIndex = 0;
	iDoublePar = 0;
}

void AMyPlayerState::SetCurrentGameMode(class AGolfGameGameModeBase* mode)
{
	CurrentGameMode = mode;
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

void AMyPlayerState::SetInitDoublePar(int32 num)
{
	iDoublePar = num;
}

void AMyPlayerState::SetDistanceRemaining()
{
	Distance = FVector::Dist(GetPawn()->GetActorLocation(), CurrentGameMode->GetHoleCupLocation(CurrentHoleIndex)) / 100;
	GetDistanceOnWidget.Broadcast();
}

void AMyPlayerState::PlusScore()
{
	// 더블파일때는 증가시키지 않음 -> 규칙 체크 필요 !
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
	if (CurrentHoleIndex < CurrentGameMode->GetNumOfAllHole())
	{
		FormerBallLocation = CurrentGameMode->GetSpawnLocation(CurrentHoleIndex);
		iDoublePar = -ScoreTable[CurrentHoleIndex];
		WholeDistacne = FVector::Dist(FormerBallLocation, CurrentGameMode->GetHoleCupLocation(CurrentHoleIndex)) / 100;
		
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