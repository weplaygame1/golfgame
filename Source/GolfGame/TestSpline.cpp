// Fill out your copyright notice in the Description page of Project Settings.


#include "TestSpline.h"

#include "Materials/MaterialInterface.h"
#include "Engine/StaticMesh.h"

#include "Components/SplineComponent.h"
#include "Components/DecalComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Components/SplineMeshComponent.h"

// Sets default values
ATestSpline::ATestSpline()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	spline = CreateDefaultSubobject<USplineComponent>(TEXT("SPLINE"));
	spline->SetMobility(EComponentMobility::Static);
	RootComponent = spline;
	
	material = CreateDefaultSubobject<UMaterialInterface>(TEXT("MATERIAL"));
	
	decalSize_X = 1;
	decalSize_Y = 0.5;
	decalSize_Z = 1;
	SOrder = 0;

	bCreateDecal = true;
	bCreateMesh = false;
	bCreateBox = true;
	bCreateSplineMesh = false;

	inum = 100;

	
}

void ATestSpline::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	remove0();
	build0();
}

// Called when the game starts or when spawned
void ATestSpline::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestSpline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

void ATestSpline::build0()
{
	if (bCreateSplineMesh) {
		for (int32 index = 0; index < spline->GetNumberOfSplinePoints(); index++)
		{
			FString str = FString::Printf(TEXT("SP%d"), index);
			splinecom = NewObject<USplineMeshComponent>(this, FName(*str));
			splinecom->RegisterComponent();
			this->AddInstanceComponent(splinecom);
			
			splinecom->SetStaticMesh(splinemesh);
			splinecom->SetForwardAxis(ESplineMeshAxis::Z);
			splinecom->SetCollisionProfileName("OverlapAll");
			splinecom->SetUseCCD(true);

			// close loop 했을때 마지막과 시작부분을 이어주는 스플라인 메쉬 컴포넌트
			if (index == spline->GetNumberOfSplinePoints() - 1)
			{
				splinecom->SetStartAndEnd(
					spline->GetLocationAtSplinePoint(index, ESplineCoordinateSpace::World) + FVector(0, 0, 1500),
					spline->GetTangentAtSplinePoint(index, ESplineCoordinateSpace::World),
					spline->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World) + FVector(0, 0, 1500),
					spline->GetTangentAtSplinePoint(0, ESplineCoordinateSpace::World)
				);
			}
			else
			{
				splinecom->SetStartAndEnd(
					spline->GetLocationAtSplinePoint(index, ESplineCoordinateSpace::World) + FVector(0, 0, 1500),
					spline->GetTangentAtSplinePoint(index, ESplineCoordinateSpace::World),
					spline->GetLocationAtSplinePoint(index + 1, ESplineCoordinateSpace::World) + FVector(0, 0, 1500),
					spline->GetTangentAtSplinePoint(index + 1, ESplineCoordinateSpace::World)
				);
			}
			splinecom->SetHiddenInGame(true);

			splineArr.Add(splinecom);
		}
	}

	/*
	// 스플라인 포인트를 땅에 붙이는 기능
	for (int32 index = 0; index < spline->GetNumberOfSplinePoints(); index++)
	{
		FHitResult OutHit;
		FVector Startpoint = spline->GetLocationAtSplinePoint(index, ESplineCoordinateSpace::World);
		FVector Endpoint = Startpoint + FVector(0, 0, -1000);
		FCollisionQueryParams CollisionParams;

		bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, Startpoint, Endpoint, ECC_Visibility, CollisionParams);
		if (isHit)
		{
			spline->SetLocationAtSplinePoint(index, OutHit.Location, ESplineCoordinateSpace::World);
		}
	}
	*/

	for (int32 index = 0; index < FMath::TruncToInt(spline->GetSplineLength() / inum) + 1; index++)
	{
		FVector Loc = spline->GetLocationAtDistanceAlongSpline(index * inum, ESplineCoordinateSpace::World);
		FRotator Rot = spline->GetRotationAtDistanceAlongSpline(index * inum, ESplineCoordinateSpace::World);
		Rot.Pitch = 0;

		// 데칼 생성
		if (bCreateDecal)
		{
			FString str = FString::Printf(TEXT("DECAL%d"), index);
			decal = NewObject<UDecalComponent>(this, FName(*str));

			decal->RegisterComponent();
			this->AddInstanceComponent(decal);
			
			decal->SetSortOrder(SOrder);
			decal->SetMaterial(0, material);
			decal->SetWorldScale3D(decal->GetRelativeScale3D() * FVector(decalSize_X, decalSize_Y, decalSize_Z));

			decal->SetWorldLocationAndRotation(Loc, Rot);

			decalArr.Add(decal);
		}

		// 박스 트리거 생성
		if (bCreateBox) 
		{
			FString str1 = FString::Printf(TEXT("BOX%d"), index);
			box = NewObject<UBoxComponent>(this, FName(*str1));
			box->RegisterComponent();
			this->AddInstanceComponent(box);

			box->SetGenerateOverlapEvents(false);
			box->SetCollisionProfileName("NoCollision");
			box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			box->SetWorldLocationAndRotation(Loc, Rot);
			box->SetBoxExtent(FVector(128, 256, 256) * FVector(decalSize_X, decalSize_Y, decalSize_Z));

			boxArr.Add(box);
		}
	}

	if (bCreateMesh)
	{
		for (int32 index = 0; index < FMath::TruncToInt(spline->GetSplineLength() / 2000) + 1; index++)
		{
			// 메쉬 생성
			FString str1 = FString::Printf(TEXT("MESH%d"), index);
			staticcom = NewObject<UStaticMeshComponent>(this, FName(*str1));
			staticcom->RegisterComponent();
			this->AddInstanceComponent(staticcom);

			staticcom->SetStaticMesh(staticmesh);
			staticcom->SetReceivesDecals(false);
			staticcom->SetWorldScale3D(FVector(0.2, 0.2, 0.2));

			FHitResult OutHit;
			FVector Startpoint = spline->GetLocationAtDistanceAlongSpline(index * 2000, ESplineCoordinateSpace::World) + FVector(0, 0, 1000);
			FVector Endpoint = Startpoint * FVector(1, 1, 0) + FVector(0, 0, -1000);
			FCollisionQueryParams CollisionParams;
			bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, Startpoint, Endpoint, ECC_Visibility, CollisionParams);
			if (isHit)
			{
				staticcom->SetWorldLocation(OutHit.Location + FVector(0, 0, -10));
			}
			else
			{
				staticcom->SetWorldLocation(spline->GetLocationAtDistanceAlongSpline(index * 2000, ESplineCoordinateSpace::World));
			}

			staticArr.Add(staticcom);
		}
	}

}

void ATestSpline::remove0()
{
	for (int index = 0; index < splineArr.Num(); index++)
	{
		splineArr[index]->DestroyComponent();
	}
	splineArr.Empty();

	for (int index = 0; index < decalArr.Num(); index++)
	{
		decalArr[index]->DestroyComponent();
	}
	decalArr.Empty();

	for (int index = 0; index < boxArr.Num(); index++)
	{
		boxArr[index]->DestroyComponent();
	}
	boxArr.Empty();

	for (int index = 0; index < staticArr.Num(); index++)
	{
		staticArr[index]->DestroyComponent();
	}
	staticArr.Empty();
}
