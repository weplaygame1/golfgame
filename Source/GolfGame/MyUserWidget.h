// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "GolfGame.h"

#include "Components/ProgressBar.h"


#include "Blueprint/UserWidget.h"
#include "MyUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class GOLFGAME_API UMyUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	virtual bool Initialize();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* Bar;


	
};
