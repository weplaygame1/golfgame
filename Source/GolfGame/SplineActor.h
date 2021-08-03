// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GolfGame.h"

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
	class USplineComponent* spline;
	UPROPERTY(EditAnywhere)
	class UProceduralMeshComponent* procedural;
	UPROPERTY(EditAnywhere)
	class UMaterialInterface* material;

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

	UPROPERTY(EditAnywhere)
		FVector tempOrigin;

};
