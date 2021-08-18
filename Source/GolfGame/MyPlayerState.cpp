// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerState.h"

#include "GolfGameGameModeBase.h"

AMyPlayerState::AMyPlayerState()
{
	CurrentHoleIndex = 0;
	iDoublePar = 0;
	NowHoleScore = 0;
	AllScore = 0;
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
	/*
	// 더블파일때는 증가시키지 않음 -> 규칙 체크 필요 !
	if (ScoreTable[CurrentHoleIndex] < iDoublePar)
	{
		ScoreTable[CurrentHoleIndex]++;
		GetScoreOnWidget.Broadcast();
	}
	*/

	NowHoleScore++;
	GetScoreOnWidget.Broadcast();
}

bool AMyPlayerState::NextHole()
{
	// 이번 홀 점수 저장
	ScoreTable.Add(NowHoleScore);
	// 총 스코어 갱신
	AllScore += (NowHoleScore - CurrentGameMode->GetScoreTable(CurrentHoleIndex));
	NowHoleScore = 0;

	CurrentHoleIndex++;

	// 다음 홀로 진행
	if (CurrentHoleIndex < CurrentGameMode->GetNumOfAllHole())
	{
		FormerBallLocation = CurrentGameMode->GetSpawnLocation(CurrentHoleIndex);
		iDoublePar = (CurrentGameMode->GetScoreTable(CurrentHoleIndex) * 2) - 1;
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