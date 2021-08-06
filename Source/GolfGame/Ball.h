// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GolfGame.h"

#include "GameFramework/Pawn.h"
#include "Ball.generated.h"

DECLARE_MULTICAST_DELEGATE(FUpdateBallStateDelegate);

// 골프 클럽 ENUM CLASS
UENUM()
enum class EGolfClub : uint8
{
	//우드
	//아이언
	//퍼터 
	//우선 이렇게 3개만

	WOOD = 0 UMETA(DisplayName = "WOOD"),
	IRON UMETA(DisplayName = "IRON"),
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
	CHECK UMETA(DisplayName = "CHECK")
};
// 지형속성 ENUM CLASS
UENUM()
enum class EGeographyState : uint8
{
	ROUGH = 0 UMETA(DisplayName = "ROUGH"),
	FAIRWAY UMETA(DisplayName = "FAIRWAY"),
	GREEN UMETA(DisplayName = "GREEN"),
	BUNKER UMETA(DisplayName = "BUNKER")
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
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* BallMesh;
	UPROPERTY(EditAnywhere)
	class USpringArmComponent* BallCameraSpringArm;
	UPROPERTY(EditAnywhere)
	class UCameraComponent* BallCamera;
	
	UPROPERTY(EditAnywhere)
	int32 CheckNowScore;

	/* When AddImpulse to Ball */
	//UPROPERTY(EditAnywhere)
		FVector fvtemp;
	//UPROPERTY(EditAnywhere)
		FVector av;
	//UPROPERTY(EditAnywhere)
		FVector bv;
	//UPROPERTY(EditAnywhere)
		FVector cv;

	/* Club State */
	UPROPERTY(EditAnywhere)
	EGolfClub ClubState;
	float DrivingDis;
	float ArcValue;

	/* when use line trace */
	FHitResult OutHit;
	UPROPERTY(EditAnywhere)
	FString NowMaterial;

	/* Player state */
	class AMyPlayerState* BallPlayerState;

	UPROPERTY(EditAnywhere)
	bool bCanHitBall;
	bool bIsChargingHit;

	UPROPERTY(EditAnywhere)
	bool bIsMoving;
	float JumpPower;
	float JumpAngle;

	// 현재 진행중인 홀을 마쳤는가
	bool bCheckHoleCup;
	bool bCheckConcede;

	// MOVE에서 CHECK 넘어갈때 확인하는 변수
	bool bCheckOnce;

	float PowerPercent = 0;
	bool PowerIncrease = true;

	UPROPERTY(EditAnywhere)
	EBallState CurrentState;
	UPROPERTY(EditAnywhere)
	EGeographyState GeographyState;

	float MovingDis;

	//test
	UPROPERTY(EditAnywhere)
		FString fsttt;
	UPROPERTY(EditAnywhere)
		int32 itestest;

	UPROPERTY(EditAnywhere)
		FVector PredictLocation;


	UPROPERTY(EditAnywhere)
		FVector ftemp1;
	UPROPERTY(EditAnywhere)
		FVector ftemp3; 
	UPROPERTY(EditAnywhere)
		FVector outVelocity;
	UPROPERTY(EditAnywhere)
		FVector outVelocity22;
	UPROPERTY(EditAnywhere)
		int32 itestlen;


public:
	void OnPressBallHit();
	void OnRealseBallHit();
	void OnPressChangeClub();
	void MoveDirection(float AxisValue);

	void CheckBallisMoiving();
	void UseLineTrace();

	void MoveNextHole();

	void ChargingPower();
	void CheckBallLocation();

	void CalPredictLocation();

	void SetMovingDis();

	float GetPower() { return PowerPercent / 100; }
	FVector GetPredictLocation() const { return PredictLocation; }

	float GetDrivingDis() { return DrivingDis; }
	float GetMovingDis() { return MovingDis; }
	EGolfClub GetClubState() { return ClubState; }

	EGeographyState GetGeographyState() { return GeographyState; }
};
