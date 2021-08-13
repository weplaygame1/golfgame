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

// Sets default values
ABall::ABall()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create BallMesh
	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BALLMESH"));
	BallMesh->SetSimulatePhysics(true);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BALL(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if (SM_BALL.Succeeded())
	{
		BallMesh->SetStaticMesh(SM_BALL.Object);
		BallMesh->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
	}
	RootComponent = BallMesh;

	BallMesh->SetCollisionObjectType(ECC_Pawn);
	BallMesh->SetCollisionProfileName("SetLineTraceChannel");

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
	bIsChargingHit = false;

	bCheckOnce = true;
	JumpPower = 0;

	// Get Player State
	BallPlayerState = Cast<AMyPlayerState>(GetPlayerState());
	BallPlayerState->GetParOnWidget.Broadcast();
	BallPlayerState->GetScoreOnWidget.Broadcast();
	BallPlayerState->GetWholeDistanceOnWidget.Broadcast();

	// Set Default CameraLagSpeed
	BallCameraSpringArm->CameraLagSpeed = 3.0F;
	
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
}

// Called every frame
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// TEST
	UseLineTrace();

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
		fvtemp = BallCamera->GetForwardVector()*FVector(1.0f, 1.0f, 1.0f);
		fvtemp.Z = 1;
		fvtemp = fvtemp * 100 * 10;
		av = fvtemp.GetSafeNormal();
		bv = av.ToOrientationQuat().GetRightVector();
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

		// if 퍼터일때는 그냥 addimpulse

		// else 아래 그대로
		FVector startLoc = this->GetActorLocation();// 발사 지점
		FVector targetLoc = this->GetActorLocation() + (BallCamera->GetForwardVector() * (DrivingDis * fPercent));// 타겟 지점.
		outVelocity = FVector::ZeroVector;// 결과 Velocity
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
		
		// 여기에서 지형속성에 따른 패널티 부과 (러프와 벙커에서만?)
		BallMesh->AddImpulse(outVelocity, NAME_None, true);

		OnOffMainPanelOnWidget.Broadcast(false);
		OnOffMovingPanelOnWidget.Broadcast(true);

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
		if (BallPlayerState->GetNowHoleScore() == 0)
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
	//UseLineTrace();
	
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
	//Print("UseLineTrace");

	FVector Startpoint = this->GetActorLocation() + FVector(0.0f, 0.0f, 100.0f);
	FVector Endpoint = Startpoint * FVector(1.0f, 1.0f, 0.0f) + FVector(0.0f, 0.0f, -100.0f);
	FCollisionQueryParams CollisionParams;
	CollisionParams.bReturnPhysicalMaterial = true;
	CollisionParams.bTraceComplex = false;

	//ECC_GameTraceChannel1 -> Custom trace channel
	/*
	bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, Startpoint, Endpoint, ECC_GameTraceChannel1, CollisionParams);
	if (isHit)
	{
		fsttt=OutHit.GetActor()->GetName();
		FName GeoState = OutHit.GetComponent()->GetMaterial(0)->GetFName();

		//디버그용, 확인하고 지우기
		//itestest = StaticEnum<EGeographyState>()->GetValueByName(GeoState);
		
		GeographyState = (EGeographyState)StaticEnum<EGeographyState>()->GetValueByName(GeoState);
		UpdateGeoStateOnWidget.Broadcast();

		// 각 지형 속성에 맞는 설정 ex) 파워감소 등등
		// ex) PlayerState.Surface = 0.1 -> 10% 파워 감소

		// 부분은 계속 돌릴필요없이 공을 치기전에 계산해주면 될거같은데?
		switch (GeographyState)
		{
		case EGeographyState::ROUGH:
			//Print("ROUGH");

			break;
		case EGeographyState::FAIRWAY:
			//Print("FAIRWAY");

			break;
		case EGeographyState::GREEN:
			//Print("GREEN");

			break;
		case EGeographyState::BUNKER:
			//Print("BUNKER");

			break;
		default:
			//Print("DEFAULT");

			//GetValueByName(FName name);
			//만약 enum에 없는 이름이면 -1을 리턴시킴 -> 이걸 사용하는 방법도 가능



			break;
		}

	}
	else
	{
		// OB , HAZARD 일때?
		// 이 녀석들은 공을 옮겨야함
		// 추가로 타수를 줄여줌
		//Print("OUT");

		// 우선은 OB, HAZARD 공통적으로 그 전 위치로 보내줌
	}
	*/

	DrawDebugLine(GetWorld(), Startpoint, Endpoint, FColor(255, 0, 0), false, -1, 0, 12.333);

	// 랜드스케이프에 칠해진 surface physics 가져오는 방법
	// EPhysicalSurface epstemp = UGameplayStatics::GetSurfaceType(OutHit);
	bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, Startpoint, Endpoint, ECC_GameTraceChannel1, CollisionParams);
	if (isHit)
	{
		EPhysicalSurface epstemp = UGameplayStatics::GetSurfaceType(OutHit);

		switch (epstemp)
		{
		case SurfaceType1:
			Print("GREEN");
			break;
		case SurfaceType2:
			Print("FAIR");

			break;
		case SurfaceType3:
			Print("ROUGH");

			break;
		case SurfaceType4:
			Print("BUNKER");

			break;
		default:
			// 여기에서 경계부분이나 OB를 체크하면 될듯?

			Print("Out");

			fsttt = Cast<ATestSpline>(OutHit.GetActor())->GetMaterialName();
			// 이거 디폴트 값 설정을 잘 생각해서 설계해야할듯
			

			// 마지지막 볼 로케이션 함수에서
			// fsttt (예정) 이 None이 아닐때 && OB or HAZARD면 처리
			// 그외 나머지는 지금 라인트레이스 그대로 수행
			

			break;
		}
	}
}

void ABall::MoveNextHole()
{
	// 기본 설정
	bCheckHoleCup = false;
	bCheckConcede = false;

	if (BallPlayerState->NextHole())
	{
		// 카메라가 따라가는 현상을 없애기 위해 속도 무한대로 설정
		BallCameraSpringArm->CameraLagSpeed = 0.0f;
		this->SetActorLocation(BallPlayerState->GetFormerLocation());

		// 추가적으로 카메라의 방향을 정해줘야함
		// 현재는 마지막에 쳤던 방향을 보고있음


		GeographyState = EGeographyState::FAIRWAY;
		UpdateGeoStateOnWidget.Broadcast();

		// 여기에서 다음홀을 준비하기 위한 변수들 디폴트값으로 세팅

	}
	else
	{
		// 게임끝났을때 temp
		Print("End");
		
	}
}

void ABall::ChargingPower()
{
	//PrintWithFloat("", PowerPercent);
	if (PowerIncrease)
	{
		PowerPercent += 0.5f;

		if(PowerPercent>=100.0f)
		{
			PowerIncrease = false;
		}
	}
	else
	{
		PowerPercent -= 0.5f;

		if (PowerPercent <= 0.0f)
		{
			CurrentState = EBallState::STOP;
		}
	}
	GetPowerGaugeOnWidget.Broadcast();
}

void ABall::CheckBallLocation()
{

	//더블파 규칙 확인하고 수정해야함 !!!!!

	if (!bIsMoving) {
		// 홀 아웃인지 체크
		if (bCheckConcede || bCheckHoleCup)
		{
			// 홀인
			if (bCheckHoleCup)
			{

			}
			// 컨시드
			else
			{
				// 타수하나 줄임
				BallPlayerState->PlusScore();
			}

			// 다음 홀로 넘어가는 스텝
			MoveNextHole();
		}
		else
		{
			// 더블파 -> 다음홀로 넘어감
			if (BallPlayerState->GetNowHoleScore() == BallPlayerState->GetDoublePar())
			{
				MoveNextHole();
			}
			else
			{
				// 여기에서 남은거리에 따라 클럽 지정해줌.
				// 그린이 아닐때는 남은 거리에 따라
				if (GeographyState != EGeographyState::GREEN) {
					ChangeClubFromDis();
				}
				else
				{
					ClubState = EGolfClub::PUTTER;

					// 그린 위에 도착하면 미니맵을 바꿔줘야함
					// ex) 원래 미니맵 hidden, 그린전용 미니맵 visibility 이런식으로?
					// 그에따라 예상 도착지점도 바꾸고 표현해줄 필요 없음
					// 이부분 구현 필요
					ChangeClub();
				}
			}
		}
		//여기서 딜레이 1초 정도?

		OnOffMainPanelOnWidget.Broadcast(true);
		OnOffMovingPanelOnWidget.Broadcast(false);
		UpdateBallIconOnWidget.Broadcast();
		SetPowerZeroOnWidget.Broadcast();
		CurrentState = EBallState::STOP;
	}
}

void ABall::ChangeClub()
{
	// 각도 수정 필요
	switch (ClubState)
	{
	case EGolfClub::DRIVER:
		DrivingDis = 15000;
		ArcValue = 0.7;
		break;
	case EGolfClub::WOOD:
		DrivingDis = 13000;
		ArcValue = 0.7;
		break;
	case EGolfClub::IRON:
		DrivingDis = 10000;
		ArcValue = 0.6;
		break;
	case EGolfClub::WEDGE:
		DrivingDis = 7000;
		ArcValue = 0.3;
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
	PredictLocation = this->GetActorLocation() + BallCamera->GetForwardVector() * DrivingDis;
	UpdatePredictIconOnWidget.Broadcast();
	UpdateClubStateOnWidget.Broadcast();
}

void ABall::SetMovingDis()
{
	//공을 쳤을때 움직인 거리(위젯에서 비거리로 보여줄 변수 업데이트)
	MovingDis = FVector::Dist(BallPlayerState->GetFormerLocation(),this->GetActorLocation()) / 100;

	// 위젯에서 비거리를 업데이트 해주는 델리게이트
	UpdateMovingInfoOnWidget.Broadcast();
}

void ABall::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Print("OverLap");
	
	if (OtherComp->GetName() == TEXT("CONCEDE"))
	{
		bCheckConcede = true;
	}
	if (OtherComp->GetName() == TEXT("HOLECUP"))
	{
		bCheckHoleCup = true;
	}
}

void ABall::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Print("EndLap");

	if (OtherComp->GetName() == TEXT("CONCEDE"))
	{
		bCheckConcede = false;
	}
	if (OtherComp->GetName() == TEXT("HOLECUP"))
	{
		bCheckHoleCup = false;
	}
}