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
	BallMesh->SetCollisionProfileName("SetLineTraceChannel");

	// set temp angulardamping
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
	bCheckOnce = true;
	JumpPower = 0;

	// Get Player State
	BallPlayerState = Cast<AMyPlayerState>(GetPlayerState());

	// �̷��� Ŀ���� ���� ��带 �����ü� �ִ�.
	CheckNowScore = GetWorld()->GetAuthGameMode<AGolfGameGameModeBase>()->itest;
}

// Called every frame
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// TEST


	// Check Ball is Moving
	CheckBallisMoiving();

	// Get Current Ball Location
	CurrentBallLocation = this->GetActorLocation();
	CurrentBallForwrad = BallCamera->GetForwardVector();

	// ���� ������ ��������  && �� �ѹ� ����
	if (!bIsMoving && bCheckOnce)
	{
		Print("Check");
		bCheckOnce = false;

		// Ȧ �ƿ����� üũ
		if (bCheckConcede || bCheckHoleCup)
		{
			if (bCheckHoleCup)
			{
				// Ȧ��
			}
			else
			{
				// ���õ�
				// Ÿ���ϳ� ����
			}

			// ���� Ȧ�� �Ѿ�� ����
			BallPlayerState->NextHole();

			// ���� ���� �����ϰ� ���ο� �������� �������ϴ� ���
			/*AController* controller = GetController();
			controller->UnPossess();
			
			ABall* CurrentBall = this;
			ABall* NewBall = GetWorld()->SpawnActor<ABall>(ABall::StaticClass(), FVector(1000, 18000, 20),FRotator(0,0,0));
			
			controller->Possess(NewBall);
			CurrentBall->Destroy();*/

			// ���� ���� ���������ְ� �̵��ص� �ɵ�, �̷��� �߰��� ī�޶� �����ؾ���
			// ī�޶� ���󰡴� ������ ���ֱ� ���� �ӵ� ���Ѵ�� ����
			BallCameraSpringArm->CameraLagSpeed = 0.0f;
			this->SetActorLocation(FVector(1000, 18000, 20));
			
			
			
			// �߰������� ī�޶��� ������ ���������
			// ����� �������� �ƴ� ������ ��������

			



			// ����Ȧ�� �Ѿ�� ���� üũ���ִ� 
			// *���� ������ �ִ� ������ �ʱ�ȭ* 


			
		}
		else
		{
			// �������̸� ����Ȧ�� �Ѿ
			

			// ��� ������ �ִ��� üũ
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

		//���� ġ�� ���� Ÿ���� �ϳ� �ٿ��� 
		CheckNowScore++;

		//���� ������ �����½����� ���� �ٷ� ������ ������ ������ ����
		//������ ���� ��ġ�� �����ϴ� ����� ��ġ�� �ٲ����
		//���� ���������� ��ġ�� player state �� �����Ŵ
		BallPlayerState->SetFormerLocation(CurrentBallLocation);

		 
	}
}

void ABall::OnRealseBallHit()
{
	if (bCanHitBall && !bIsMoving)
	{
		fvtemp = BallCamera->GetForwardVector()*FVector(1.0f, 1.0f, 1.0f);
		fvtemp.Z = 1;
		fvtemp = fvtemp * 100 * JumpPower;

		// �� ȸ�����̶�µ� �ٽ� üũ�ؾ��ҵ�
		av = fvtemp.GetSafeNormal();
		bv = av.ToOrientationQuat().GetRightVector();
		cv = FVector::CrossProduct(av, bv);
		
		BallMesh->AddAngularImpulseInDegrees(bv*10000, NAME_None, true);
		BallMesh->AddImpulse(fvtemp, NAME_None, true);
		
		// ���� ó�� ���� �����ϴ¼����� angular damping ���� ����������ҵ�?
		// �ƴϸ� physics material�� ���� �����������ҵ�

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
	// �ִ�ġ�� 1�� ���ϸ� �ִ� ���� 45��
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
		EPhysicalSurface epstemp = UGameplayStatics::GetSurfaceType(OutHit);

		// �� ���� �Ӽ��� �´� ���� ex) �Ŀ����� ���
		// ex) PlayerState.Surface = 0.1 -> 10% �Ŀ� ����
		switch (epstemp)
		{
		case SurfaceType1: // GREEN

			break;
		case SurfaceType2: // APRON

			break;
		case SurfaceType3: // FAIRWAY

			break;
		case SurfaceType4: //ROUGH

			break;
		case SurfaceType5: //BUNKER

			break;
		// �� �༮���� ���� �Űܾ���
		// �߰��� Ÿ���� �ٿ���
		case SurfaceType6: // HAZARD
			BallMesh->SetWorldLocation(BallPlayerState->GetFormerLocation());

			break;
		case SurfaceType7: // OB
			BallMesh->SetWorldLocation(BallPlayerState->GetFormerLocation());

			break;
	
		default:
			break;
		}
		

	}
	

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
		bCheckConcede = true;
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
		bCheckConcede = false;
	}
}