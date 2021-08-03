// Copyright Epic Games, Inc. All Rights Reserved.


#include "GolfGameGameModeBase.h"

#include "Ball.h"
#include "MyPlayerState.h"
#include "MyPlayerController.h"

AGolfGameGameModeBase::AGolfGameGameModeBase()
{
	PlayerControllerClass = AMyPlayerController::StaticClass();
	PlayerStateClass = AMyPlayerState::StaticClass();
	DefaultPawnClass = ABall::StaticClass();
}

void AGolfGameGameModeBase::PostLogin(APlayerController* NewPlayer) 
{
	Super::PostLogin(NewPlayer);

	// 게임 정보 초기화
	// 코스의 갯수, 시작 위치, 홀컴 위치, 점수 테이블
	NumOfAllHole = 2;
	InitSpawnLocation();
	InitHoleCupLocation();
	InitScoreTable();

	// PlayerState에 초기값을 설정해줌
	auto PS = Cast<AMyPlayerState>(NewPlayer->PlayerState);
	PS->SetInitSocreTable(AllScoreTable);
	PS->SetInitDoublePar((PS->GetNowHoleScore()) * (-1));
	PS->SetCurrentGameMode(this);
	PS->WholeDistacne = FVector::Dist(AllSpawnLocation[0], AllHoleCupLocation[0]);


}

void AGolfGameGameModeBase::InitSpawnLocation()
{
	// 시작 위치
	AllSpawnLocation.Add(FVector(1620, 6080, 200));
	AllSpawnLocation.Add(FVector(1000, 18000, 200));
}

void AGolfGameGameModeBase::InitHoleCupLocation()
{
	// 홀컵 위치
	AllHoleCupLocation.Add(FVector(12040, 4160, 3));
	AllHoleCupLocation.Add(FVector(8000, 23000, 3));
}

void AGolfGameGameModeBase::InitScoreTable()
{
	// Par 수
	AllScoreTable.Add(-5);
	AllScoreTable.Add(-3);
}

TArray<int32> AGolfGameGameModeBase::GetScoreTable()
{
	return AllScoreTable;
}

FVector AGolfGameGameModeBase::GetSpawnLocation(int index)
{
	return AllSpawnLocation[index];
}

FVector AGolfGameGameModeBase::GetHoleCupLocation(int index)
{
	return AllHoleCupLocation[index];
}

int AGolfGameGameModeBase::GetNumOfAllHole() const
{
	return NumOfAllHole;
}
