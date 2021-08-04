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

	int32 GetNowHoleScore() const { return ScoreTable[CurrentHoleIndex]; }
	int32 GetCurrentHoleIndex() const { return CurrentHoleIndex; }
	int32 GetDoublePar() const { return iDoublePar; }
	int32 GetWholeDistance() const { return WholeDistacne; }

	float GetDistanceRemaining() const { return Distance; }

public:
	TWeakObjectPtr<class AGolfGameGameModeBase> CurrentGameMode;

	// 공을 치기전 위치
	FVector FormerBallLocation;

	// 플레이어의 점수 배열
	//-> 스코어카드 UI를 만들때 gamemode를 가져오지 못하면 초기 점수표를 추가로 만들어야함
	UPROPERTY(EditAnywhere)
	TArray<int32> ScoreTable;

	// 더블파 체크 변수
	UPROPERTY(EditAnywhere)
	int32 iDoublePar;

	// 현재 진행중인 홀 인덱스
	UPROPERTY(EditAnywhere)
	int32 CurrentHoleIndex; 

	// 남은 거리
	UPROPERTY(EditAnywhere)
	float Distance;

	// 처음부터의 총 거리
	UPROPERTY(EditAnywhere)
	int WholeDistacne;

};
