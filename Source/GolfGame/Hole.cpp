// Fill out your copyright notice in the Description page of Project Settings.


#include "Hole.h"

// Sets default values
AHole::AHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Hole = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HOLE"));
	RootComponent = Hole;
	Hole->SetCollisionProfileName("SetLineTraceChannel");



}

// Called when the game starts or when spawned
void AHole::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

