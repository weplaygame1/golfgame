// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GolfGame.h"

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
	void InitHoleCupLocation();
	void InitScoreTable();
	void InitMinimapCenterLocation();
	void InitMinimapWidth();
	void InitFlagIcon();

	int32 GetScoreTable(int index) { return AllScoreTable[index]; }

	FVector GetSpawnLocation(int index) { return AllSpawnLocation[index]; }
	FVector GetHoleCupLocation(int index) { return AllHoleCupLocation[index]; }

	int GetNumOfAllHole() const { return NumOfAllHole; }

	FVector GetMinimapCenterLocation(int index) { return MinimapCenterLocation[index]; }
	int32 GetMinimapWidth(int index) { return MinimapWidth[index]; }
public:

	// 모든 홀의 시작 타수 정보 ex) 파3, 파4, 파5
	// 초기 ScoreTable 값이라고 생각하면될듯
	UPROPERTY(EditAnywhere)
	TArray<int32> AllScoreTable;

	// 모든 홀의 스폰위치
	UPROPERTY(EditAnywhere)
	TArray<FVector> AllSpawnLocation;

	// 모든 홀의 컵 위치
	UPROPERTY(EditAnywhere)
	TArray<FVector> AllHoleCupLocation;

	// 총 홀의 개수 ex)나는 3개만 구성할것이므로 3
	int32 NumOfAllHole;

	// 플래그 아이콘 번호
	UPROPERTY(EditAnywhere)
	TArray<int32> FlagIconIndex;

	//미니맵 좌표전환에서 사용할 변수들
	//캡처 카메라 중심 위치, 미니맵 중심 위치
	TArray<FVector> MinimapCenterLocation;
	//캡처 카메라 너비 Width, 미니맵 너비
	TArray<int32> MinimapWidth;
};
