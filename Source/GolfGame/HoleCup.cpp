// Fill out your copyright notice in the Description page of Project Settings.


#include "HoleCup.h"

#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

#include "DrawDebugHelpers.h"

// Sets default values
AHoleCup::AHoleCup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	HoleCup = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HOLECUP"));
	Concede = CreateDefaultSubobject<USphereComponent>(TEXT("CONCEDE"));
	Flag = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FLAG"));
	Concede->SetSphereRadius(300);

	RootComponent = HoleCup;
	Concede->SetupAttachment(RootComponent);
	Flag->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AHoleCup::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AHoleCup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}