// Copyright Epic Games, Inc. All Rights Reserved.


#include "GolfGameGameModeBase.h"
#include "Ball.h"

AGolfGameGameModeBase::AGolfGameGameModeBase()
{
	DefaultPawnClass = ABall::StaticClass();

}