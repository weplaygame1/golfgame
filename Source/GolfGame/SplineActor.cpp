// Fill out your copyright notice in the Description page of Project Settings.


#include "SplineActor.h"

// Sets default values
ASplineActor::ASplineActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	spline = CreateDefaultSubobject<USplineComponent>(TEXT("SPLINE"));
	spline->SetClosedLoop(true);

	material = CreateDefaultSubobject<UMaterialInterface>(TEXT("MATERIAL"));

	TriangleSize = 50;
	Padding = 3;
}

// Called when the game starts or when spawned
void ASplineActor::BeginPlay()
{
	Super::BeginPlay();
	//BuildMeshFromOutline();
}

// Called every frame
void ASplineActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASplineActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	BuildMeshFromOutline();
	
}

void ASplineActor::MakePointGrid()
{
	//TriangleSize = FMath::Clamp(TriangleSize, 2.f, 30.f);
	Vertices = TArray<FVector>();

	FVector Origin;
	FVector BoxExtent;
	float SphereRadius;
	UKismetSystemLibrary::GetComponentBounds(spline, Origin, BoxExtent, SphereRadius);

	// ������ �����ؼ� +1 ��� *2�� ����
	//NumX = UKismetMathLibrary::FCeil((BoxExtent.X / TriangleSize) + 1);
	int32 NumX = UKismetMathLibrary::FCeil(BoxExtent.X / TriangleSize * 2);
	int32 NumY = UKismetMathLibrary::FCeil((BoxExtent.Y / (TriangleSize / 2) * UKismetMathLibrary::DegTan(60)));

	TArray<int32> PointIndex;

	for (int32 indexY = -NumY; indexY <= NumY; indexY++)
	{
		for (int32 indexX = -NumX; indexX <= NumX; indexX++)
		{
			FVector CurrentLoc;

			//��������Ʈ�� �����ִ� ������ �ٸ�
			//CurrentLoc.X = Origin.X + (TriangleSize / 2 * indexX) + (FMath::Abs(indexY + NumY) % 2 );
			//CurrentLoc.Y = Origin.Y + ((TriangleSize / 2) * (UKismetMathLibrary::DegTan(60)) * indexY);

			CurrentLoc.X = Origin.X + (TriangleSize * indexX) + ((TriangleSize / 2)*(FMath::Abs(indexY + NumY) % 2));
			CurrentLoc.Y = Origin.Y + (((TriangleSize / 2) * UKismetMathLibrary::DegTan(60)) * indexY);

			CurrentLoc.Z = Origin.Z;

			FVector CurrentEdgeLoc = spline->FindLocationClosestToWorldLocation(CurrentLoc, ESplineCoordinateSpace::World);

			FVector FVtemp1 = CurrentEdgeLoc - CurrentLoc;
			FVector FVtemp2 = spline->FindDirectionClosestToWorldLocation(CurrentLoc, ESplineCoordinateSpace::World);
			FVector FVtemp3 = UKismetMathLibrary::Cross_VectorVector(FVtemp2, FVector(0.0f, 0.0f, 1.0f));
			//FVector FVtemp3 = UKismetMathLibrary::Cross_VectorVector(FVector(0.0f, 0.0f, 1.0f),FVtemp2);

			float fdot = UKismetMathLibrary::Dot_VectorVector(FVtemp1, FVtemp3);

			bool inside = fdot > 0.0f;
			bool edge = FVtemp1.Size() < TriangleSize;

			if (inside)
			{
				Vertices.Add(CurrentLoc);
				PointIndex.Add(0);
			}
			else if (edge)
			{
				Vertices.Add(CurrentEdgeLoc);
				PointIndex.Add(1);
			}
			else
			{
				Vertices.Add(CurrentLoc);
				PointIndex.Add(-1);
			}
		}
	}
	GridX = NumX * 2;
	GridPoints = PointIndex;
}

void ASplineActor::BuildMeshFromOutline()
{
	MakePointGrid();

	TArray<int32> TrianglesL;

	for (int index = GridX + 1; index <= GridPoints.Num() - 2; index++)
	{
		// ���� ���İ� ��������Ʈ�� ���� �ణ �ٸ����� �ﰢ�� ũ�Ⱑ ������ ������ ���̾���

		// ���ﰢ��
		// ����
		//int32 point1 = (((index / (GridX + 1) % 2)* (-1)) + 1) + index;
		// ��������Ʈ
		int32 point1 = ((((index / (GridX + 1)) % 2) * (-1)) + 1) + index;

		int32 point2 = ((index - (GridX + 1)) - (-1));
		int32 point3 = (index - (GridX + 1));
		int32 point4 = GridPoints[point1] + GridPoints[point2] + GridPoints[point3];

		if (GridPoints[point1] != -1 && GridPoints[point2] != -1 && GridPoints[point3] != -1 && point4 != 3)
		{
			//TrianglesL.Add((((index / (GridX + 1) % 2)* (-1)) + 1) + index);
			TrianglesL.Add(((((index / (GridX + 1)) % 2) * (-1)) + 1) + index);

			TrianglesL.Add(((index - (GridX + 1)) - (-1)));
			TrianglesL.Add((index - (GridX + 1)));
		}

		// �ﰢ��
		point1 = index;
		point2 = index + 1;

		// ����
		//point3 = index - ((GridX + 1) - ((index / (GridX + 1))) % 2);
		// ��������Ʈ
		point3 = index - ((GridX + 1) - ((index / (GridX + 1)) % 2));

		point4 = GridPoints[point1] + GridPoints[point2] + GridPoints[point3];

		if (GridPoints[point1] != -1 && GridPoints[point2] != -1 && GridPoints[point3] != -1 && point4 != 3)
		{
			TrianglesL.Add(index);
			TrianglesL.Add(index + 1);

			//TrianglesL.Add(index - ((GridX + 1) - ((index / (GridX + 1))) % 2));
			TrianglesL.Add(index - ((GridX + 1) - ((index / (GridX + 1)) % 2)));
		}
	}
	Triangles = TrianglesL;

	//NormalizePointGridforUV();

	// Add Procedural Mesh Component
	procedural = NewObject<UProceduralMeshComponent>(this, TEXT("PROCEDURAL"));
	procedural->RegisterComponent();
	this->AddInstanceComponent(procedural);

	FOccluderVertexArray Normals;
	TArray<FVector2D> UV0;
	TArray<FColor> VertexColors;
	TArray<FProcMeshTangent> Tangents;

	//procedural->CreateMeshSection(0, Vertices, TrianglesL, Normals, UV, VertexColor, Tangents, true);
	procedural->CreateMeshSection(0, Vertices, TrianglesL, Normals, UV0, VertexColors, Tangents, true);
	procedural->SetMaterial(0, material);
}

void ASplineActor::NormalizePointGridforUV()
{
	//Padding = FMath::Clamp(Padding, 0.f, 0.25f);

	FVector Origin;
	FVector BoxExtent;
	float SphereRadius;
	UKismetSystemLibrary::GetComponentBounds(spline, Origin, BoxExtent, SphereRadius);

	bool condition = BoxExtent.X > BoxExtent.Y;

	if (condition)
	{
		Scale = Padding / (BoxExtent.X * 2);
	}
	else
	{
		Scale = Padding / (BoxExtent.Y * 2);
	}

	OriginScaled = Origin * Scale;

	for (int index = 0; index < Vertices.Num(); index++)
	{
		FVector temp = Vertices[index];

		float fx = (temp.X * Scale) + (OriginScaled * (-1)).X + 0.5;
		float fy = (temp.Y * Scale) + (OriginScaled * (-1)).Y + 0.5;

		UV.Add(FVector2D(fx, fy));

		FVector temp1 = spline->FindLocationClosestToWorldLocation(temp, ESplineCoordinateSpace::World) - temp;
		VertexColor.Add((FColor)(temp1.Size() * Scale));
	}
}

void ASplineActor::checktest()
{
	// ���� ��ġ
	FVector CurrentLoc = FVector(994, 1000, 0);

	FVector CurrentEdgeLoc = spline->FindLocationClosestToWorldLocation(CurrentLoc, ESplineCoordinateSpace::World);

	FVector FVtemp1 = CurrentEdgeLoc - CurrentLoc;
	FVector FVtemp2 = spline->FindDirectionClosestToWorldLocation(CurrentLoc, ESplineCoordinateSpace::World);
	FVector FVtemp3 = UKismetMathLibrary::Cross_VectorVector(FVtemp2, FVector(0.0f, 0.0f, 1.0f));

	float fdot = UKismetMathLibrary::Dot_VectorVector(FVtemp1, FVtemp3);

	bool bin = fdot >= 0.0f;
	// bin �� true�̸� ���ö��� �ȿ� �ִ�.
}
