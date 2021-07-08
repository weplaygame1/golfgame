// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"

// Sets default values
ABall::ABall()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Collision
	BallCollision = CreateDefaultSubobject<USphereComponent>(TEXT("BALLCOLLISION"));
	BallCollision->SetSphereRadius(5.f);
	BallCollision->SetSimulatePhysics(true);
	
	BallCollision->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	
	BallCollision->SetCollisionProfileName("SetLineTraceChannel");
	BallCollision->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	// Set Damping
	BallCollision->SetAngularDamping(30.0f);
	RootComponent = BallCollision;
	


	// Create BallMesh
	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BALLMESH"));
	BallMesh->SetupAttachment(RootComponent);
	BallMesh->SetCollisionProfileName("NoCollision");
	BallMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BALL(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if (SM_BALL.Succeeded())
	{
		BallMesh->SetStaticMesh(SM_BALL.Object);
		BallMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -5.f));
		BallMesh->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
	}
	

	// Create CameraSpringArm;
	BallCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CAMERASPRINGARM"));
	BallCameraSpringArm->SetupAttachment(RootComponent);
	BallCameraSpringArm->TargetArmLength = 600.0F;
	BallCameraSpringArm->bEnableCameraLag = true;
	BallCameraSpringArm->bUsePawnControlRotation = true;
	BallCameraSpringArm->CameraLagSpeed = 3.0F;
	BallCameraSpringArm->SocketOffset = FVector(0.0F, 0.0F, 200.0F);

	// Create Camera
	BallCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("BALLCAMERA"));
	BallCamera->SetupAttachment(BallCameraSpringArm, USpringArmComponent::SocketName);
	




	// Set Default Value
	bCanHitBall = true;
	JumpPower = 0;


	// Set CurrentHoleName
	CurrentHoleNumber = 0;
	CurrentHoleName = FString(TEXT("Hole")) + FString::FromInt(CurrentHoleNumber);
	
	
}

// Called when the game starts or when spawned
void ABall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	// Check Ball is Moving
	CheckBallisMoiving();

	// Get Current Ball Location
	CurrentBallLocation = this->GetActorLocation();
	CurrentForwrad = BallCamera->GetForwardVector();

	// Line trace
	UseLineTrace();
	



	/*frtemp = BallCollision->GetRelativeRotation();
	
	if (CurrentBallLocation.Z > GetBallLocation.Z)
	{
		BallCollision->AddAngularImpulseInDegrees(bv*(10000000));

		if (maxspeed < BallCollision->GetComponentVelocity().GetAbsMax())
		{
			maxspeed = BallCollision->GetComponentVelocity().GetAbsMax();
		}
		
	}*/
	
	
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


		GetBallLocation = CurrentBallLocation;
		maxspeed = 0;
	}
}

void ABall::OnRealseBallHit()
{
	if (bCanHitBall && !bIsMoving)
	{


		fvtemp = BallCamera->GetForwardVector()*FVector(1.0f, 1.0f, 1.0f);
		fvtemp.Z = 1;
		fvtemp = fvtemp * 100 * JumpPower;
		

		av = fvtemp.GetSafeNormal();
		bv = av.ToOrientationQuat().GetRightVector()*1000;
		cv = FVector::CrossProduct(av, bv);

		//BallCollision->AddAngularImpulseInDegrees(bv*10000000000000000, NAME_None, true);
		BallCollision->SetAllPhysicsAngularVelocityInDegrees(bv);


		BallCollision->AddAngularImpulseInDegrees(bv,NAME_None,true);

		BallCollision->AddImpulse(fvtemp,NAME_None,true);
		
		
		

		//BallCollision->AddAngularImpulseInRadians(FVector(.0f,.0f,1000000.f), NAME_None, true);
		//BallCollision->AddAngularImpulseInRadians(fvtemp*10000000000, NAME_None, false);
		


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
	if (this->GetVelocity().Size())
	{
		bIsMoving = true;
	}
	else
	{
		bIsMoving = false;
	}
}

void ABall::UseLineTrace()
{
	FVector Startpoint = CurrentBallLocation;
	FVector Endpoint = CurrentBallLocation * FVector(1.0f, 1.0f, 0.0f);
	FCollisionQueryParams CollisionParams;

	bool isHit = GetWorld()->LineTraceMultiByChannel(OutHits, Startpoint, Endpoint, ECC_Visibility, CollisionParams);
	if (isHit && OutHits.Num() > 1)
	{
		Atemp = OutHits[1].GetActor();
		Ptemp = OutHits[1].GetComponent();
		
		CureentActorName = Atemp->GetActorLabel();
		CurrentComponentName = Ptemp->GetFName();

		// Check 현재 진행하는 홀인지 -> 이게 OB 일까?
		if (CurrentHoleName != CureentActorName)
		{
			//bCanHitBall = false;
		}
		else
		{
			bCanHitBall = true;

			// Chekc Fairway, OB, Hazard etc..
			// 이런식보다는 스플라인을 사용해볼것
			if (CurrentComponentName == FName(TEXT("OB")))
			{

			}
			else if (CurrentComponentName == FName(TEXT("BUNKER")))
			{

			}
			else if (CurrentComponentName == FName(TEXT("GREEN")))
			{

			}
			else if (CurrentComponentName == FName(TEXT("HOLECUP")))
			{

			}
		}

	}
}