// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"
#include "Course.h"

// Sets default values
ABall::ABall()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Collision
	BallCollision = CreateDefaultSubobject<USphereComponent>(TEXT("BALLCOLLISION"));
	BallCollision->SetSphereRadius(5.0f);
	BallCollision->SetSimulatePhysics(true);
	BallCollision->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	
	BallCollision->SetCollisionProfileName("SetLineTraceChannel");
	//BallCollision->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	// Set Damping
	BallCollision->SetAngularDamping(30.0f);
	RootComponent = BallCollision;

	// Create BallMesh
	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BALLMESH"));
	BallMesh->SetupAttachment(BallCollision);
	BallMesh->SetCollisionProfileName("NoCollision");
	//BallMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BALL(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if (SM_BALL.Succeeded())
	{
		BallMesh->SetStaticMesh(SM_BALL.Object);
		BallMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -5.0f));
		BallMesh->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
	}

	// Create CameraSpringArm;
	BallCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CAMERASPRINGARM"));
	BallCameraSpringArm->SetupAttachment(BallCollision);
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

	// Line trace
	UseLineTrace();
	

	// 공이 멈추고 && 불가능한 지역에 있을때 
	// 적절한 위치에 리스폰 

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

	// Rotate viewpoint : Press A or D
	PlayerInputComponent->BindAxis("ViewMove", this, &ABall::ViewMove);

	

	
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
		FVector temp = BallCamera->GetForwardVector()*FVector(1.0f, 1.0f, 0.0f)*JumpPower;
		temp.Z = JumpPower;

		BallCollision->AddImpulse(temp * 50.0f);

		bIsChargingHit = false;
		
		JumpPower = 0;
	}
}

void ABall::ViewMove(float AxisValue)
{
	if (AxisValue != 0 && !bIsMoving)
	{
		if (!bIsChargingHit)
		{
			AddControllerYawInput(AxisValue);
		}
	}
}

void ABall::GettingPower(float AxisValue)
{
	if (AxisValue != 0 && !bIsMoving)
	{
		if (bCanHitBall)
		{
			JumpPower += 0.5;
			PrintWithFloat("", JumpPower);
		}
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
			bCanHitBall = false;
		}
		else
		{
			bCanHitBall = true;

			// Chekc Fairway, OB, Hazard etc..
			if (CurrentComponentName == FName(TEXT("OB")))
			{
				bCanHitBall = false;

			}
			else if (CurrentComponentName == FName(TEXT("BUNKER")))
			{

			}
			else if (CurrentComponentName == FName(TEXT("GREEN")))
			{

			}
		}

	}
}