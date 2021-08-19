// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

#include "MyUserWidget.h"

#include "GolfGameGameModeBase.h"
#include "MyPlayerState.h"
#include "Ball.h"

AMyPlayerController::AMyPlayerController()
{
	static ConstructorHelpers::FClassFinder<UMyUserWidget> UUW(TEXT("/Game/Blueprints/Widget/MainWidget.MainWidget_C"));
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
	
	CurrentWidget->SetCurrentBallState(Cast<ABall>(GetPawn()));
	CurrentWidget->SetCurrentPlayerState(Cast<AMyPlayerState>(PlayerState));
	CurrentWidget->SetCurrentGameMode(GetWorld()->GetAuthGameMode<AGolfGameGameModeBase>());

	CurrentWidget->SetMinimapImage();
	CurrentWidget->OnOffMovingPanel(false);
	CurrentWidget->OnOffOBResult(false);
	CurrentWidget->OnOffConcedeResult(false);
	CurrentWidget->OnOffScoreResult(false);

	CurrentWidget->UpdateWholeDistance();
	CurrentWidget->UpdateBallIcon();
	CurrentWidget->UpdatePredictIcon();
}
