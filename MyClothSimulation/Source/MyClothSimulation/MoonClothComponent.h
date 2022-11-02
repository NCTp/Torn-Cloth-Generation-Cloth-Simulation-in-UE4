// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "MoonClothComponent.generated.h"

/**
 * 
 */
class  HashGrid;
struct FVerletClothConstraint;

struct FVerletClothParticle
{
	FVerletClothParticle()
		: Position(0, 0, 0)
		, PrevPosition(0, 0, 0)
		, Force(0, 0, 0)
		, Col(255, 255, 255, 255)
		, ID(-1)
		, C_idx(-1)
		, state(1)
		, conCount(0)
	{}
	FVector Position;
	FVector PrevPosition;
	FVector Force;
	FColor Col;

	int32 ID;
	uint32 C_idx;
	int8 state, conCount;
};

UCLASS()
class MYCLOTHSIMULATION_API UMoonClothComponent : public UProceduralMeshComponent
{
	GENERATED_BODY()
public:

	struct
	{
		// SM Deserialized
		TArray<FVector>            Pos;
		TArray<FColor>             Col;
		TArray<FVector>            Normal;
		TArray<FProcMeshTangent>   Tang;
		TArray<FVector2D>          UV;
		TArray<int32>              Ind;
		TArray<FIntVector>         Tris;

		// Vert Shared Tris
		TArray<int32>* vtris;

		// SM Buffer Ptrs
		FPositionVertexBuffer* vb;
		FStaticMeshVertexBuffer* smvb;
		FColorVertexBuffer* cvb;
		FRawStaticIndexBuffer* ib;

		int32 vert_count, ind_count, adj_count, tri_count;
		bool has_uv, has_col;
	} m_smData;

	// Mesh Properties
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Cloth")
	UStaticMeshComponent *m_sm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloth Simulation")
	bool bShowStaticMesh;
	
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Cloth Simulation")
	void StaticToProcedural();

	UMoonClothComponent(const FObjectInitializer& ObjectInitializer);

	void OnRegister() override;

	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;



private:

	// --- Cloth Data ---
	TArray<FVerletClothParticle> Particles;
	TArray<FVerletClothConstraint> Constraints;
	TArray<FVector> Normals;
	TArray<FVerletClothParticle*> VolSamplePts;
	float restVolume, curVolume, deltaVolume;
	int32 particleCount;

	// --- State Flags --- 
	bool clothStateExists, world_collided;


};
struct FVerletClothConstraint
{
	FVerletClothConstraint(FVerletClothParticle& Pt_0, FVerletClothParticle& Pt_1, UMoonClothComponent* cloth)
		: Pt0(Pt_0), Pt1(Pt_1), Cloth(cloth)
	{
		// Get Particles Corresponding Vertices Orginal Postions and Rest Length.
		orgP0 = Cloth->m_smData.Pos[Pt_0.ID]; orgP1 = Cloth->m_smData.Pos[Pt_1.ID];
		restLength = (orgP1 - orgP0).Size();
		// ID to Identify Particle/Vertex ID Pair of Constraint. 
		conID = Pt_0.ID * Pt_1.ID;
	}
	FVerletClothConstraint() = delete;

	FVerletClothParticle& Pt0, & Pt1;
	FVector orgP0, orgP1;
	float restLength;
	int32 conID;
	UMoonClothComponent* Cloth;
};
