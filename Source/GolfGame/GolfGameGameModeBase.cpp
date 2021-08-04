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

	// 게임 정보 초기화
	// 코스 갯수
	NumOfAllHole = 2;
	// 시작 위치
	InitSpawnLocation();
	// 홀컴 위치
	InitHoleCupLocation();
	// 점수 테이블
	InitScoreTable();
}

void AGolfGameGameModeBase::PostLogin(APlayerController* NewPlayer) 
{
	Super::PostLogin(NewPlayer);
	
	// PlayerState에 초기값을 설정해줌
	auto PS = Cast<AMyPlayerState>(NewPlayer->PlayerState);
	PS->SetInitSocreTable(AllScoreTable);
	PS->SetInitDoublePar((PS->GetNowHoleScore()) * (-1));
	PS->SetCurrentGameMode(this);
	PS->WholeDistacne = FVector::Dist(AllSpawnLocation[0], AllHoleCupLocation[0]) / 100;


}

void AGolfGameGameModeBase::InitSpawnLocation()
{
	// 시작 위치
	AllSpawnLocation.Add(FVector(2000, 6200, 200));
	AllSpawnLocation.Add(FVector(1000, 18000, 200));
}

void AGolfGameGameModeBase::InitHoleCupLocation()
{
	// 홀컵 위치
	AllHoleCupLocation.Add(FVector(19000, 6300, 3));
	AllHoleCupLocation.Add(FVector(8000, 23000, 3));
}

void AGolfGameGameModeBase::InitScoreTable()
{
	// Par 수
	AllScoreTable.Add(-3);
	AllScoreTable.Add(-4);
}

void AGolfGameGameModeBase::InitMinimapCenterLocation()
{
	MinimapCenterLocation.Add(FVector(11000, 6000, 100));
}

void AGolfGameGameModeBase::InitMinimapWidth()
{
	MinimapWidth.Add(25000);
}