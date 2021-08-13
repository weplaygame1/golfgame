// Fill out your copyright notice in the Description page of Project Settings.


#include "TestSpline.h"

#include "Materials/MaterialInterface.h"
#include "Components/SplineComponent.h"
#include "Components/DecalComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ATestSpline::ATestSpline()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	spline = CreateDefaultSubobject<USplineComponent>(TEXT("SPLINE"));
	material = CreateDefaultSubobject<UMaterialInterface>(TEXT("MATERIAL"));
	decalSize_X = 1.5;
	decalSize_Y = 0.5;
	decalSize_Z = 1;
	SOrder = 0;
}

void ATestSpline::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
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
	for (int32 index = 0; index < decalArray.Num(); index++)
	{
		decalArray[index]->DestroyComponent();
	}
	decalArray.Empty();
	for (int32 index = 0; index < boxArray.Num(); index++) {
		boxArray[index]->DestroyComponent();
	}
	boxArray.Empty();
	/*for (int32 index = 0; index < spline->GetNumberOfSplinePoints(); index++)
	{
		FHitResult OutHit;
		FVector Startpoint = spline->GetLocationAtSplinePoint(index, ESplineCoordinateSpace::World) + FVector(0, 0, 100);
		FVector Endpoint = spline->GetLocationAtSplinePoint(index, ESplineCoordinateSpace::World) - FVector(0, 0, 100);
		FCollisionQueryParams CollisionParams;

		bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, Startpoint, Endpoint, ECC_Visibility, CollisionParams);
		if (isHit)
		{
			spline->SetLocationAtSplinePoint(index, OutHit.Location, ESplineCoordinateSpace::World);
		}
	}*/

	for (int32 index = 0; index < FMath::TruncToInt(spline->GetSplineLength()) / 256 + 1; index++)
	{
		FString str = FString::Printf(TEXT("DECAL%d"), index);
		decal = NewObject<UDecalComponent>(this, FName(*str));
		decal->RegisterComponent();
		this->AddInstanceComponent(decal);

		decal->SetSortOrder(SOrder);
		decal->SetMaterial(0, material);
		decal->SetWorldScale3D(decal->GetRelativeScale3D() * FVector(decalSize_X, decalSize_Y, decalSize_Z));
		APhysicsVolume *newvolume = NULL;
		decal->SetPhysicsVolume(newvolume, true);
		decal->SetWorldLocationAndRotation(
			spline->GetLocationAtDistanceAlongSpline(index * 256, ESplineCoordinateSpace::World),
			spline->GetRotationAtDistanceAlongSpline(index * 256, ESplineCoordinateSpace::World)
		);
		decalArray.Add(decal);

		FString str1 = FString::Printf(TEXT("BOX%d"), index);
		box = NewObject<UBoxComponent>(this, FName(*str1));
		box->RegisterComponent();
		this->AddInstanceComponent(box);
		box->SetWorldLocationAndRotation(
			spline->GetLocationAtDistanceAlongSpline(index * 256, ESplineCoordinateSpace::World),
			spline->GetRotationAtDistanceAlongSpline(index * 256, ESplineCoordinateSpace::World)
		);
		box->SetBoxExtent(FVector(128, 256, 256) * FVector(decalSize_X, decalSize_Y, decalSize_Z));
		
		boxArray.Add(box);
	}
}
