// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "MClothComponent.generated.h"

/**
 * 
 */
struct FClothParticle
{

	float mass;
	int posI, posJ;
	float dim; // size of particles
	float boxRadius; //box to avoid internal collision
	int numberOfAdjTriangles;
	int adjTriangles[6];
	int dampForce;

	bool bIsFixed;

	FVector Position;
	FVector PrevPosition;
	FVector Normal;
	FVector Velocity;
	FVector ForceAcum;

	FClothParticle() :
		Position(0.0f, 0.0f, 0.0f),
		PrevPosition(0.0f, 0.0f, 0.0f),
		Normal(0.0f, 0.0f, 0.0f),
		Velocity(0.0f, 0.0f, 0.0f),
		ForceAcum(0.0f, 0.0f, 0.0f),
		mass(1.0f),
		posI(0),
		posJ(0),
		numberOfAdjTriangles(0),
		dim(5.0f),
		dampForce(5.0f),
		bIsFixed(false)
	{}

	virtual ~FClothParticle()
	{

	}

	void SetPosition(FVector newPos)
	{
		Position = newPos;
	}

	void ClearForces()
	{
		ForceAcum = FVector(0.0f, 0.0f, 0.0f);
	}

	void AddForce(FVector Force)
	{
		ForceAcum += Force;
	}

	FVector VerletIntegration()
	{
		FVector NextPosition = FVector();

		NextPosition.X = (2 * this->Position.X) - this->PrevPosition.X + this->ForceAcum.X * 0.02f * 0.02f;
		NextPosition.Y = (2 * this->Position.Y) - this->PrevPosition.Y + this->ForceAcum.Y * 0.02f * 0.02f;
		NextPosition.Z = (2 * this->Position.Z) - this->PrevPosition.Z + this->ForceAcum.Z * 0.02f * 0.02f;

		return NextPosition;
	}

	void Update()
	{
		Velocity = ForceAcum * 0.1f;

		FVector tempX = Position;

		FVector NextPos = VerletIntegration();

		Position = NextPos;

		PrevPosition = tempX;
	}
	
};

struct FClothTriangle
{
public:

	FClothParticle* particles[2];
	FVector* Normal;

	FClothTriangle() :
		Normal(new FVector)
	{}
	virtual ~FClothTriangle()
	{

	}

	void SetTriangle(FClothParticle* p1, FClothParticle* p2, FClothParticle* p3)
	{
		particles[0] = p1;
		particles[1] = p2;
		particles[2] = p3;
	}
	void CalculateNormal()
	{
		FVector v1 = FVector(0.0f, 0.0f, 0.0f);
		FVector v2 = FVector(0.0f, 0.0f, 0.0f);

		v1 = particles[0]->Position - particles[1]->Position;
		v2 = particles[0]->Position - particles[2]->Position;

		Normal->X = v1.Y * v2.Z - v1.Z * v2.Y;
		Normal->Y = v1.Z * v2.X - v1.X * v2.Z;
		Normal->Z = v1.X * v2.Y - v1.Y * v2.X;
	}

};

struct FClothSpring
{
public:
	FClothParticle* P1;
	FClothParticle* P2;
	float ks;
	float kd;
	float normalLength;

	FClothSpring(FClothParticle* p1, FClothParticle* p2, float NormalLength) :
		P1(p1),
		P2(p2),
		ks(20),
		kd(0.2f),
		normalLength(NormalLength)
	{}

	virtual ~FClothSpring()
	{
	}

	void Update(float KS, float KD)
	{
		this->ks = KS;
		this->kd = KD;
		ApplySpringForce();
	}
	void ApplySpringForce()
	{
		FVector ePos = P2->Position - P1->Position;

		//float length = FMath::Sqrt(FMath::Pow(Epos.X, 2) + FMath::Pow(Epos.Y, 2) + FMath::Pow(Epos.Z, 2));

		ePos.Normalize();

		float v1 = ePos.X * P1->Velocity.X * ePos.Y * P1->Velocity.Y + ePos.Z * P1->Velocity.Z;
		float v2 = ePos.X * P2->Velocity.X * ePos.Y * P2->Velocity.Y + ePos.Z * P2->Velocity.Z;

		float fSD = -ks * (normalLength - this->GetLength()) - kd * (v1 - v2);

		FVector fPos = ePos * fSD;

		P1->AddForce(fPos);
		P2->AddForce(fPos);

	}
	float GetLength()
	{
		FVector dist = FVector(0.0f, 0.0f, 0.0f);
		dist = P2->Position - P1->Position;
		float length = FMath::Sqrt((dist.X * dist.X + dist.Y * dist.Y + dist.Z * dist.Z));
		return length;
	}
};
UCLASS(hidecategories = (Object, LOD), editinlinenew, meta = (BlueprintSpawnableComponent), ClassGroup = Rendering, DisplayName = "MClothComponent")
class MCLOTH_API UMClothComponent : public UProceduralMeshComponent
{
	GENERATED_BODY()

	typedef TArray<FClothParticle> ClothParticles_I;

public:

	UMClothComponent(const FObjectInitializer& ObjectInitializer);
	virtual ~UMClothComponent();

	virtual void OnRegister() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void UpdateProceduralMesh(bool init);
	void VerletIntegration(float InSubstepTime, const FVector& Gravity);
	void ComputeNormals();
	void SolveConstraints();
	void PerformSubstep(float InSubstepTime, const FVector& Gravity);

	//////////////// Wire
	void AddParticle(FClothParticle Particle, int i, int j);
	void GenerateTriangles(int resolution);
	void Update(float ks, float kd);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 2), Category = "Cloth Geometry")
		int32 HorizontalVertexCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 2), Category = "Cloth Geometry")
		int32 VerticalVertexCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0), Category = "Cloth Geometry")
		float HorizontalDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0), Category = "Cloth Geometry")
		float VerticalDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0.0, ClampMax = 1.0), Category = "Cloth Physics")
		float ClothGravityScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0.005, ClampMax = 0.1), Category = "Cloth Physics")
		float SubstepTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 1), Category = "Cloth Physics")
		int32 SolverIterationCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloth Physics")
		TArray<uint8> FixedParticleIndices;

protected:
	TArray<FClothParticle> ClothParticles;

	TArray<ClothParticles_I> ClothParticles_J;
	int numberOfParticles;

	TArray<FClothTriangle> ClothTriangles;
	int numberOfTriangles;

	TArray<FClothSpring> ClothSprings;
	int numberOfSprings;

	

protected:


};
