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
	FClothParticle() :
		Position(0.0f, 0.0f, 0.0f),
		PrevPosition(0.0f, 0.0f, 0.0f),
		Normal(0.0f, 0.0f, 0.0f),
		Velocity(0.0f, 0.0f, 0.0f),
		Force(0.0f, 0.0f, 0.0f),
		mass(1.0f),
		k(10.0f),
		bFree(false)
	{}
	FVector Position;
	FVector PrevPosition;
	FVector Normal;
	FVector Velocity;
	FVector Force;

	float mass = 1.0f;
	float k; // Spring Constant

	bool bFree;
	
	
};
UCLASS(hidecategories = (Object, LOD), editinlinenew, meta = (BlueprintSpawnableComponent), ClassGroup = Rendering, DisplayName = "MClothComponent")
class MCLOTH_API UMClothComponent : public UProceduralMeshComponent
{
	GENERATED_BODY()
	

public:
	UMClothComponent(const FObjectInitializer& ObjectInitializer);
	virtual ~UMClothComponent();

	virtual void OnRegister() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void UpdateProceduralMesh(bool init);
	void VerletIntegration(float InSubstepTime, const FVector& Gravity);
	void ComputeNormals();


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

protected:


};
