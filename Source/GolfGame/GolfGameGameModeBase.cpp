// Copyright Epic Games, Inc. All Rights Reserved.


#include "GolfGameGameModeBase.h"

#include "Ball.h"
#include "MyPlayerState.h"
#include "MyPlayerController.h"

AGolfGameGameModeBase::AGolfGameGameModeBase()
{
	PlayerControllerClass = AMyPlayerController::StaticClass();
	PlayerStateClass = AMyPlayerState::StaticClass();
	//DefaultPawnClass = ABall::StaticClass();
	static ConstructorHelpers::FClassFinder<ABall> Ball_BP(TEXT("/Game/Blueprints/MyBall_BP.MyBall_BP_C"));
	if (Ball_BP.Succeeded())
	{
		DefaultPawnClass = Ball_BP.Class;
	}

	/* 게임 정보 초기화 */
	// 코스 갯수
	NumOfAllHole = 2;
	// 시작 위치
	InitSpawnLocation();
	// 홀컴 위치
	InitHoleCupLocation();
	// 점수 테이블
	InitScoreTable();
	// 미니맵 중심 위치
	InitMinimapCenterLocation();
	// 미니맵 너비
	InitMinimapWidth();
}

void AGolfGameGameModeBase::PostLogin(APlayerController* NewPlayer) 
{
	Super::PostLogin(NewPlayer);
	
	// PlayerState에 초기값을 설정해줌
	auto PS = Cast<AMyPlayerState>(NewPlayer->PlayerState);
	PS->SetInitDoublePar((AllScoreTable[0] * 2) - 1);
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
	AllScoreTable.Add(3);
	AllScoreTable.Add(4);
}

void AGolfGameGameModeBase::InitMinimapCenterLocation()
{
	// 미니맵 중심 위치
	MinimapCenterLocation.Add(FVector(11000, 6000, 100));
}

void AGolfGameGameModeBase::InitMinimapWidth()
{
	// 미니맵 너비
	MinimapWidth.Add(25000);
}