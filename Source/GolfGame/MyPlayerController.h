// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "GolfGame.h"

#include "MyUserWidget.h"
#include "MyPlayerState.h"

#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GOLFGAME_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMyPlayerController();

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* aPawn) override;

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UMyUserWidget> PlayerWidget;
	UPROPERTY(EditAnywhere)
	UMyUserWidget* CurrentWidget;

};
