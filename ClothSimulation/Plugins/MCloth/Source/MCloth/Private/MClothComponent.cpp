// Fill out your copyright notice in the Description page of Project Settings.


#include "MClothComponent.h"
#include "DrawDebugHelpers.h"
#include "Async/ParallelFor.h"

#define PrintString(String) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, String)
#define SPHERE_RADIUS 100


UMClothComponent::UMClothComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true; bTickInEditor = true;

	HorizontalVertexCount = 10;
	VerticalVertexCount = 10;

	HorizontalDistance = 2.0f;
	VerticalDistance = 2.0f;

	ClothGravityScale = 1.0f;
	SubstepTime = 0.02f;

	
}

UMClothComponent::~UMClothComponent()
{
	
}

void UMClothComponent::OnRegister()
{
	Super::OnRegister();
	/*
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%i"), TestParticle.posI));
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("ggg")));
	TestParticle.SetPosition(1, 2, 3, 7, 8);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%i"), TestParticle.posI));
	*/
	UpdateProceduralMesh(true);
}

void UMClothComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	PerformSubstep(FMath::Max(SubstepTime, 0.05f), FVector(0,0, GetWorld()->GetGravityZ() * ClothGravityScale));
	
	//TestParticle.AddForce(1.0f, 1.0f, 1.0f);
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%f, %f, %f"), TestParticle.ForceAcum.X, TestParticle.ForceAcum.Y, TestParticle.ForceAcum.Z));


	UpdateProceduralMesh(false);
}

void UMClothComponent::UpdateProceduralMesh(bool Init)
{
	TArray<FVector> Locations;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FColor> Colors;
	TArray<FProcMeshTangent> Tangents;
	const int32 VertexCount = HorizontalVertexCount * VerticalVertexCount;
	const int32 TriangleCount = (HorizontalVertexCount - 1) * (VerticalVertexCount - 1) * 6;
	Locations.AddUninitialized(VertexCount);
	Triangles.AddUninitialized(TriangleCount);
	Normals.AddUninitialized(VertexCount);
	UVs.AddUninitialized(VertexCount);
	Colors.AddUninitialized(VertexCount);
	Tangents.AddUninitialized(VertexCount);

	ClothParticles.Reset();
	ClothParticles.AddUninitialized(VertexCount);
	UWorld* world = GetWorld();
	if (Init)
	{
		// Initialize Position
		for (int i = 0; i < HorizontalVertexCount; ++i)
		{
			for (int j = 0; j < VerticalVertexCount; ++j)
			{
				float LocX = i * HorizontalDistance;
				float LocY = j * VerticalDistance;
				float LocZ = 0.0f;

				FVector InitLocation(LocX, LocY, LocZ); // First Location

				int32 Index = i * HorizontalVertexCount + j; // Index of each vertices

				ClothParticles[Index].Position = InitLocation;
				ClothParticles[Index].Normal = FVector::UpVector;
				ClothParticles[Index].bFree = !FixedParticleIndices.Contains(Index);

				DrawDebugSphere(world, ClothParticles[Index].Position, 1, 3, FColor(255, 0, 0, 1), false, 3.0f);

				Locations[Index] = InitLocation;
				Normals[Index] = FVector::UpVector;
				Colors[Index] = FColor(1.0f, 255.0f, 1.0f, 1.0f);
				if (Index > 0)
					DrawDebugLine(world, Locations[Index - 1], Locations[Index], FColor(255, 0, 0), false, 5.0f);

			}
		}
		// Initialize Triangles
		for (int32 Y = 0, Index = 0; Y < VerticalVertexCount - 1; ++Y)
		{
			for (int32 X = 0; X < HorizontalVertexCount - 1; ++X)
			{
				int A = Y * HorizontalVertexCount + X;
				int B = A + HorizontalVertexCount;
				int C = A + HorizontalVertexCount + 1;
				int D = A + 1;

				Triangles[Index++] = A;
				Triangles[Index++] = B;
				Triangles[Index++] = C;

				Triangles[Index++] = A;
				Triangles[Index++] = C;
				Triangles[Index++] = D;
			}
		}

		ClearMeshSection(0);
		CreateMeshSection(0, Locations, Triangles, Normals, UVs, Colors, Tangents, true);
	}
	else
	{
	/*
		for (int i = 0; i < HorizontalVertexCount; ++i)
		{
			for (int j = 0; j < VerticalVertexCount; ++j)
			{
				int32 Index = i * HorizontalVertexCount + j; // Index of each vertices

				DrawDebugSphere(world, ClothParticles[Index].Position, 1, 3, FColor(255, 0, 0, 1), false, 1.0f);
				Locations[Index] = ClothParticles[Index].Position;
				Normals[Index] = ClothParticles[Index].Normal;
				Colors[Index] = FColor(1.0f, 1.0f, 1.0f, 1.0f);

			}
		}
		UpdateMeshSection(0, Locations, Normals, UVs, Colors, Tangents);
		*/
		//ComputeNormals();
		
		for (int i = 0; i < HorizontalVertexCount; ++i)
		{
			for (int j = 0; j < VerticalVertexCount; ++j)
			{
				int32 Index = i * HorizontalVertexCount + j; // Index of each vertices
				if (j % 2 == 0)
					ClothParticles[Index].Position -= FVector(0.0f, 0.0f, 0.1f);
				Locations[Index] = ClothParticles[Index].Position;
				Normals[Index] = ClothParticles[Index].Normal;
				Colors[Index] = FColor(1.0f, 255.0f, 1.0f, 1.0f);
				DrawDebugSphere(world, ClothParticles[Index].Position, 1, 3, FColor(255, 0, 0, 1), false, 1.0f);
			}
		}
		
		UpdateMeshSection(0, Locations, Normals, UVs, Colors, Tangents);
	}
	
}

void UMClothComponent::ComputeNormals()
{
	ParallelFor(ClothParticles.Num(), [&](int32 Index)
		{
			const int32 X = Index % HorizontalVertexCount;
			const int32 Y = Index / HorizontalVertexCount;
			const FVector ParticlePos = ClothParticles[Index].Position;
			FVector DirA, DirB;
			int32 IndexA, IndexB;

			IndexA = X > 0 ? X - 1 : 1; // Favor left side
			IndexB = Y > 0 ? Y - 1 : 1; // Favor up side

			FVector PosA = ClothParticles[Y * HorizontalVertexCount + IndexA].Position; // Left side position
			FVector PosB = ClothParticles[IndexB * HorizontalVertexCount + X].Position; // Up side position

			DirA = PosA - ParticlePos;
			DirB = PosB - ParticlePos;

			ClothParticles[Index].Normal = (DirB ^ DirA).GetSafeNormal();

			if ((X == 0) ^ (Y == 0))
			{
				ClothParticles[Index].Normal *= -1;
			}
		});
}

void UMClothComponent::PerformSubstep(float InSubstepTime, const FVector& Gravity)
{
	//VerletIntegration(InSubstepTime, Gravity);
	//ComputeNormals();
}
