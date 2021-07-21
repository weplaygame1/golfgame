// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

//#include "CoreMinimal.h"
#include "GolfGame.h"

#include "Ball.h"
#include "MyPlayerState.h"


#include "GameFramework/GameModeBase.h"
#include "GolfGameGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class GOLFGAME_API AGolfGameGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
	AGolfGameGameModeBase();
	virtual void PostLogin(APlayerController* NewPlayer);
public:
	void InitSpawnLocation();
	void InitScoreTable();

public:

	// 모든 홀의 시작 타수 정보 ex) 파3, 파4, 파5
	// 초기 ScoreTable 값이라고 생각하면될듯
	UPROPERTY(EditAnywhere)
	TArray<int32> AllScoreTable;

	// 모든 홀의 스폰위치
	UPROPERTY(EditAnywhere)
	TArray<FVector> AllSpawnLocation;

	// 아마도,, 처음 진입할때의 카메라의 방향 ?

	// 모든 홀의 컵 위치

	// 총 홀의 개수 ex)나는 3개만 구성할것이므로 3
	int32 NumOfAllHole;

	
public:
	TArray<FVector> GetSpawnLocation();
	TArray<int32> GetScoreTable();
};
