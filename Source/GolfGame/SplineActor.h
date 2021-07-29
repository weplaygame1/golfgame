// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "GolfGame.h"

#include "Materials/MaterialInterface.h"
#include "Components/SplineComponent.h"
#include "ProceduralMeshComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetArrayLibrary.h"

#include "GameFramework/Actor.h"
#include "SplineActor.generated.h"

UCLASS()
class GOLFGAME_API ASplineActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASplineActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	void MakePointGrid();
	void BuildMeshFromOutline();
	void NormalizePointGridforUV();
	
	// 어떤 위치가 액터 내부에 있는지 체크
	void checktest();

public:
	UPROPERTY(EditAnywhere)
	USplineComponent* spline;
	UPROPERTY(EditAnywhere)
	UProceduralMeshComponent* procedural;
	UPROPERTY(EditAnywhere)
	UMaterialInterface* material;

	/* MakePointGrid */
	UPROPERTY(EditAnywhere)
	float TriangleSize;
	int32 GridX;

	/* BuildMeshFromOutline */
	TArray<int32> GridPoints;
	TArray<FVector> Vertices;
	TArray<int32> Triangles;

	TArray<FColor> VertexColor;

	/* NormalizePointGridforUV */
	float Padding;
	TArray<FVector2D> UV;
	float Scale;
	FVector OriginScaled;

};
