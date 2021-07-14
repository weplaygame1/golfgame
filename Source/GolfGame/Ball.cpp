// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"

// Sets default values
ABall::ABall()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create BallMesh
	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BALLMESH"));
	BallMesh->SetSimulatePhysics(true);
	
	//BallMesh->SetNotifyRigidBodyCollision(true);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BALL(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if (SM_BALL.Succeeded())
	{
		BallMesh->SetStaticMesh(SM_BALL.Object);
		BallMesh->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
	}
	RootComponent = BallMesh;

	BallMesh->SetCollisionObjectType(ECC_Pawn);
	BallMesh->SetCollisionProfileName("SetLineTraceChannel");

	// temp set angulardamping
	BallMesh->SetAngularDamping(30);


	// Create CameraSpringArm;
	BallCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CAMERASPRINGARM"));
	BallCameraSpringArm->SetupAttachment(RootComponent);
	BallCameraSpringArm->TargetArmLength = 600.0F;
	BallCameraSpringArm->bEnableCameraLag = true;
	BallCameraSpringArm->bUsePawnControlRotation = true;
	BallCameraSpringArm->CameraLagSpeed = 3.0F;
	
	
	
	BallCameraSpringArm->SocketOffset = FVector(0.0F, 0.0F, 200.0F);
	BallCameraSpringArm->TargetOffset = FVector(0.0F, 0.0F, 50.0F);
	
	// Create Camera
	BallCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("BALLCAMERA"));
	BallCamera->SetupAttachment(BallCameraSpringArm, USpringArmComponent::SocketName);


	
	

	// Set Default Value
	bCanHitBall = true;
	bCheckOnce = true;
	JumpPower = 0;


	// Set CurrentHoleName
	CurrentHoleNumber = 0;
	CurrentHoleName = FString(TEXT("Hole")) + FString::FromInt(CurrentHoleNumber);
	
}

// Called when the game starts or when spawned
void ABall::BeginPlay()
{
	Super::BeginPlay();
	
	// Set Default Value
	bCheckHoleOut = false; 
	bCheckConcede = false;

	// 해저드나 오비일때도 리스폰하는 형식으로 즉 계속 타수++ 
	// 이러면 홀아웃으로인한 스폰을때는 타수의 기본값에 -1 더해줘야할듯

	// 리스폰하는순간 다른 actor 이므로 기존의 값 다 지워짐
	// 즉 ball에 저장하는게 아닌 다른곳에 저장시켜야함
	
	
}

// Called every frame
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Check Ball is Moving
	CheckBallisMoiving();

	// Get Current Ball Location
	CurrentBallLocation = this->GetActorLocation();
	CurrentBallForwrad = BallCamera->GetForwardVector();

	// 공이 완전히 멈췄을때 딱 한번 수행
	if (!bIsMoving && bCheckOnce)
	{
		Print("Check");
		bCheckOnce = false;

		// 홀 아웃인지 아닌지 체크후
		if (bCheckHoleOut) 
		{
			// 홀아웃이면 공 삭제하고 새로 만들고 다시 빙의
			// 이걸 함수로 따로 빼면
			// 함수 파라미터로 홀 숫자를 주고 
			// 해당 인덱스의 위치로 공을 스폰해주면 될듯

			AController* controller = GetController();
			controller->UnPossess();
			
			ABall* CurrentBall = this;
			ABall* NewBall = GetWorld()->SpawnActor<ABall>(ABall::StaticClass(), FVector(3000, 3000, 20),FRotator(0,0,0));
			
			controller->Possess(NewBall);
			CurrentBall->Destroy();
		}
		else
		{
			// Line trace
			UseLineTrace();
		}
	}
}

// Called to bind functionality to input
void ABall::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Hit : Press H
	PlayerInputComponent->BindAction("BallHit", IE_Pressed, this, &ABall::OnPressBallHit);
	PlayerInputComponent->BindAction("BallHit", IE_Released, this, &ABall::OnRealseBallHit);

	// GetPower : Press H
	PlayerInputComponent->BindAxis("GetPower", this, &ABall::GettingPower);

	// Move Direction : Press A or D
	PlayerInputComponent->BindAxis("MoveDirection", this, &ABall::MoveDirection);
	// Move Angle : Press W or S
	PlayerInputComponent->BindAxis("MoveAngle", this, &ABall::MoveAngle);

	
	
	
}

void ABall::OnPressBallHit()
{
	if (bCanHitBall && !bIsMoving)
	{
		bIsChargingHit = true;
		Print("Press H");

	}
}

void ABall::OnRealseBallHit()
{
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
}

void ABall::MoveDirection(float AxisValue)
{
	if (AxisValue != 0 && !bIsMoving)
	{
		if (!bIsChargingHit)
		{
			AddControllerYawInput(AxisValue);
		}
	}
}

void ABall::MoveAngle(float AxisValue)
{
	// 최대치를 1로 정하면 최대 각도 45도
	if (AxisValue != 0 && !bIsMoving)
	{
		if (!bIsChargingHit)
		{

			/*if (JumpAngle <= 0.99 && AxisValue == 1)
			{
				JumpAngle += 0.01;

				PrintWithFloat("Angle : ", JumpAngle);
			}
			else if (JumpAngle >= 0.01 && AxisValue == -1)
			{
				JumpAngle -= 0.01;

				PrintWithFloat("Angle : ", JumpAngle);

			}*/
			JumpAngle = 1;
		}
	}
}

void ABall::GettingPower(float AxisValue)
{
	if (AxisValue != 0 && !bIsMoving)
	{
		/*if (bCanHitBall)
		{
			if (JumpPower <= 9.5)
			{
				JumpPower += 0.5;
				PrintWithFloat("Power : ", JumpPower);
			}
		}*/
		JumpPower = 10;
	}
}

void ABall::CheckBallisMoiving()
{
	if (BallMesh->GetComponentVelocity().Size()>0)
	{
		bIsMoving = true;
		bCheckOnce = true;
	}
	else
	{
		bIsMoving = false;
	}
}

void ABall::UseLineTrace()
{
	Print("UseLineTrace");

	FVector Startpoint = CurrentBallLocation;
	FVector Endpoint = CurrentBallLocation * FVector(1.0f, 1.0f, 0.0f) + FVector(0.0f, 0.0f, -10.0f);
	FCollisionQueryParams CollisionParams;
	CollisionParams.bReturnPhysicalMaterial = true;
	CollisionParams.bTraceComplex = false;
	
	//DrawDebugLine(GetWorld(), Startpoint, Endpoint, FColor::Orange,false,10);

	bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, Startpoint, Endpoint, ECC_Visibility, CollisionParams);
	if (isHit)
	{
		CureentActorName = OutHit.GetActor()->GetActorLabel();
		EPhysicalSurface epstemp = UGameplayStatics::GetSurfaceType(OutHit);
		
		if (epstemp == 1) NowMaterial = TEXT("Green");
		else if (epstemp == 2) NowMaterial = TEXT("Apron");
		else if (epstemp == 3) NowMaterial = TEXT("Fairway");
		else if (epstemp == 4) NowMaterial = TEXT("Rough");
		else if (epstemp == 5) NowMaterial = TEXT("Bunker");
		else if (epstemp == 6) NowMaterial = TEXT("Hazard");
		else if (epstemp == 7) NowMaterial = TEXT("OB");
		

	}
	

}

void ABall::NotifyHit(UPrimitiveComponent *MyComp, AActor *Other, UPrimitiveComponent *OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult &Hit)
{
	// 이러면 굳이 line trace를 사용할 이유가 없다
	if (bIsMoving) {

		// just debug
		PrintWithFloat("Hit", fcheck++);

		CureentActorName = Hit.GetActor()->GetActorLabel();
		EPhysicalSurface epstemp = UGameplayStatics::GetSurfaceType(Hit);

		if (epstemp == 1) NowMaterial = TEXT("Green");
		else if (epstemp == 2) NowMaterial = TEXT("Apron");
		else if (epstemp == 3) NowMaterial = TEXT("Fairway");
		else if (epstemp == 4) NowMaterial = TEXT("Rough");
		else if (epstemp == 5) NowMaterial = TEXT("Bunker");
		else if (epstemp == 6) NowMaterial = TEXT("Hazard");
		else if (epstemp == 7) NowMaterial = TEXT("OB");
	}
	
}

void ABall::NotifyActorBeginOverlap(AActor *OtherActor)
{
	Print("overlap");
	bCheckHoleOut = true;
}

void ABall::NotifyActorEndOverlap(AActor *OtherActor)
{
	Print("Endoverlap");
	bCheckHoleOut = false;
}