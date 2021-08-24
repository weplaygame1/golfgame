// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GolfGame.h"

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
	
	void SetCurrentBallState(class ABall* ball);
	void SetCurrentPlayerState(class AMyPlayerState* state);
	void SetCurrentGameMode(class AGolfGameGameModeBase* mode);

	void UpdatePower();
	void SetPowerZero();
	void UpdateWholeDistance();
	void UpdateDistance();
	void UpdatePar();
	void UpdateScore();
	void SetMinimapImage();
	void UpdateBallIcon();
	void UpdatePredictIcon();
	void UpdateClubState();
	void UpdateMovingInformation();
	void UpdateGeoState();
	void UpdateShotNumberth();
	void UpdateScoreResult();
	void UpdateScoreTable();
	void UpdatePreAndFlag();

	UFUNCTION()
	void ClickGameOver();

	void OnOffMainPanel(bool on);
	void OnOffMovingPanel(bool on);
	void OnOffOBResult(bool on);
	void OnOffConcedeResult(bool on);
	void OnOffScoreResult(bool on);
	void OnOffScoreTable(bool on);
	void OnOffGameOverButton(bool on);
public:
	TWeakObjectPtr<class ABall> CurrentBallState;
	TWeakObjectPtr<class AMyPlayerState> CurrentPlayerState;
	TWeakObjectPtr<class AGolfGameGameModeBase> CurrentGameMode;

	/* 메인 패널 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* MainPanel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* WholeDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* DistanceRemaining;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* ShowPar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* NowScore;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* ClubState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* GeoState_1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* ShotNumberth;

	/* 결과 패널 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* OB_Result;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Concede_Result;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Score_Result;

	// 게이지바
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* Power;

	// 미니맵
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* Minimap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* BallIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* FlagIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* PredictIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* PreAndFlag;
	
	/* 무빙 패널 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* MovingPanel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* MovingDis;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* RemainingDis; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* GeoState_0;
	
	/* 스코어 표 패널*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* ScorePanel;
	//하나하나 변수로 구현하는게 아니라 리스트처럼 할 수 있을텐데 잘 모르겠음.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* ScorePar0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* ScorePar1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* ScorePar2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TotalPar;
	
	int32 iTotalPar = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Score0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Score1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Score2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TotalScore;

	int32 iTotalScore = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* ScoreDis0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* ScoreDis1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* ScoreDis2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* GameOver;
};
