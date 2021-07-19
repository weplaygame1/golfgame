// Copyright Epic Games, Inc. All Rights Reserved.


#include "GolfGameGameModeBase.h"


AGolfGameGameModeBase::AGolfGameGameModeBase()
{
	DefaultPawnClass = ABall::StaticClass();
	PlayerStateClass = AMyPlayerState::StaticClass();
}

void AGolfGameGameModeBase::InitGameState()
{
	// 모든 변수들 초기화

	itest = 10;
}
