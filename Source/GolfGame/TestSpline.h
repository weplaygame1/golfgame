// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

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
	void build0();
	void remove0();

	FString GetMaterialName() { return material->GetName(); }
public:
	UPROPERTY(EditAnywhere)
	class UMaterialInterface* material;
	UPROPERTY(EditAnywhere)
	class UStaticMesh* staticmesh;
	UPROPERTY(EditAnywhere)
	class UStaticMesh* splinemesh;
	
	UPROPERTY(EditAnywhere)
	class USplineComponent* spline;
	UPROPERTY()
	class UDecalComponent* decal;
	UPROPERTY()
	class UBoxComponent* box;
	UPROPERTY()
	class UStaticMeshComponent* staticcom;
	UPROPERTY()
	class USplineMeshComponent* splinecom;

	
	UPROPERTY()
	TArray<class UDecalComponent*> decalArr;
	UPROPERTY()
	TArray<class UBoxComponent*> boxArr;
	UPROPERTY()
	TArray<class UStaticMeshComponent*> staticArr;
	UPROPERTY()
	TArray<class USplineMeshComponent*> splineArr;

	UPROPERTY(EditAnywhere)
	float decalSize_X;
	UPROPERTY(EditAnywhere)
	float decalSize_Y;
	UPROPERTY(EditAnywhere)
	float decalSize_Z;
	UPROPERTY(EditAnywhere)
	int32 SOrder;


	UPROPERTY(EditAnywhere)
	bool bCreateDecal;
	UPROPERTY(EditAnywhere)
	bool bCreateBox;
	UPROPERTY(EditAnywhere)
	bool bCreateMesh;
	UPROPERTY(EditAnywhere)
	bool bCreateSplineMesh;

	UPROPERTY(EditAnywhere)
	int32 inum;
};
