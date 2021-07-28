// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "GolfGame.h"

#include "Materials/MaterialInterface.h"
#include "Components/SplineComponent.h"
#include "ProceduralMeshComponent.h"

#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InstancedStaticMeshComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetArrayLibrary.h"

#include "DrawDebugHelpers.h"

#include "GameFramework/Actor.h"
#include "TestSpline.generated.h"

UCLASS()
class GOLFGAME_API ATestSpline : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestSpline();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	void MakePointGrid();
	void BuildMeshFromOutline();
	void NormalizePointGridforUV();

	void checktest();

public:
	UPROPERTY(EditAnywhere)
	USplineComponent* spline;
	UPROPERTY(EditAnywhere)
	class UProceduralMeshComponent* procedural;
	UPROPERTY(EditAnywhere)
	UMaterialInterface* material;

	/* MakePointGrid */
	UPROPERTY(EditAnywhere)
	float TriangleSize;
	//UROPERTY(EditAnywhere)
	int32 GridX;

	/* BuildMeshFromOutline */
	//PointIndex == GridPoints
	//UPROPERTY(EditAnywhere)
	TArray<int32> GridPoints;

	//UPROPERTY(EditAnywhere)
	TArray<FVector> Vertices;

	//TrianglesL == Triangles
	//UPROPERTY(EditAnywhere)
	TArray<int32> Triangles;

	/* NormalizePointGridforUV */
	//UPROPERTY(EditAnywhere)
	float Padding;
	//UPROPERTY(EditAnywhere)
	TArray<FVector2D> UV;
	//UPROPERTY(EditAnywhere)
	float Scale;
	//UPROPERTY(EditAnywhere)
	FVector OriginScaled;

	
	UPROPERTY(EditAnywhere)
		bool bin;
	UPROPERTY(EditAnywhere)
		bool bline;
	UPROPERTY(EditAnywhere)
		float ftest;
	UPROPERTY(EditAnywhere)
		float fsize;
	UPROPERTY(EditAnywhere)
		float fffffff;
	UPROPERTY(EditAnywhere)
		FVector closet;
};
