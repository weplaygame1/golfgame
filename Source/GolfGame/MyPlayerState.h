// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "GolfGame.h"


#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE(FUpdatePlayerStateDelegate);

// 골프클럽 ENUM CLASS
UENUM()
enum class EGolfClub : uint8
{
	//우드
	//아이언
	//퍼터 
	//우선 이렇게 3개만

	E_Wood = 0 UMETA(DisplayName = "Wood"),
	E_Iron UMETA(DisplayName = "Iron"),
	E_Putter UMETA(DisplayName = "Putter")
};

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
public:
	void SetFormerLocation(FVector location);
	void SetInitSocreTable(TArray<int32> score);
	void SetInitSpawnLocation(TArray<FVector> location);
	void SetInitHoleCupLocation(TArray<FVector> location);
	void SetInitDoublePar(int32 num);
	void SetInitEndHoleIndex(int32 num);
	void SetDistanceRemaining();

	void PlusScore();
	bool NextHole();

public:
	FVector GetFormerLocation() const;
	FVector GetNextSpawnLocation() const;

	int32 GetNowHoleScore() const;
	int32 GetCurrentHoleIndex() const;
	int32 GetDoublePar() const;
	int32 GetWholeDistance() const;

	float GetDistanceRemaining() const;

public:
	// 공을 치기전 위치
	FVector FormerBallLocation;

	// 점수 배열 -> 스코어카드 UI를 만들때 gamemode를 가져오지 못하면 초기 점수표를 추가로 만들어야함
	UPROPERTY(EditAnywhere)
	TArray<int32> ScoreTable;

	// 더블파 체크 변수
	UPROPERTY(EditAnywhere)
	int32 iDoublePar;

	// 각 홀 시작 위치
	UPROPERTY(EditAnywhere)
	TArray<FVector> SpawnLocation;

	// 각 홀 컵 위치
	UPROPERTY(EditAnywhere)
	TArray<FVector> HoleCupLocation;

	// 현재 진행중인 홀 인덱스
	UPROPERTY(EditAnywhere)
	int32 CurrentHoleIndex; 

	// 총 홀의 개수 
	UPROPERTY(EditAnywhere)
	int32 EndHoleIndex;

	// 현재 클럽 상태
	EGolfClub ClubState;

	// 남은 거리
	UPROPERTY(EditAnywhere)
	float Distance;

	int WholeDistacne;

	
	/*
	// Set CurrentHoleName
	CurrentHoleNumber = 0;
	CurrentHoleName = FString(TEXT("Hole")) + FString::FromInt(CurrentHoleNumber);
	*/

};
