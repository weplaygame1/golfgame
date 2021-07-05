// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "GolfGame.h"


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
	FVector CurrentBallLocation;

	UPROPERTY(EditAnywhere)
	TArray<FHitResult> OutHits;

	UPROPERTY(EditAnywhere)
		int32 numof;
	
	UPROPERTY(EditAnywhere)
	FVector Startpoint;
	UPROPERTY(EditAnywhere)
	FVector Endpoint;
	
	UPROPERTY(EditAnywhere)
		AActor* atemp;
	UPROPERTY(EditAnywhere)
		UPrimitiveComponent* ptemp;

public:
	bool bCanHitBall;
	bool bIsChargingHit;
	bool bIsMoving;
	float JumpPower;



	   



protected:
	void OnPressBallHit();
	void OnRealseBallHit();
	void ViewMove(float AxisValue);
	void GettingPower(float AxisValue);



};
