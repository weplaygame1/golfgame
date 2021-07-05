// Fill out your copyright notice in the Description page of Project Settings.


#include "Course.h"

// Sets default values
ACourse::ACourse()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set Ground
	Ground = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GROUND"));
	RootComponent = Ground;
	Ground->SetCollisionProfileName("SetLineTraceChannel");

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_GROUND(TEXT("/Game/StarterContent/Architecture/Floor_400x400.Floor_400x400"));
	if (SM_GROUND.Succeeded())
	{
		Ground->SetStaticMesh(SM_GROUND.Object);
		Ground->SetWorldScale3D(FVector(1000.0f, 1000.0f, 1.0f));
		Ground->SetWorldLocation(FVector(0.0f, 0.0f, 0.0f));
	}
	
	

	
}

// Called when the game starts or when spawned
void ACourse::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACourse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
