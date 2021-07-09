// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "GolfGame.h"


#include "GameFramework/Actor.h"
#include "Hole.generated.h"

UCLASS()
class GOLFGAME_API AHole : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHole();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ROUGH;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* OB;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BUNKER;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* GREEN;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* HOLECUP;


	
	


};
