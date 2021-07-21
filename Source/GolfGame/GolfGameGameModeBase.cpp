// Copyright Epic Games, Inc. All Rights Reserved.


#include "GolfGameGameModeBase.h"


AGolfGameGameModeBase::AGolfGameGameModeBase()
{
	DefaultPawnClass = ABall::StaticClass();
	PlayerStateClass = AMyPlayerState::StaticClass();
}

void AGolfGameGameModeBase::PostLogin(APlayerController* NewPlayer) 
{
	Super::PostLogin(NewPlayer);

	// 모든 변수들 초기화
	NumOfAllHole = 2;
	InitSpawnLocation();
	InitScoreTable();

	// PlayerState에 초기값을 설정해줌
	AMyPlayerState* PS = Cast<AMyPlayerState>(NewPlayer->PlayerState);
	PS->SetInitSocreTable(AllScoreTable);
	PS->SetInitSpawnLocation(AllSpawnLocation);
	PS->SetInitEndHoleIndex(NumOfAllHole);
	PS->SetInitDoublePar(-PS->GetNowHoleScore());




}



void AGolfGameGameModeBase::InitSpawnLocation()
{
	AllSpawnLocation.Add(FVector(1000, 1000, 200));
	AllSpawnLocation.Add(FVector(1000, 18000, 200));
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

TArray<int32> AGolfGameGameModeBase::GetScoreTable()
{
	return AllScoreTable;
}