// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

//#include "CoreMinimal.h"
#include "GolfGame.h"

#include "Ball.h"
#include "MyPlayerState.h"


#include "GameFramework/GameModeBase.h"
#include "GolfGameGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class GOLFGAME_API AGolfGameGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
	AGolfGameGameModeBase();

public:
	virtual void InitGameState() override;


public:

	//FVector �迭��
	// ��� Ȧ�� ������ġ

	// �Ƹ���,, ó�� �����Ҷ��� ī�޶��� ���� ?

	// ��� Ȧ�� �� ��ġ


	// ��� Ȧ�� ���� Ÿ�� ���� ex) ��3, ��4, ��5
	// �ʱ� ScoreTable ���̶�� �����ϸ�ɵ�


	int32 itest;
};
