// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "GolfGame.h"

#include "Hole.h"

#include "GameFramework/Pawn.h"
#include "Ball.generated.h"

UCLASS()
class GOLFGAME_API ABall : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABall();

	USphereComponent* GetBallCollision() const { return BallCollision; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BallMesh;
	UPROPERTY(EditAnywhere)
	USphereComponent* BallCollision;
	UPROPERTY(EditAnywhere)
	USpringArmComponent* BallCameraSpringArm;
	UPROPERTY(EditAnywhere)
	UCameraComponent* BallCamera;
	

	UPROPERTY(EditAnywhere)
		FVector GetBallLocation;


	UPROPERTY(EditAnywhere)
	FVector CurrentForwrad;

	UPROPERTY(EditAnywhere)
	FRotator frtemp;


	UPROPERTY(EditAnywhere)
		FVector nowgpav;
	UPROPERTY(EditAnywhere)
		FVector nowsp;

	UPROPERTY(EditAnywhere)
		float maxspeed;
	UPROPERTY(EditAnywhere)
		float maxrotate;

	//UPROPERTY(EditAnywhere)
		FVector fvtemp;
	//UPROPERTY(EditAnywhere)
		FVector av;
	//UPROPERTY(EditAnywhere)
		FVector bv;
	//UPROPERTY(EditAnywhere)
		FVector cv;


	//UPROPERTY(EditAnywhere)
	FVector CurrentBallLocation;
	//UPROPERTY(EditAnywhere)
	TArray<FHitResult> OutHits;
	AActor* Atemp;
	UPrimitiveComponent* Ptemp;

	//UPROPERTY(EditAnywhere)
	FString CurrentHoleName;
	//UPROPERTY(EditAnywhere)
	FName CurrentComponentName;
	//UPROPERTY(EditAnywhere)
	FString CureentActorName;

	
	

public:
	UPROPERTY(EditAnywhere)
	bool bCanHitBall;

	bool bIsChargingHit;
	bool bIsMoving;
	float JumpPower;
	float JumpAngle;

	int32 CurrentHoleNumber;

	   



protected:
	void OnPressBallHit();
	void OnRealseBallHit();
	void MoveDirection(float AxisValue);
	void MoveAngle(float AxisValue);
	void GettingPower(float AxisValue);

	void CheckBallisMoiving();
	void UseLineTrace();


};
