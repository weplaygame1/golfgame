// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "GolfGame.h"

#include "MyUserWidget.h"

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


	virtual void OnPossess(APawn* aPawn) override;
	virtual void BeginPlay() override;
	

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UMyUserWidget> Widget_MainClass;
	UPROPERTY(EditAnywhere)
	UMyUserWidget* CurrentWidget;

};
