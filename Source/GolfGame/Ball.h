// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "GolfGame.h"
#include "Kismet/GameplayStatics.h"

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

	virtual void NotifyHit(UPrimitiveComponent *MyComp, AActor *Other, UPrimitiveComponent *OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult &Hit) override;
	virtual void NotifyActorBeginOverlap(AActor *OtherActor) override; 
	virtual void NotifyActorEndOverlap(AActor *OtherActor) override;



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

	UPROPERTY(EditAnywhere)
	FString CureentActorName;


	

public:
	UPROPERTY(EditAnywhere)
	bool bCanHitBall;
	bool bIsChargingHit;

	UPROPERTY(EditAnywhere)
	bool bIsMoving;
	float JumpPower;
	float JumpAngle;

	// 현재 진행중인 홀을 마쳤는가
	bool bCheckHoleOut;
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
