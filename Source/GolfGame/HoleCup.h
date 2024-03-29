﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GolfGame.h"

#include "GameFramework/Actor.h"
#include "HoleCup.generated.h"

UCLASS()
class GOLFGAME_API AHoleCup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHoleCup();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* HoleCup;
	UPROPERTY(VisibleAnywhere)
	class USphereComponent* Concede;
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Flag;
};
