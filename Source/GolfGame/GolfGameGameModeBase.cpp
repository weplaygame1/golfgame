// Copyright Epic Games, Inc. All Rights Reserved.


#include "GolfGameGameModeBase.h"


AGolfGameGameModeBase::AGolfGameGameModeBase()
{
	DefaultPawnClass = ABall::StaticClass();
	PlayerStateClass = AMyPlayerState::StaticClass();
}