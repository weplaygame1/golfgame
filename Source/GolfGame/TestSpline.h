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

	FString GetMaterialName() { return material->GetName(); }
public:
	UPROPERTY(EditAnywhere)
	class USplineComponent* spline;
	UPROPERTY(EditAnywhere)
	class UMaterialInterface* material;
	UPROPERTY(EditAnywhere)
	class UDecalComponent* decal;
	UPROPERTY(EditAnywhere)
	class UBoxComponent* box;

	UPROPERTY(EditAnywhere)
	TArray<class UDecalComponent*> decalArray;
	UPROPERTY(EditAnywhere)
	TArray<class UBoxComponent*> boxArray;
	UPROPERTY(EditAnywhere)
	float decalSize_X;
	UPROPERTY(EditAnywhere)
	float decalSize_Y;
	UPROPERTY(EditAnywhere)
	float decalSize_Z;
	UPROPERTY(EditAnywhere)
	int32 SOrder;
};
