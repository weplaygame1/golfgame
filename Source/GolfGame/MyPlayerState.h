// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GolfGame.h"

#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE(FUpdatePlayerStateDelegate);

UCLASS()
class GOLFGAME_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	AMyPlayerState();

	FUpdatePlayerStateDelegate GetDistanceOnWidget;
	FUpdatePlayerStateDelegate GetParOnWidget;
	FUpdatePlayerStateDelegate GetScoreOnWidget;
	FUpdatePlayerStateDelegate GetWholeDistanceOnWidget;
	FUpdatePlayerStateDelegate GetMinimapOnWidget;
public:
	void SetCurrentGameMode(class AGolfGameGameModeBase* mode);
	
	void SetFormerLocation(FVector location);
	void SetInitSocreTable(TArray<int32> score);
	void SetInitDoublePar(int32 num);
	void SetDistanceRemaining();
	void PlusScore();
	bool NextHole();
public:
	FVector GetFormerLocation() const { return FormerBallLocation; }
	int32 GetCurrentHoleIndex() const { return CurrentHoleIndex; }
	int32 GetDoublePar() const { return iDoublePar; }
	int32 GetWholeDistance() const { return WholeDistacne; }
	int32 GetAllScore() const { return AllScore; }
	float GetDistanceRemaining() const { return Distance; }
	int32 GetNowHoleScore() const { return NowHoleScore; }
	int32 GetNumberth() const { return NowHoleScore; }
public:
	TWeakObjectPtr<class AGolfGameGameModeBase> CurrentGameMode;

	// 공을 치기전 위치
	FVector FormerBallLocation;
	// 플레이어의 점수 배열
	TArray<int32> ScoreTable;
	// 더블파 체크 변수
	int32 iDoublePar;
	// 현재 진행중인 홀 인덱스
	int32 CurrentHoleIndex; 
	// 남은 거리
	float Distance;
	// 처음부터의 총 거리
	int WholeDistacne;
	// 이번 홀에서 친 횟수
	int32 NowHoleScore;
	// 총 점수
	int32 AllScore;
};
