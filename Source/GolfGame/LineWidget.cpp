// Fill out your copyright notice in the Description page of Project Settings.


#include "LineWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"

#include "GolfGameGameModeBase.h"
#include "MyPlayerState.h"
#include "Ball.h"

int32 ULineWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	//Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
	FPaintContext Context(AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	// 의문점 : MyUserWidget처럼 PS에서 set이 되지 않는다.
	// 대신에 이렇게 직접 Widget의 Get을 사용하여 활용할 수 있다.
	if (Cast<ABall>(GetOwningPlayerPawn()) != NULL &&
		Cast<AMyPlayerState>(GetOwningPlayerState()) != NULL &&
		Cast<AGolfGameGameModeBase>(GetWorld()->GetAuthGameMode()) != NULL)
	{
		FVector BallLocation = Cast<ABall>(GetOwningPlayerPawn())->GetActorLocation();
		FVector2D BallLoc;
		BallLoc.X = (BallLocation.Y + 6500) / 25000 * 500 ;

		BallLoc.Y = 500 - ((BallLocation.X + 1500) / 25000 * 500);

		FVector PredictLocation = Cast<ABall>(GetOwningPlayerPawn())->GetPredictLocation();
		FVector2D PreLoc;
		PreLoc.X = (PredictLocation.Y + 6500) / 25000 * 500 ;

		PreLoc.Y = 500 - ((PredictLocation.X + 1500) / 25000 * 500);

		int index = Cast<AMyPlayerState>(GetOwningPlayerState())->GetCurrentHoleIndex();
		FVector FlagLocation = Cast<AGolfGameGameModeBase>(GetWorld()->GetAuthGameMode())->GetHoleCupLocation(index);
		FVector2D FlagLoc;
		FlagLoc.X = (FlagLocation.Y + 6500) / 25000 * 500;

		FlagLoc.Y = 500 - ((FlagLocation.X + 1500) / 25000 * 500);

		UWidgetBlueprintLibrary::DrawLine(Context, BallLoc, PreLoc, FLinearColor::Yellow, false, 4);

		//// 시작과 도착을 잘게 잘라서 적절하게 그려주면 점선됨
		UWidgetBlueprintLibrary::DrawLine(Context, PreLoc, FlagLoc, FLinearColor::White, false, 4);
	}
	return LayerId;
}