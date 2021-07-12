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
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BALL(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if (SM_BALL.Succeeded())
	{
		BallMesh->SetStaticMesh(SM_BALL.Object);
		BallMesh->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
	}
	RootComponent = BallMesh;

	BallMesh->SetCollisionObjectType(ECC_Pawn);
	//BallMesh->SetCollisionProfileName("SetLineTraceChannel");
	BallMesh->bReturnMaterialOnMove = true;

	BallMesh->SetAngularDamping(0.5);


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


	BallCollision = CreateDefaultSubobject<USphereComponent>(TEXT("BallCollision"));
	BallCollision->SetupAttachment(RootComponent);
	BallCollision->bReturnMaterialOnMove = true;


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
	CollisionParams.bReturnPhysicalMaterial = true;
	CollisionParams.bTraceComplex = false;

	
	/*bool isHit = GetWorld()->LineTraceMultiByChannel(OutHits, Startpoint, Endpoint, ECC_Visibility, CollisionParams);
	if (isHit && OutHits.Num() > 1)
	{
		
		Atemp = OutHits[1].GetActor();
		Ptemp = OutHits[1].GetComponent();
		
		CureentActorName = Atemp->GetActorLabel();
		CurrentComponentName = Ptemp->GetFName();

		if (CurrentHoleName != CureentActorName)
		{
			bCanHitBall = false;
		}
	}*/

	//DrawDebugLine(GetWorld(), Startpoint, Endpoint, FColor::Orange, false, 2.0f);

	bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, Startpoint, Endpoint, ECC_Visibility, CollisionParams);
	if (isHit)
	{
		Atemp = OutHit.GetActor();
		Ptemp = OutHit.GetComponent();

		CureentActorName = Atemp->GetActorLabel();
		CurrentComponentName = Ptemp->GetFName();

		
		EPhysicalSurface epstemp = UGameplayStatics::GetSurfaceType(OutHit);
		itemp = static_cast<int>(epstemp);
		
	}
	
	//OutHit.PhysMaterial.Get()


}