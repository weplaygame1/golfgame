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
	USpringArmComponent* BallCameraSpringArm;
	UPROPERTY(EditAnywhere)
	UCameraComponent* BallCamera;
	

	UPROPERTY(EditAnywhere)
	FVector GetBallLocation;

	UPROPERTY(EditAnywhere)
	FVector CurrentForwrad;


	UPROPERTY(EditAnywhere)
		FVector fvtemp;
	UPROPERTY(EditAnywhere)
		FVector av;
	UPROPERTY(EditAnywhere)
		FVector bv;
	UPROPERTY(EditAnywhere)
		FVector cv;




	/* when use line trace */
	//UPROPERTY(EditAnywhere)
	FVector CurrentBallLocation;
	//UPROPERTY(EditAnywhere)
	TArray<FHitResult> OutHits;
	AActor* Atemp;
	UPrimitiveComponent* Ptemp;



	/* when check current hole name */
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
