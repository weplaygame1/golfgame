// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "GolfGame.h"


#include "GameFramework/Actor.h"
#include "Course.generated.h"

UCLASS()
class GOLFGAME_API ACourse : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACourse();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Ground;


	



};
