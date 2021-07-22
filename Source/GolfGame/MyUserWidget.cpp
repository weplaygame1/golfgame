// Fill out your copyright notice in the Description page of Project Settings.


#include "MyUserWidget.h"

bool UMyUserWidget::Initialize()
{
	Super::Initialize();

	Bar = Cast<UProgressBar>(GetWidgetFromName(TEXT("Power")));
	




	return true;
}

