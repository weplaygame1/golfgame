// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GolfGame.h"

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
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UMyUserWidget> PlayerWidget;
	UPROPERTY(EditAnywhere)
	class UMyUserWidget* CurrentWidget;
};
