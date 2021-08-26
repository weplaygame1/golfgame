// Fill out your copyright notice in the Description page of Project Settings.

#include "MyUserWidget.h"

#include "Components/CanvasPanel.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "Components/Button.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

#include "GolfGameGameModeBase.h"
#include "MyPlayerState.h"
#include "Ball.h"

bool UMyUserWidget::Initialize()
{
	Super::Initialize();

	GameOver->OnClicked.AddDynamic(this, &UMyUserWidget::ClickGameOver);

	return true;
}

void UMyUserWidget::SetCurrentGameMode(AGolfGameGameModeBase* mode)
{
	CurrentGameMode = mode;
}

void UMyUserWidget::SetCurrentBallState(ABall* ball) 
{
	// 멀티델리게이트 있으니까 일대일 매칭말고 기능별로 묶어서 델리게이트 수를 줄일수도 있겠지?
	CurrentBallState = ball; 
	ball->GetPowerGaugeOnWidget.AddUObject(this, &UMyUserWidget::UpdatePower);
	ball->SetPowerZeroOnWidget.AddUObject(this, &UMyUserWidget::SetPowerZero);
	ball->UpdateBallIconOnWidget.AddUObject(this,&UMyUserWidget::UpdateBallIcon);
	ball->UpdatePredictIconOnWidget.AddUObject(this, &UMyUserWidget::UpdatePredictIcon);
	ball->UpdateClubStateOnWidget.AddUObject(this, &UMyUserWidget::UpdateClubState);
	ball->UpdateMovingInfoOnWidget.AddUObject(this, &UMyUserWidget::UpdateMovingInformation);
	ball->UpdateGeoStateOnWidget.AddUObject(this, &UMyUserWidget::UpdateGeoState);
	ball->UpdateShotNumberthOnWidget.AddUObject(this, &UMyUserWidget::UpdateShotNumberth);
	ball->UpdateScoreResultOnWidget.AddUObject(this, &UMyUserWidget::UpdateScoreResult);
	ball->UpdateScoreTableOnWidget.AddUObject(this, &UMyUserWidget::UpdateScoreTable);
	ball->UpdatePredictIconOnWidget.AddUObject(this, &UMyUserWidget::UpdatePreAndFlag);
	
	ball->OnOffMainPanelOnWidget.AddUObject(this, &UMyUserWidget::OnOffMainPanel);
	ball->OnOffMovingPanelOnWidget.AddUObject(this, &UMyUserWidget::OnOffMovingPanel);
	ball->OnOffOBResultOnWidget.AddUObject(this, &UMyUserWidget::OnOffOBResult);
	ball->OnOffConcedeResultOnWidget.AddUObject(this, &UMyUserWidget::OnOffConcedeResult);
	ball->OnOffOnScoreResultOnWidget.AddUObject(this, &UMyUserWidget::OnOffScoreResult);
	ball->OnOffOnScoreTableOnWidget.AddUObject(this, &UMyUserWidget::OnOffScoreTable);
	ball->OnOffOnGameOverButtonOnWidget.AddUObject(this, &UMyUserWidget::OnOffGameOverButton);
}

void UMyUserWidget::SetCurrentPlayerState(AMyPlayerState* state)
{
	CurrentPlayerState = state; 
	state->GetDistanceOnWidget.AddUObject(this, &UMyUserWidget::UpdateDistance);
	state->GetParOnWidget.AddUObject(this, &UMyUserWidget::UpdatePar);
	state->GetScoreOnWidget.AddUObject(this, &UMyUserWidget::UpdateScore);
	state->GetWholeDistanceOnWidget.AddUObject(this,&UMyUserWidget::UpdateWholeDistance);
	state->GetMinimapOnWidget.AddUObject(this, &UMyUserWidget::SetMinimapImage);
}

void UMyUserWidget::UpdatePower()
{
	Power->SetPercent(CurrentBallState->GetPower());
}

void UMyUserWidget::SetPowerZero()
{
	Power->SetPercent(0);
}

void UMyUserWidget::UpdateWholeDistance()
{
	int32 dis = CurrentPlayerState->GetWholeDistance();
	FString fstr = FString::Printf(TEXT("%dm"), dis);
	WholeDistance->SetText(FText::FromString(fstr));
}

void UMyUserWidget::UpdateDistance()
{
	float dis = CurrentPlayerState->GetDistanceRemaining();
	FString fstr = FString::Printf(TEXT("%.1fm"), dis);
	DistanceRemaining->SetText(FText::FromString(fstr));
}

void UMyUserWidget::UpdatePar()
{
	int32 inum = CurrentGameMode->GetScoreTable(CurrentPlayerState->GetCurrentHoleIndex());
	FString fstr = TEXT("Par ") + FString::Printf(TEXT("%d"), inum);
	ShowPar->SetText(FText::FromString(fstr));
}

void UMyUserWidget::UpdateScore()
{
	int32 inum = CurrentPlayerState->GetAllScore();
	FString fstr;
	if (inum > 0)
	{
		fstr = FString::Printf(TEXT("+%d"), inum);
	}
	else
	{
		fstr = FString::Printf(TEXT("%d"), inum);
	}
	NowScore->SetText(FText::FromString(fstr));
}

void UMyUserWidget::SetMinimapImage()
{
	// 미니맵 이미지 세팅
	int index = CurrentPlayerState->GetCurrentHoleIndex();
	FString Path = FString::Printf(TEXT("/Game/Blueprints/Widget/MinimapImage/MnimapIndex%d.MnimapIndex%d"), index, index);
	UTexture2D* Texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *Path));
	Minimap->SetBrushFromTexture(Texture);

	// 플래그 아이콘 세팅
	int index1 = CurrentGameMode->GetFlagIconIndex(index);
	FString Path1 = FString::Printf(TEXT("/Game/Blueprints/Widget/MinimapImage/FlagIndex%d.FlagIndex%d"), index1, index1);
	UTexture2D* Texture1 = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *Path1));
	FlagIcon->SetBrushFromTexture(Texture1);
	
	FVector FlagLocation = CurrentGameMode->GetHoleCupLocation(index);

	/* 좌표 변환 */
	// 카메라 중앙 좌표
	FVector Center = CurrentGameMode->GetMinimapCenterLocation(index);
	// 미니맵 너비
	int32 Width = CurrentGameMode->GetMinimapWidth(index);

	FVector2D IconLocation;
	IconLocation.X = (FlagLocation.Y - (Center.Y - (Width / 2))) / Width * 500;
	IconLocation.Y = 500 - ((FlagLocation.X - (Center.X - (Width / 2))) / Width * 500);

	FlagIcon->SetRenderTranslation(IconLocation);
}

void UMyUserWidget::UpdateBallIcon()
{
	FVector BallLocation = CurrentBallState->GetActorLocation();

	/* 좌표 변환 */
	int index = CurrentPlayerState->GetCurrentHoleIndex();
	// 카메라 중앙 좌표
	FVector Center = CurrentGameMode->GetMinimapCenterLocation(index);
	// 미니맵 너비
	int32 Width = CurrentGameMode->GetMinimapWidth(index);

	FVector2D IconLocation;
	IconLocation.X = (BallLocation.Y - (Center.Y - (Width / 2))) / Width * 500;
	IconLocation.Y = 500 - ((BallLocation.X - (Center.X - (Width / 2))) / Width * 500);

	BallIcon->SetRenderTranslation(IconLocation);
}

void UMyUserWidget::UpdatePredictIcon()
{
	FVector PredictLocation = CurrentBallState->GetPredictLocation();

	/* 좌표 변환 */
	int index = CurrentPlayerState->GetCurrentHoleIndex();
	// 카메라 중앙 좌표
	FVector Center = CurrentGameMode->GetMinimapCenterLocation(index);
	// 미니맵 너비
	int32 Width = CurrentGameMode->GetMinimapWidth(index);

	FVector2D IconLocation;
	IconLocation.X = (PredictLocation.Y - (Center.Y - (Width / 2))) / Width * 500;
	IconLocation.Y = 500 - ((PredictLocation.X - (Center.X - (Width / 2))) / Width * 500);

	PredictIcon->SetRenderTranslation(IconLocation);
}

void UMyUserWidget::UpdateClubState()
{
	// 클럽 이름
	FText txt = (FText)StaticEnum<EGolfClub>()->GetDisplayNameTextByIndex((int32)CurrentBallState->GetClubState());
	ClubState->SetText(txt);

	// 클럽 사진
	FString Path;
	switch (CurrentBallState->GetClubState())
	{
	case EGolfClub::DRIVER:
		Path = TEXT("/Game/Blueprints/Widget/ClubImage/DRIVER.DRIVER");
		break;
	case EGolfClub::WOOD:
		Path = TEXT("/Game/Blueprints/Widget/ClubImage/WOOD.WOOD");
		break;
	case EGolfClub::IRON:
		Path = TEXT("/Game/Blueprints/Widget/ClubImage/IRON.IRON");
		break;
	case EGolfClub::WEDGE:
		Path = TEXT("/Game/Blueprints/Widget/ClubImage/WEDGE.WEDGE");
		break;
	case EGolfClub::PUTTER:
		Path = TEXT("/Game/Blueprints/Widget/ClubImage/PUTTER");
		break;
	default:
		break;
	}

	UTexture2D* Texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *Path));
	ClubImage->SetBrushFromTexture(Texture);

	// 게이지 바 아래 m 표시
	// 현재 클럽의 최대 비거리
	int32 dis = CurrentBallState->GetDrivingDis() / 100;
	FString MaxDisStr = FString::Printf(TEXT("%dm"), dis);
	FString MidDisStr = FString::Printf(TEXT("%dm"), dis/2);
	
	MaxDis->SetText(FText::FromString(MaxDisStr));
	MidDis->SetText(FText::FromString(MidDisStr));

	// 게이지 바 위에 예상 파워 아이콘 위치 구현
	// 현재 남은 거리
	int32 RemainDis = CurrentPlayerState->GetDistanceRemaining();
	FVector2D IconLocation;
	IconLocation.Y = 0;

	// 남은 거리가 더 높을경우 그냥 맨 끝에 위치
	if (dis < RemainDis)
	{
		IconLocation.X = 790;
	}
	// 더 짧을 경우 해당 게이지 바 위에 위치
	else
	{
		IconLocation.X = (RemainDis * 790 / dis);
	}
	PredictPower->SetRenderTranslation(IconLocation);
}

void UMyUserWidget::UpdateMovingInformation()
{
	float Moving_Dis = CurrentBallState->GetMovingDis();
	FString fstr0 = FString::Printf(TEXT("%.2fm"), Moving_Dis);
	MovingDis->SetText(FText::FromString(fstr0));

	float Remaining_dis = CurrentPlayerState->GetDistanceRemaining();
	FString fstr1 = FString::Printf(TEXT("%.2fm"), Remaining_dis);
	RemainingDis->SetText(FText::FromString(fstr1));
}

void UMyUserWidget::UpdateGeoState()
{
	FText txt = StaticEnum<EGeographyState>()->GetDisplayNameTextByIndex((int32)CurrentBallState->GetGeographyState());
	GeoState_0->SetText(txt);
	GeoState_1->SetText(txt);
	GeoState_2->SetText(txt);

	FString fstr;
	switch (CurrentBallState->GetGeographyState())
	{
	case EGeographyState::ROUGH:
		fstr = TEXT("  85% ~ 95%");
		break;
	case EGeographyState::BUNKER:
		fstr = TEXT("  75% ~ 85%");
		break;
	default:
		break;
	}
	RandRange->SetText(FText::FromString(fstr));
}

void UMyUserWidget::UpdateShotNumberth()
{
	int32 inum = CurrentPlayerState->GetNumberth() + 1;
	
	FString fstr = FString::Printf(TEXT("%d"), inum);

	if (inum%10 == 1)
	{
		fstr += TEXT("st");
	}
	else if (inum%10 == 2)
	{
		fstr += TEXT("nd");
	}
	else if (inum%10 == 3)
	{
		fstr += TEXT("rd");
	}
	else
	{
		fstr += TEXT("th");
	}
	fstr+= TEXT(" Shot");

	ShotNumberth->SetText(FText::FromString(fstr));
}

void UMyUserWidget::UpdateScoreResult()
{
	int32 Numberth = CurrentPlayerState->GetNumberth();
	int32 ParNum = CurrentGameMode->GetScoreTable(CurrentPlayerState->GetCurrentHoleIndex());
	int32 inum = Numberth - ParNum;

	FString fstr;

	if (Numberth == 1)
	{
		fstr = TEXT("HOLE IN ONE");
	}
	else if (inum == ParNum)
	{
		fstr = TEXT("DOUBLE PAR");
	}
	else
	{
		switch (inum)
		{
		case -3:
			fstr = TEXT("ALBATROSS");
			break;
		case -2:
			fstr = TEXT("EAGLE");
			break;
		case -1:
			fstr = TEXT("BIRDIE");
			break;
		case 0:
			fstr = TEXT("PAR");
			break;
		case 1:
			fstr = TEXT("BOGEY");
			break;
		case 2:
			fstr = TEXT("DOUBLE BOGEY");
			break;
		case 3:
			fstr = TEXT("TRIPLE BOGEY");
			break;
		case 4:
			fstr = TEXT("QUARDRUPLE BOGEY");
			break;
		default:
			break;
		}
	}
	Score_Result->SetText(FText::FromString(fstr));
}

void UMyUserWidget::UpdateScoreTable()
{
	int index = CurrentPlayerState->GetCurrentHoleIndex();

	// PAR
	int32 ParNum = CurrentGameMode->GetScoreTable(CurrentPlayerState->GetCurrentHoleIndex());
	FString fstr0 = FString::Printf(TEXT("%d"), ParNum);
	
	// TOTAL PAR
	iTotalPar += ParNum;
	FString fstr1 = FString::Printf(TEXT("%d"), iTotalPar);

	// SCORE
	int32 Numberth = CurrentPlayerState->GetNumberth();
	int32 inum = Numberth - ParNum;
	FString fstr2 = FString::Printf(TEXT("%d"), inum);

	// TOTAL SCORE
	iTotalScore += inum;
	FString fstr3 = FString::Printf(TEXT("%d"), iTotalScore);

	// HOLE DISTANCE
	int32 dis = CurrentPlayerState->GetWholeDistance();
	FString fstr4 = FString::Printf(TEXT("%d"), dis);

	// 우선 테스트로 3개까지만, 이부분 함수로 묶어도 될듯
	switch (index)
	{
	case 0:
		ScorePar0->SetText(FText::FromString(fstr0));
		TotalPar->SetText(FText::FromString(fstr1));
		Score0->SetText(FText::FromString(fstr2));
		TotalScore->SetText(FText::FromString(fstr3));
		ScoreDis0->SetText(FText::FromString(fstr4));
		break;
	case 1:
		ScorePar1->SetText(FText::FromString(fstr0));
		TotalPar->SetText(FText::FromString(fstr1));
		Score1->SetText(FText::FromString(fstr2));
		TotalScore->SetText(FText::FromString(fstr3));
		ScoreDis1->SetText(FText::FromString(fstr4));
		break;
	case 2:
		ScorePar2->SetText(FText::FromString(fstr0));
		TotalPar->SetText(FText::FromString(fstr1));
		Score2->SetText(FText::FromString(fstr2));
		TotalScore->SetText(FText::FromString(fstr3));
		ScoreDis2->SetText(FText::FromString(fstr4));
		break;
	default:
		break;
	}
}

void UMyUserWidget::UpdatePreAndFlag()
{
	// 도착 예상 지점과 플래그 사이의 거리
	// 흰색 점선 거리 출력
	FVector PredictLocation = CurrentBallState->GetPredictLocation();
	FVector2D PredictLocation2D(PredictLocation.X, PredictLocation.Y);

	int index = CurrentPlayerState->GetCurrentHoleIndex();
	FVector FlagLocation = CurrentGameMode->GetHoleCupLocation(index);
	FVector2D FlagLocation2D(FlagLocation.X, FlagLocation.Y);

	float dis = FVector2D::Distance(PredictLocation2D, FlagLocation2D) / 100;

	FString fstr = FString::Printf(TEXT("%.1fm"), dis);
	PreAndFlag->SetText(FText::FromString(fstr));

	//좌표변환
	FVector Center = CurrentGameMode->GetMinimapCenterLocation(index);
	int32 Width = CurrentGameMode->GetMinimapWidth(index);

	FVector2D IconLocation1;
	IconLocation1.X = (FlagLocation.Y - (Center.Y - (Width / 2))) / Width * 500;
	IconLocation1.Y = 500 - ((FlagLocation.X - (Center.X - (Width / 2))) / Width * 500);

	FVector2D IconLocation2;
	IconLocation2.X = (PredictLocation.Y - (Center.Y - (Width / 2))) / Width * 500;
	IconLocation2.Y = 500 - ((PredictLocation.X - (Center.X - (Width / 2))) / Width * 500);

	FVector2D IconLocation3;
	IconLocation3.X = (IconLocation1.X + IconLocation2.X) / 2;
	IconLocation3.Y = (IconLocation1.Y + IconLocation2.Y) / 2;

	PreAndFlag->SetRenderTranslation(IconLocation3);
}

void UMyUserWidget::ClickGameOver()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}

void UMyUserWidget::OnOffMainPanel(bool on)
{
	if (on)
	{
		MainPanel->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		MainPanel->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UMyUserWidget::OnOffMovingPanel(bool on)
{
	if (on)
	{
		MovingPanel->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		MovingPanel->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UMyUserWidget::OnOffOBResult(bool on)
{
	if (on)
	{
		OB_Result->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		OB_Result->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UMyUserWidget::OnOffConcedeResult(bool on)
{
	if (on)
	{
		Concede_Result->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Concede_Result->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UMyUserWidget::OnOffScoreResult(bool on)
{
	if (on)
	{
		Score_Result->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Score_Result->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UMyUserWidget::OnOffScoreTable(bool on)
{
	if (on)
	{
		ScorePanel->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		ScorePanel->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UMyUserWidget::OnOffGameOverButton(bool on)
{
	if (on)
	{
		GameOver->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		GameOver->SetVisibility(ESlateVisibility::Hidden);
	}
}
