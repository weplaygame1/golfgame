﻿// Copyright Epic Games, Inc. All Rights Reserved.


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

	/* 게임 정보 초기화 → 코드에서 설정하는게 아니라 데이터 테이블로 바꿀 수 있음 */
	// 코스 갯수
	NumOfAllHole = 3;
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
	// 플래그 아이콘 인덱스
	InitFlagIcon();
}

void AGolfGameGameModeBase::PostLogin(APlayerController* NewPlayer) 
{
	Super::PostLogin(NewPlayer);
	
	// PlayerState에 초기값을 설정해줌
	auto PS = Cast<AMyPlayerState>(NewPlayer->PlayerState);
	PS->SetInitDoublePar((AllScoreTable[0] * 2) - 1);
	PS->SetCurrentGameMode(this);
	PS->WholeDistacne = FVector::Dist(AllSpawnLocation[0], AllHoleCupLocation[0]) / 100 + 0.5;
}

void AGolfGameGameModeBase::InitSpawnLocation()
{
	// 시작 위치
	AllSpawnLocation.Add(FVector(1500, 6700, 200));
	AllSpawnLocation.Add(FVector(-15000, -11000, 200));
	AllSpawnLocation.Add(FVector(-22000, 30000, 200));
}

void AGolfGameGameModeBase::InitHoleCupLocation()
{
	// 홀컵 위치
	AllHoleCupLocation.Add(FVector(19070, 5900, 3));
	AllHoleCupLocation.Add(FVector(18500, -13000, 3));
	AllHoleCupLocation.Add(FVector(23000, 23000, 3));
}

void AGolfGameGameModeBase::InitScoreTable()
{
	// Par 수
	AllScoreTable.Add(3);
	AllScoreTable.Add(4);
	AllScoreTable.Add(5);
}

void AGolfGameGameModeBase::InitMinimapCenterLocation()
{
	// 미니맵 중심 위치
	MinimapCenterLocation.Add(FVector(11000, 6000, 10000));
	MinimapCenterLocation.Add(FVector(4000, -13000, 10000));
	MinimapCenterLocation.Add(FVector(400, 30000, 10000));
}

void AGolfGameGameModeBase::InitMinimapWidth()
{
	// 미니맵 너비
	MinimapWidth.Add(25000);
	MinimapWidth.Add(50000);
	MinimapWidth.Add(70000);
}

void AGolfGameGameModeBase::InitFlagIcon()
{
	// 플래그 아이콘 번호
	FlagIconIndex.Add(2);
	FlagIconIndex.Add(1);
	FlagIconIndex.Add(0);
}
