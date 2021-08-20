// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"

#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "MyPlayerState.h"
#include "GolfGameGameModeBase.h"
#include "TestSpline.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"

#include "MyPlayerController.h"

// Sets default values
ABall::ABall()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create BallMesh
	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BALLMESH"));
	BallMesh->SetSimulatePhysics(true);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BALL(TEXT("/Game/Meterial/Shape_Sphere.Shape_Sphere"));
	if (SM_BALL.Succeeded())
	{
		BallMesh->SetStaticMesh(SM_BALL.Object);
		BallMesh->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
	}
	RootComponent = BallMesh;

	BallMesh->SetCollisionObjectType(ECC_Pawn);
	BallMesh->SetCollisionProfileName("SetLineTraceChannel");
	BallMesh->SetReceivesDecals(false);

	// set temp angulardamping
	BallMesh->SetAngularDamping(30);

	// Create CameraSpringArm;
	BallCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CAMERASPRINGARM"));
	BallCameraSpringArm->SetupAttachment(RootComponent);
	BallCameraSpringArm->TargetArmLength = 700.0F;
	BallCameraSpringArm->bEnableCameraLag = true;
	BallCameraSpringArm->bUsePawnControlRotation = true;
	BallCameraSpringArm->CameraLagSpeed = .0F;
	
	BallCameraSpringArm->SocketOffset = FVector(0.0F, 0.0F, 200.0F);
	BallCameraSpringArm->TargetOffset = FVector(0.0F, 0.0F, 50.0F);
	
	// Create Camera
	BallCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("BALLCAMERA"));
	BallCamera->SetupAttachment(BallCameraSpringArm, USpringArmComponent::SocketName);
	


}

// Called when the game starts or when spawned
void ABall::BeginPlay()
{
	Super::BeginPlay();
	
	BallMesh->OnComponentBeginOverlap.AddDynamic(this, &ABall::OnOverlapBegin);
	BallMesh->OnComponentEndOverlap.AddDynamic(this, &ABall::OnOverlapEnd);
	
	// Set Default Value
	bCheckHoleCup = false;
	bCheckConcede = false;
	bCheckOnce = true;
	bCheckOB = false;
	bWaitTimer = true;

	// Get Player State
	BallPlayerState = Cast<AMyPlayerState>(GetPlayerState());
	BallPlayerState->GetParOnWidget.Broadcast();
	BallPlayerState->GetScoreOnWidget.Broadcast();
	BallPlayerState->GetWholeDistanceOnWidget.Broadcast();

	// Set Default CameraLagSpeed
	BallCameraSpringArm->CameraLagSpeed = .0F;
	
	// 미니맵 세팅
	OnOffMainPanelOnWidget.Broadcast(true);
	OnOffMovingPanelOnWidget.Broadcast(false);

	// 값 초기화
	CurrentState = EBallState::STOP;
	GeographyState = EGeographyState::FAIRWAY;
	UpdateGeoStateOnWidget.Broadcast();
	ClubState = EGolfClub::DRIVER;
	ChangeClub();

	// 볼 아이콘 업데이트 델리게이트
	UpdateBallIconOnWidget.Broadcast();
	// 남은거리 갱신
	BallPlayerState->SetDistanceRemaining();
	// 볼 초기 위치
	SetBallLoc();
}

// Called every frame
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// TEST
	//UseLineTrace();

	switch (CurrentState)
	{
	case EBallState::STOP:
		break;
	case EBallState::READY:
		break;
	case EBallState::CHARGING:
		ChargingPower();
		break;
	case EBallState::MOVING:
		CheckBallisMoiving();
		break;
	case EBallState::CHECK:
		CheckBallLocation();
		break;
	default:
		break;
	}
}

// Called to bind functionality to input
void ABall::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Hit : Press SpaceBar
	PlayerInputComponent->BindAction("BallHit", IE_Pressed, this, &ABall::OnPressBallHit);
	PlayerInputComponent->BindAction("BallHit", IE_Released, this, &ABall::OnRealseBallHit);
	// ChangeClub : Press C
	PlayerInputComponent->BindAction("ChangeClub", IE_Pressed, this, &ABall::OnPressChangeClub);

	// Move Direction : Press A or D
	PlayerInputComponent->BindAxis("MoveDirection", this, &ABall::MoveDirection);

	

	
}

void ABall::OnPressBallHit()
{
	// 누르는 순간에 공의 속도가 0일때 추가로 체크해줘야할듯
	if (CurrentState == EBallState::STOP)
	{
		CurrentState = EBallState::READY;
	}

	if (CurrentState == EBallState::CHARGING)
	{
		// ball hit : power = PowerPercent
		BallCameraSpringArm->CameraLagSpeed = 3.0F;
		BallPlayerState->SetFormerLocation(this->GetActorLocation());
		BallPlayerState->PlusScore();
		
		// 공 치는 부분 -> 함수하나 만들고 클럽 종류에따라 각도와 힘 계산
		/*
		FVector fvtemp = BallCamera->GetForwardVector()*FVector(1.0f, 1.0f, 1.0f);
		fvtemp.Z = 1;
		fvtemp = fvtemp * 100 * 10;
		FVector av = fvtemp.GetSafeNormal();
		FVector bv = av.ToOrientationQuat().GetRightVector();
		BallMesh->AddAngularImpulseInDegrees(bv * 10000, NAME_None, true);
		BallMesh->AddImpulse(fvtemp, NAME_None, true);
		*/

		// 차징된 파워를 가지고 여기에서 위의 파워식을 계산해야함 !

		float fPercent = PowerPercent / 100;

		// 지형에 따라 추가 감소 설정
		switch (GeographyState)
		{
		case EGeographyState::ROUGH:
			// 85% ~ 95%
			fPercent *= 0.9;
			break;
		case EGeographyState::FAIRWAY:
			break;
		case EGeographyState::GREEN:
			break;
		case EGeographyState::BUNKER:
			//55%  ~ 65% , 75% ~ 85%
			//안쪽과 바깥쪽의 퍼센트가 다름
			fPercent *= 0.6;
			break;
		default:
			break;
		}

		// PUTTER 일때는 단순 Addimpulse
		if (ClubState == EGolfClub::PUTTER)
		{
			FVector outVelocity = BallCamera->GetForwardVector() * FVector(1, 1, 1) * 3000 * fPercent;
			BallMesh->AddImpulse(outVelocity, NAME_None, true);
		}
		// 그외에는 SuggestProjectileVelocity_CustomArc 로 계산
		else
		{
			FVector startLoc = this->GetActorLocation();// 발사 지점
			FVector targetLoc = this->GetActorLocation() + (BallCamera->GetForwardVector() * (DrivingDis * fPercent));// 타겟 지점.
			FVector outVelocity = FVector::ZeroVector;// 결과 Velocity
			if (UGameplayStatics::SuggestProjectileVelocity_CustomArc(this, outVelocity, startLoc, targetLoc, GetWorld()->GetGravityZ(), ArcValue))
			{
				/* 예상 포물선 DRAW LINE
				FPredictProjectilePathParams predictParams(-1, startLoc, outVelocity, 5.0f);   // 20: tracing 보여질 프로젝타일 크기, 15: 시물레이션되는 Max 시간(초)
				predictParams.DrawDebugTime = 5.0f;     //디버그 라인 보여지는 시간 (초)
				predictParams.DrawDebugType = EDrawDebugTrace::Type::ForDuration;  // DrawDebugTime 을 지정하면 EDrawDebugTrace::Type::ForDuration 필요.
				predictParams.OverrideGravityZ = 0;

				FPredictProjectilePathResult result;
				UGameplayStatics::PredictProjectilePath(this, predictParams, result);
				*/
			}
			BallMesh->AddImpulse(outVelocity, NAME_None, true);
		}

		OnOffMainPanelOnWidget.Broadcast(false);
		OnOffMovingPanelOnWidget.Broadcast(true);
		UpdateShotNumberthOnWidget.Broadcast();

		bCheckOnce = false;
		CurrentState = EBallState::MOVING;
	}
}

void ABall::OnRealseBallHit()
{
	if (CurrentState == EBallState::READY)
	{
		PowerPercent = 0.0f;
		PowerIncrease = true;
		CurrentState = EBallState::CHARGING;
	}
}

void ABall::OnPressChangeClub()
{
	// 볼이 멈춰있으면서 && 그린이 아닐때
	if (CurrentState == EBallState::STOP && GeographyState != EGeographyState::GREEN)
	{
		int32 index = (int32)ClubState + 1;

		// 티샷일때
		// 드라이버부터 웨지까지
		// 이때 지형속성을 TEE라고 보여줘야할까 ?
		if (BallPlayerState->GetNumberth() == 0)
		{
			if (index == (int32)StaticEnum<EGolfClub>()->NumEnums() - 2)
			{
				index = 0;
			}
		}
		// 우드부터 웨지까지
		else
		{
			if (index == (int32)StaticEnum<EGolfClub>()->NumEnums() - 2)
			{
				index = 1;
			}
		}

		ClubState = (EGolfClub)StaticEnum<EGolfClub>()->GetValueByIndex(index);

		ChangeClub();
	}
}

void ABall::MoveDirection(float AxisValue)
{
	if (AxisValue != 0 && CurrentState == EBallState::STOP)
	{
		AddControllerYawInput(AxisValue);

		PredictLocation = this->GetActorLocation() + BallCamera->GetForwardVector() * DrivingDis;
		UpdatePredictIconOnWidget.Broadcast();
	}
}

void ABall::CheckBallisMoiving()
{
	// 남은거리 갱신
	BallPlayerState->SetDistanceRemaining();
	// 비거리 계산
	SetMovingDis();
	// 지형속성 체크
	UseLineTrace();
	
	if (BallMesh->GetComponentVelocity().Size() > 0.1f)
	{
		bCheckOnce = true;
		bIsMoving = true;
	}
	else
	{
		bIsMoving = false;
		if (bCheckOnce && !bIsMoving)
		{
			CurrentState = EBallState::CHECK;
		}
	}
}

void ABall::UseLineTrace()
{
	// 홀컵
	if (bCheckHoleCup)
	{
		GeographyState = EGeographyState::HOLECUP;

	}
	// 컨시드
	else if (bCheckConcede)
	{
		GeographyState = EGeographyState::CONCEDE;

	}
	// OB 
	else if (bCheckOB) {
		GeographyState = EGeographyState::OB;
	}
	// 그 외에 지형속성
	else
	{
		FHitResult OutHit;
		FVector Startpoint = this->GetActorLocation() + FVector(0.0f, 0.0f, 1000.0f);
		FVector Endpoint = this->GetActorLocation() * FVector(1, 1, 0) + FVector(0.0f, 0.0f, -3000.0f);
		FCollisionQueryParams CollisionParams;
		CollisionParams.bReturnPhysicalMaterial = true;
		CollisionParams.bTraceComplex = false;
		
		//DrawDebugLine(GetWorld(), Startpoint, Endpoint, FColor(255, 0, 0), false, -1, 0, 12.333);

		//ECC_GameTraceChannel1 -> Custom trace channel
		bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, Startpoint, Endpoint, ECC_GameTraceChannel1, CollisionParams);
		if (isHit)
		{
			// 랜드스케이프에 칠해진 surface physics 가져오는 방법
			EPhysicalSurface epstemp = UGameplayStatics::GetSurfaceType(OutHit);

			switch (epstemp)
			{
			case SurfaceType1:
				GeographyState = EGeographyState::GREEN;
				break;
			case SurfaceType2:
				GeographyState = EGeographyState::FAIRWAY;
				break;
			case SurfaceType3:
				GeographyState = EGeographyState::ROUGH;
				break;
			case SurfaceType4:
				GeographyState = EGeographyState::BUNKER;
				break;
			default:
				// 데칼의 메테리얼 이름으로 확인
				FString DecalMat = Cast<ATestSpline>(OutHit.GetActor())->GetMaterialName();
				if (DecalMat == "Decal_FAIRWAY")
				{
					GeographyState = EGeographyState::FAIRWAY;
				}
				else if (DecalMat == "Decal_BUNKER")
				{
					GeographyState = EGeographyState::BUNKER;
				}
				else if (DecalMat == "Decal_GREEN")
				{
					GeographyState = EGeographyState::GREEN;
				}
				break;
			}
		}
	}

	UpdateGeoStateOnWidget.Broadcast();
}

void ABall::MoveNextHole()
{
	// 기본 설정
	bCheckHoleCup = false;
	bCheckConcede = false;
	bCheckOB = false;

	// 다음홀이 있을때
	if (BallPlayerState->NextHole())
	{
		// 카메라가 따라가는 현상을 없애기 위해 속도 무한대로 설정
		BallCameraSpringArm->CameraLagSpeed = 0.0f;
		SetBallLoc();

		// 추가적으로 카메라의 방향을 정해줘야함
		// 현재는 마지막에 쳤던 방향을 보고있음


		UseLineTrace();
		ClubState = EGolfClub::DRIVER;
		ChangeClub();

		// 여기에서 다음홀을 준비하기 위한 변수들 디폴트값으로 세팅

	}
	else
	{
		// 스코어표 출력
		
		// 그후
		// 1. 게임 자동종료
		// 2. 게임 종료버튼 and 처음부터 시작하기 버튼

	}
}

void ABall::ChargingPower()
{
	//PrintWithFloat("", PowerPercent);
	if (PowerIncrease)
	{
		PowerPercent += 1.f;

		if(PowerPercent>=100.0f)
		{
			PowerIncrease = false;
		}
	}
	else
	{
		PowerPercent -= 1.f;

		if (PowerPercent <= 0.0f)
		{
			CurrentState = EBallState::STOP;
		}
	}
	GetPowerGaugeOnWidget.Broadcast();
}

void ABall::CheckBallLocation()
{
	FTimerHandle handle;
	
	if (!bIsMoving && bWaitTimer) {
		bWaitTimer = false;

		// 홀인
		if (bCheckHoleCup)
		{
			UpdateScoreResultOnWidget.Broadcast();
			
			// 결과 위젯
			FTimerHandle handle2;
			GetWorld()->GetTimerManager().SetTimer(handle2, [this]() {
				OnOffOnScoreResultOnWidget.Broadcast(true);
				OnOffMovingPanelOnWidget.Broadcast(false);
			}, 1, false);
			
			GetWorld()->GetTimerManager().SetTimer(handle, [this]() {
				MoveNextHole();
				UseInTimer();
			}, 3, false);
		}
		// 더블파 체크
		else if (BallPlayerState->GetNumberth() == BallPlayerState->GetDoublePar())
		{
			BallPlayerState->PlusScore();
			UpdateScoreResultOnWidget.Broadcast();

			// 결과 위젯
			FTimerHandle handle2;
			GetWorld()->GetTimerManager().SetTimer(handle2, [this]() {
				OnOffOnScoreResultOnWidget.Broadcast(true);
				OnOffMovingPanelOnWidget.Broadcast(false);
			}, 1, false);

			GetWorld()->GetTimerManager().SetTimer(handle, [this]() {
				MoveNextHole();
				UseInTimer();
			}, 3, false);
		}
		// 컨시드
		else if (bCheckConcede)
		{
			BallPlayerState->PlusScore();
			UpdateScoreResultOnWidget.Broadcast();

			// 결과 위젯
			FTimerHandle handle2;
			GetWorld()->GetTimerManager().SetTimer(handle2, [this]() {
				OnOffConcedeResultOnWidget.Broadcast(true);
				OnOffOnScoreResultOnWidget.Broadcast(true);
				OnOffMovingPanelOnWidget.Broadcast(false);
			}, 1, false);

			GetWorld()->GetTimerManager().SetTimer(handle, [this]() {
				MoveNextHole();
				UseInTimer();
			}, 3, false);
		}
		//OB일때
		else if (GeographyState == EGeographyState::OB)
		{
			// OB 상태 해제
			bCheckOB = false;
			//벌타 추가
			BallPlayerState->PlusScore();

			// if 더블파일때
			if (BallPlayerState->GetNumberth() == BallPlayerState->GetDoublePar())
			{
				BallPlayerState->PlusScore();
				UpdateScoreResultOnWidget.Broadcast();

				// 결과 위젯
				FTimerHandle handle2;
				GetWorld()->GetTimerManager().SetTimer(handle2, [this]() {
					OnOffOnScoreResultOnWidget.Broadcast(true);
					OnOffMovingPanelOnWidget.Broadcast(false);
				}, 1, false);

				GetWorld()->GetTimerManager().SetTimer(handle, [this]() {
					MoveNextHole();
					UseInTimer();
				}, 3, false);
			}
			// else 더블파 아닐때
			else
			{
				FTimerHandle handle2;
				GetWorld()->GetTimerManager().SetTimer(handle2, [this]() {
					OnOffOBResultOnWidget.Broadcast(true);
					OnOffMovingPanelOnWidget.Broadcast(false);
				}, 1, false);

				//이전지점으로 이동
				GetWorld()->GetTimerManager().SetTimer(handle, [this]() {
					BallCameraSpringArm->CameraLagSpeed = 0.0f;
					this->SetActorLocation(BallPlayerState->GetFormerLocation());
					UseLineTrace();
					ChangeClubFromDis();
					ChangeClub();
					UseInTimer();
				}, 3, false);
			}
		}
		// 그린일때
		else if (GeographyState == EGeographyState::GREEN) {
			GetWorld()->GetTimerManager().SetTimer(handle, [this]() {
				ClubState = EGolfClub::PUTTER;
				ChangeClub();
				UseInTimer();
			}, 1, false);
			
			// 그린 전용 미니맵? 생각해보기
		}
		// 그린이 아닐때
		else
		{
			// 남은거리에 따라 클럽 지정해줌.
			GetWorld()->GetTimerManager().SetTimer(handle, [this]() {
				ChangeClubFromDis();
				ChangeClub();
				UseInTimer();
			}, 1, false);
		}
	}
}

void ABall::ChangeClub()
{
	// 각도 수정 필요 0 ~ 1 , 값이 높을수록 공의 각도 낮아짐
	switch (ClubState)
	{
	case EGolfClub::DRIVER:
		DrivingDis = 15000;
		ArcValue = 0.6;
		break;
	case EGolfClub::WOOD:
		DrivingDis = 13000;
		ArcValue = 0.6;
		break;
	case EGolfClub::IRON:
		DrivingDis = 10000;
		ArcValue = 0.6;
		break;
	case EGolfClub::WEDGE:
		DrivingDis = 7000;
		ArcValue = 0.4;
		break;
	case EGolfClub::PUTTER:
		DrivingDis = 6000;
		ArcValue = 1;
		break;
	default:
		break;
	}
	PredictLocation = this->GetActorLocation() + BallCamera->GetForwardVector() * DrivingDis;
	UpdatePredictIconOnWidget.Broadcast();
	UpdateClubStateOnWidget.Broadcast();
}

void ABall::ChangeClubFromDis()
{
	//남은 거리에 따라
	float fDis = BallPlayerState->GetDistanceRemaining();

	if (fDis > 100)
	{
		ClubState = EGolfClub::WOOD;
	}
	else if (fDis > 70)
	{
		ClubState = EGolfClub::IRON;
	}
	else
	{
		ClubState = EGolfClub::WEDGE;
	}
}

void ABall::SetMovingDis()
{
	//공을 쳤을때 움직인 거리(위젯에서 비거리로 보여줄 변수 업데이트)
	MovingDis = FVector::Dist(BallPlayerState->GetFormerLocation(),this->GetActorLocation()) / 100;

	// 위젯에서 비거리를 업데이트 해주는 델리게이트
	UpdateMovingInfoOnWidget.Broadcast();
}

void ABall::UseInTimer()
{
	//타이머 람다 함수에 사용되는 함수 모음
	UpdateBallIconOnWidget.Broadcast();
	SetPowerZeroOnWidget.Broadcast();
	UpdateShotNumberthOnWidget.Broadcast();

	OnOffOBResultOnWidget.Broadcast(false);
	OnOffConcedeResultOnWidget.Broadcast(false);
	OnOffOnScoreResultOnWidget.Broadcast(false);

	OnOffMainPanelOnWidget.Broadcast(true);
	OnOffMovingPanelOnWidget.Broadcast(false);

	CurrentState = EBallState::STOP;
	bWaitTimer = true;
}

void ABall::SetBallLoc()
{
	FHitResult OutHit;
	FVector Startpoint = Cast<AGolfGameGameModeBase>(GetWorld()->GetAuthGameMode())->GetSpawnLocation(BallPlayerState->GetCurrentHoleIndex()) + FVector(0, 0, 10000);
	FVector Endpoint = Startpoint * FVector(1, 1, 0) + FVector(0, 0, -1000);
	FCollisionQueryParams CollisionParams;

	bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, Startpoint, Endpoint, ECC_GameTraceChannel1, CollisionParams);
	if (isHit)
	{
		this->SetActorLocation(OutHit.Location);
	}
}

void ABall::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Print("Overlap");
	if (OtherComp->GetArchetype()->GetName() == TEXT("Default__SplineMeshComponent"))
	{
	//	Print("Check OB");
		bCheckOB = !bCheckOB;
	}
	else if (OtherComp->GetName() == TEXT("CONCEDE"))
	{
		bCheckConcede = true;
	}
	else if (OtherComp->GetName() == TEXT("HOLECUP"))
	{
		bCheckHoleCup = true;
	}
}

void ABall::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp->GetName() == TEXT("CONCEDE"))
	{
		bCheckConcede = false;
	}
	else if (OtherComp->GetName() == TEXT("HOLECUP"))
	{
		bCheckHoleCup = false;
	}
}