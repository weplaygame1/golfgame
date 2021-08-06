// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"

#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "MyPlayerState.h"
#include "GolfGameGameModeBase.h"

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

	bCanHitBall = true;
	bIsChargingHit = false;

	bCheckOnce = true;
	JumpPower = 0;

	CurrentState = EBallState::STOP;
	GeographyState = EGeographyState::FAIRWAY;
	ClubState = EGolfClub::WOOD;
	CalPredictLocation();

	// Get Player State
	BallPlayerState = Cast<AMyPlayerState>(GetPlayerState());
	BallPlayerState->GetParOnWidget.Broadcast();
	BallPlayerState->GetScoreOnWidget.Broadcast();
	BallPlayerState->GetWholeDistanceOnWidget.Broadcast();

	// Set Default CameraLagSpeed
	BallCameraSpringArm->CameraLagSpeed = 3.0F;
	
}

// Called every frame
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//test

	//투비전을 참고해보면 날아가는 동안 지형속성이 계속 업데이트됨
	//그럼 라인트레이스를 움직이는 동안 계속 사용해주는걸로 설정해야할듯
	//날아갈때 사용하는 라인트레이스 함수와 도착했을때 사용하는 라인트레이스 함수를 따로 만들어서
	//설정해주면 될듯, 날아갈때는 지형속성만 출력하고 지형속성 변수를 바꿀 필요는 없을듯
	//UseLineTrace();
	
	// 아래 두개는 볼이 움직일때(MOVING) 일때만 수행해줘도 될듯
	// 볼 아이콘 업데이트 델리게이트
	UpdateBallIconOnWidget.Broadcast();
	// 남은거리 갱신
	BallPlayerState->SetDistanceRemaining();

	switch (CurrentState)
	{
	case EBallState::STOP:
		CalPredictLocation();
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

	// Hit : Press H
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
		FVector startLoc = this->GetActorLocation();// 발사 지점
		FVector targetLoc = PredictLocation;		// 타겟 지점.
		float arcValue = 0.5;                       // ArcParam (0.0-1.0)
		outVelocity = FVector::ZeroVector;			// 결과 Velocity
		if (UGameplayStatics::SuggestProjectileVelocity_CustomArc(this, outVelocity, startLoc, targetLoc, GetWorld()->GetGravityZ(), ArcValue))
		{
			FPredictProjectilePathParams predictParams(-1, startLoc+FVector(0,0,10), outVelocity, 0.3f);   // 20: tracing 보여질 프로젝타일 크기, 15: 시물레이션되는 Max 시간(초)
			predictParams.DrawDebugTime = 5.0f;     //디버그 라인 보여지는 시간 (초)
			predictParams.DrawDebugType = EDrawDebugTrace::Type::ForDuration;  // DrawDebugTime 을 지정하면 EDrawDebugTrace::Type::ForDuration 필요.
			predictParams.OverrideGravityZ = 0;
			
			
			FPredictProjectilePathResult result;
			UGameplayStatics::PredictProjectilePath(this, predictParams, result);
		}

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
		FVector startLoc = this->GetActorLocation();// 발사 지점
		FVector targetLoc = this->GetActorLocation() + (BallCamera->GetForwardVector() * (DrivingDis * PowerPercent / 100));// 타겟 지점.
		outVelocity = FVector::ZeroVector;// 결과 Velocity
		if (UGameplayStatics::SuggestProjectileVelocity_CustomArc(this, outVelocity, startLoc, targetLoc, GetWorld()->GetGravityZ(), ArcValue))
		{
			//FPredictProjectilePathParams predictParams(-1, startLoc, outVelocity, 5.0f);   // 20: tracing 보여질 프로젝타일 크기, 15: 시물레이션되는 Max 시간(초)
			//predictParams.DrawDebugTime = 5.0f;     //디버그 라인 보여지는 시간 (초)
			//predictParams.DrawDebugType = EDrawDebugTrace::Type::ForDuration;  // DrawDebugTime 을 지정하면 EDrawDebugTrace::Type::ForDuration 필요.
			//predictParams.OverrideGravityZ = 0;

			//FPredictProjectilePathResult result;
			//UGameplayStatics::PredictProjectilePath(this, predictParams, result);
		}
		
		//BallMesh->AddImpulse(outVelocity, NAME_None, true);
		BallMesh->AddImpulse(BallCamera->GetForwardVector()*2000, NAME_None, true);

		switch (ClubState)
		{
		case EGolfClub::WOOD:
		case EGolfClub::IRON:
			


			// 다른 클럽들은 SuggestProjectileVelocity_CustomArc 함수사용
			// 퍼터는 해당 방향에 파워만 조절하면 될듯, 즉 각도가 필요없음
			// why? 각도가 낮으면 버그발생
			break;
		case EGolfClub::PUTTER:
			break;
		default:
			break;
		}



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

	/*
	if (bCanHitBall && !bIsMoving)
	{
		fvtemp = BallCamera->GetForwardVector()*FVector(1.0f, 1.0f, 1.0f);
		fvtemp.Z = 1;
		fvtemp = fvtemp * 100 * JumpPower;

		// 각 회전축이라는데 다시 체크해야할듯
		av = fvtemp.GetSafeNormal();
		bv = av.ToOrientationQuat().GetRightVector();
		cv = FVector::CrossProduct(av, bv);
		
		BallMesh->AddAngularImpulseInDegrees(bv*10000, NAME_None, true);
		BallMesh->AddImpulse(fvtemp, NAME_None, true);
		
		// 공이 처음 땅에 착지하는순간에 angular damping 값을 설정해줘야할듯?
		// 아니면 physics material를 만들어서 적용시켜줘야할듯

		bIsChargingHit = false;
		JumpPower = 0;
		JumpAngle = 0;
	}
	*/
}

void ABall::OnPressChangeClub()
{
	// 아직 클럽 사용 룰을 적용하지 않음
	// 우선 모든 클럽을 선택할 수 있게 구현
	if (CurrentState == EBallState::STOP)
	{
		int32 index = (int32)ClubState + 1;

		if (index == (int32)StaticEnum<EGolfClub>()->NumEnums() - 1)
		{
			index = 0;
		}

		ClubState = (EGolfClub)StaticEnum<EGolfClub>()->GetValueByIndex(index);

		CalPredictLocation();
		UpdateClubStateOnWidget.Broadcast();
	}
}

void ABall::MoveDirection(float AxisValue)
{
	if (AxisValue != 0 && CurrentState == EBallState::STOP)
	{
		AddControllerYawInput(AxisValue);
	}
}

void ABall::CheckBallisMoiving()
{
	SetMovingDis();
	
	if (BallMesh->GetComponentVelocity().Size() > 0.f)
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
	Print("UseLineTrace");

	FVector Startpoint = this->GetActorLocation() + FVector(0.0f, 0.0f, 5.0f);
	FVector Endpoint = Startpoint * FVector(1.0f, 1.0f, 0.0f) + FVector(0.0f, 0.0f, -100.0f);
	FCollisionQueryParams CollisionParams;
	CollisionParams.bReturnPhysicalMaterial = true;
	CollisionParams.bTraceComplex = false;
	
	//DrawDebugLine(GetWorld(), Startpoint, Endpoint, FColor::Orange,false,10);

	//ECC_GameTraceChannel1 -> Custom trace channel
	bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, Startpoint, Endpoint, ECC_GameTraceChannel1, CollisionParams);
	if (isHit)
	{
		fsttt=OutHit.GetActor()->GetName();
		FName GeoState = OutHit.GetComponent()->GetMaterial(0)->GetFName();

		//디버그용, 확인하고 지우기
		//itestest = StaticEnum<EGeographyState>()->GetValueByName(GeoState);

		GeographyState = (EGeographyState)StaticEnum<EGeographyState>()->GetValueByName(GeoState);

		// 각 지형 속성에 맞는 설정 ex) 파워감소 등등
		// ex) PlayerState.Surface = 0.1 -> 10% 파워 감소
		switch (GeographyState)
		{
		case EGeographyState::ROUGH:
			Print("ROUGH");

			break;
		case EGeographyState::FAIRWAY:
			Print("FAIRWAY");

			break;
		case EGeographyState::GREEN:
			Print("GREEN");

			break;
		case EGeographyState::BUNKER:
			Print("BUNKER");

			break;
		default:
			Print("DEFAULT");

			//GetValueByName(FName name);
			//만약 enum에 없는 이름이면 -1을 리턴시킴 -> 이걸 사용하는 방법도 가능



			break;
		}

		/*
		EPhysicalSurface epstemp = UGameplayStatics::GetSurfaceType(OutHit);

		// 각 지형 속성에 맞는 설정 ex) 파워감소 등등
		// ex) PlayerState.Surface = 0.1 -> 10% 파워 감소
		switch (epstemp)
		{
		case SurfaceType1: // GREEN
			NowMaterial = TEXT("GREEN");
			break;
		case SurfaceType2: // APRON
			NowMaterial = TEXT("APRON");

			break;
		case SurfaceType3: // FAIRWAY
			NowMaterial = TEXT("FAIRWAY");

			break;
		case SurfaceType4: //ROUGH
			NowMaterial = TEXT("ROUGH");

			break;
		case SurfaceType5: //BUNKER
			NowMaterial = TEXT("BUNKER");

			break;
		// 이 녀석들은 공을 옮겨야함
		// 추가로 타수를 줄여줌
		case SurfaceType6: // HAZARD
			//this->SetActorLocation(BallPlayerState->GetFormerLocation());
			
			break;
		case SurfaceType7: // OB
			//this->SetActorLocation(BallPlayerState->GetFormerLocation());

			break;
	
		default:
			break;
		}
		*/

	}
	else
	{
		// OB , HAZARD 일때?
		// 이 녀석들은 공을 옮겨야함
		// 추가로 타수를 줄여줌
		Print("OUT");

		// 우선은 OB, HAZARD 공통적으로 그 전 위치로 보내줌
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
	}
	else
	{
		// 게임끝났을때 temp
		Print("End");
		this->Destroy();
	}
}

void ABall::ChargingPower()
{
	PrintWithFloat("", PowerPercent);
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
				// 어느 지형에 있는지 체크
				UseLineTrace();
			}
		}

		SetPowerZeroOnWidget.Broadcast();
		CurrentState = EBallState::STOP;
	}
}

void ABall::CalPredictLocation() {

	switch (ClubState)
	{
	case EGolfClub::WOOD:
		DrivingDis = 20000;
		ArcValue = 0.7;
		break;
	case EGolfClub::IRON:
		DrivingDis = 12000;
		ArcValue = 0.6;
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