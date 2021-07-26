// Copyright Epic Games, Inc. All Rights Reserved.


#include "GolfGameGameModeBase.h"


AGolfGameGameModeBase::AGolfGameGameModeBase()
{
	DefaultPawnClass = ABall::StaticClass();
	PlayerStateClass = AMyPlayerState::StaticClass();
	PlayerControllerClass = AMyPlayerController::StaticClass();

}

void AGolfGameGameModeBase::PostLogin(APlayerController* NewPlayer) 
{
	Super::PostLogin(NewPlayer);

	// 모든 변수들 초기화
	NumOfAllHole = 2;
	InitSpawnLocation();
	InitHoleCupLocation();
	InitScoreTable();

	// PlayerState에 초기값을 설정해줌
	auto PS = Cast<AMyPlayerState>(NewPlayer->PlayerState);
	PS->SetInitSocreTable(AllScoreTable);
	PS->SetInitSpawnLocation(AllSpawnLocation);
	PS->SetInitHoleCupLocation(AllHoleCupLocation);
	PS->SetInitEndHoleIndex(NumOfAllHole);
	PS->SetInitDoublePar(-PS->GetNowHoleScore());
	



}

void AGolfGameGameModeBase::InitSpawnLocation()
{
	AllSpawnLocation.Add(FVector(1000, 1000, 200));
	AllSpawnLocation.Add(FVector(1000, 18000, 200));
}

void AGolfGameGameModeBase::InitHoleCupLocation()
{
	AllHoleCupLocation.Add(FVector(16300, 3700, 3));
	AllHoleCupLocation.Add(FVector(8000, 23000, 3));
}

void AGolfGameGameModeBase::InitScoreTable()
{
	AllScoreTable.Add(-5);
	AllScoreTable.Add(-3);
}

TArray<FVector> AGolfGameGameModeBase::GetSpawnLocation()
{
	return AllSpawnLocation;
}

TArray<FVector> AGolfGameGameModeBase::GetHoleCupLocation()
{
	return AllHoleCupLocation;
}

TArray<int32> AGolfGameGameModeBase::GetScoreTable()
{
	return AllScoreTable;
}