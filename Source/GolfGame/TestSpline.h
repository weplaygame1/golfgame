// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "GolfGame.h"

#include "Materials/MaterialInterface.h"
#include "Components/SceneComponent.h"
#include "Components/SplineComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "ProceduralMeshComponent.h"

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

	void MakePointGrid();
	void BuildMeshFromOutline();
	void NormalizePointGridforUV();

public:
	UPROPERTY(EditAnywhere)
	USceneComponent* scence;
	UPROPERTY(EditAnywhere)
	USplineComponent* spline;
	UPROPERTY(EditAnywhere)
		class UProceduralMeshComponent* splineprocedural;
	UPROPERTY(EditAnywhere)
		UMaterialInterface* material;

	//MakePointGrid
	UPROPERTY(EditAnywhere)
		float TriangleSize;
	UPROPERTY(EditAnywhere)
		int32 NumX;
	UPROPERTY(EditAnywhere)
		int32 NumY;
	UPROPERTY(EditAnywhere)
		int32 CurrentPoint;
	UPROPERTY(EditAnywhere)
		int32 GridX;

	//PointIndex==GridPoints
	//UPROPERTY(EditAnywhere)
		TArray<int32> PointIndex;
	//UPROPERTY(EditAnywhere)
		TArray<int32> GridPoints;

	//UPROPERTY(EditAnywhere)
		TArray<FVector> Vertices;

	//TrianglesL==Triangles
	//UPROPERTY(EditAnywhere)
		TArray<int32> TrianglesL;
	//UPROPERTY(EditAnywhere)
		TArray<int32> Triangles;

	//NormalizePointGridforUV
	UPROPERTY(EditAnywhere)
		float Padding;
	UPROPERTY(EditAnywhere)
		TArray<FVector2D> UV;
	UPROPERTY(EditAnywhere)
		float Scale;
	UPROPERTY(EditAnywhere)
		FVector OriginScaled;


};
