// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GolfGame.h"

#include "GameFramework/Pawn.h"
#include "Ball.generated.h"

DECLARE_MULTICAST_DELEGATE(FUpdateBallStateDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FDele_Multi_OneParam, bool);

// 골프 클럽 ENUM CLASS
UENUM()
enum class EGolfClub : uint8
{
	DRIVER = 0 UMETA(DisplayName = "DRIVER"),
	WOOD UMETA(DisplayName = "WOOD"),
	IRON UMETA(DisplayName = "IRON"),
	WEDGE UMETA(DisplayName = "WEDGE"),
	PUTTER UMETA(DisplayName = "PUTTER")
};

// 공 상태 ENUM CLASS
UENUM()
enum class EBallState : uint8
{
	STOP = 0 UMETA(DisplayName = "STOP"),
	READY UMETA(DisplayName = "READY"),
	CHARGING UMETA(DisplayName = "CHARGING"),
	MOVING UMETA(DisplayName = "MOVING"),
	CHECK UMETA(DisplayName = "CHECK"),
	PAUSE UMETA(DisplayName = "PAUSE")
};
// 지형속성 ENUM CLASS
UENUM()
enum class EGeographyState : uint8
{
	ROUGH = 0 UMETA(DisplayName = "ROUGH"),
	FAIRWAY UMETA(DisplayName = "FAIRWAY"),
	GREEN UMETA(DisplayName = "GREEN"),
	BUNKER UMETA(DisplayName = "BUNKER"),
	OB UMETA(DisplayName = "OB"),
	HOLECUP UMETA(DisplayName = "HOLECUP"),
	CONCEDE UMETA(DisplayName = "CONCEDE")
};

UCLASS()
class GOLFGAME_API ABall : public APawn
{
	GENERATED_BODY()
public:
	// Sets default values for this pawn's properties
	ABall();

	FUpdateBallStateDelegate GetPowerGaugeOnWidget;
	FUpdateBallStateDelegate SetPowerZeroOnWidget;
	FUpdateBallStateDelegate UpdateBallIconOnWidget;
	FUpdateBallStateDelegate UpdatePredictIconOnWidget;
	FUpdateBallStateDelegate UpdateClubStateOnWidget;
	FUpdateBallStateDelegate UpdateMovingInfoOnWidget;
	FUpdateBallStateDelegate UpdateGeoStateOnWidget;
	FUpdateBallStateDelegate UpdateShotNumberthOnWidget;
	FUpdateBallStateDelegate UpdateScoreResultOnWidget;
	FUpdateBallStateDelegate UpdateScoreTableOnWidget;

	FDele_Multi_OneParam OnOffMainPanelOnWidget;
	FDele_Multi_OneParam OnOffMovingPanelOnWidget;
	FDele_Multi_OneParam OnOffOBResultOnWidget;
	FDele_Multi_OneParam OnOffConcedeResultOnWidget;
	FDele_Multi_OneParam OnOffOnScoreResultOnWidget;
	FDele_Multi_OneParam OnOffOnScoreTableOnWidget;
	FDele_Multi_OneParam OnOffOnGameOverButtonOnWidget;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
public:
	/* 생성자 */
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* BallMesh;
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* BallCameraSpringArm;
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* BallCamera;

	/* Club State */
	EGolfClub ClubState;
	float DrivingDis;
	float ArcValue;

	/* Player state */
	TWeakObjectPtr<class AMyPlayerState> BallPlayerState;

	/* Game mode */
	TWeakObjectPtr<class AGolfGameGameModeBase> CurrentGameMode;

	bool bIsMoving;

	// 현재 진행중인 홀을 마쳤는가
	bool bCheckHoleCup;
	bool bCheckConcede;

	// MOVE에서 CHECK 넘어갈때 확인하는 변수
	bool bCheckOnce;

	float PowerPercent = 0;
	bool PowerIncrease = true;
	
	EBallState CurrentState;
	EGeographyState GeographyState;

	float MovingDis;
	FVector PredictLocation;
	bool bCheckOB;
	bool bWaitTimer;
	int32 numberth;

	// 공과 홀컵 사이 거리
	float BallHoleDis;

	// 블루프린트, 나이아가라 set visibility 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bOnEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bOffEffect;
public:
	void OnPressBallHit();
	void OnRealseBallHit();
	void OnPressChangeClub();
	void OnPressCheat();
	void MoveDirection(float AxisValue);
	void CheckBallisMoiving();
	void UseLineTrace();
	void MoveNextHole();
	void ChargingPower();
	void CheckBallLocation();
	void ChangeClub();
	void SetMovingDis();
	void ChangeClubFromDis();
	void UseInTimer();
	void SetBallLoc();
	void SetPredictLocation();

	float GetPower() { return PowerPercent / 100; }
	float GetDrivingDis() { return DrivingDis; }
	float GetMovingDis() { return MovingDis; }
	FVector GetPredictLocation() const { return PredictLocation; }
	EGolfClub GetClubState() { return ClubState; }
	EGeographyState GetGeographyState() { return GeographyState; }
};
