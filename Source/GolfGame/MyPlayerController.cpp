// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

AMyPlayerController::AMyPlayerController()
{
	static ConstructorHelpers::FClassFinder<UMyUserWidget> UUW(TEXT("/Game/Blueprints/Widget/NewBlueprint.NewBlueprint_C"));
	if (UUW.Succeeded())
	{
		Widget_MainClass = UUW.Class;
	}
		
		


}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	CurrentWidget = CreateWidget<UMyUserWidget>(this, Widget_MainClass);
	CurrentWidget->AddToViewport();
}


void AMyPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	

}