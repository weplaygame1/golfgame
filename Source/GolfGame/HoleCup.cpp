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
	PrimaryActorTick.bCanEverTick = true;

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

	// 컨시드의 반지름을 다시 정해줘야함
	
}

// Called every frame
void AHoleCup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Draw HoleCup
	DrawCircle(GetWorld(), GetActorLocation(), FVector(1, 0, 0), FVector(0, 1, 0), FColor::Red, 250, 100, false, -1, 0, 5);

	// Draw Concede
	DrawCircle(GetWorld(), GetActorLocation(),FVector(1, 0, 0), FVector(0, 1, 0),FColor::Blue, 800, 100, false, -1, 0, 5);
	
}

