// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "GolfGame.h"

#include "DrawDebugHelpers.h"

#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "Ball.generated.h"

UCLASS()
class GOLFGAME_API ABall : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABall();


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
	/* Constructor */
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BallMesh;
	UPROPERTY(EditAnywhere)
	USpringArmComponent* BallCameraSpringArm;
	UPROPERTY(EditAnywhere)
	UCameraComponent* BallCamera;
	
	/* When AddImpulse to Ball */
	//UPROPERTY(EditAnywhere)
		FVector fvtemp;
	//UPROPERTY(EditAnywhere)
		FVector av;
	//UPROPERTY(EditAnywhere)
		FVector bv;
	//UPROPERTY(EditAnywhere)
		FVector cv;

	/* when use line trace */
	FVector CurrentBallLocation;
	FVector CurrentBallForwrad;

	FHitResult OutHit;

	UPROPERTY(EditAnywhere)
	FString NowMaterial;


	/* when check current hole name */
	UPROPERTY(EditAnywhere)
	FString CurrentHoleName;

	

	

public:
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

	int32 CurrentHoleNumber;

	bool bCheckOnce;


	float fcheck = 0;


protected:
	void OnPressBallHit();
	void OnRealseBallHit();
	void MoveDirection(float AxisValue);
	void MoveAngle(float AxisValue);
	void GettingPower(float AxisValue);

	void CheckBallisMoiving();
	void UseLineTrace();


};
