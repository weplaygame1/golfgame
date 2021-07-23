// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

AMyPlayerController::AMyPlayerController()
{
	static ConstructorHelpers::FClassFinder<UMyUserWidget> UUW(TEXT("/Game/Blueprints/Widget/NewBlueprint.NewBlueprint_C"));
	if (UUW.Succeeded())
	{
		PlayerWidget = UUW.Class;
	}
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	CurrentWidget = CreateWidget<UMyUserWidget>(this, PlayerWidget);
	CurrentWidget->AddToViewport();
	
	CurrentWidget->SetCurrentBallState(Cast<class ABall>(GetPawn()));
	CurrentWidget->SetCurrentPlayerState(Cast<class AMyPlayerState>(PlayerState));

	

}

void AMyPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}
