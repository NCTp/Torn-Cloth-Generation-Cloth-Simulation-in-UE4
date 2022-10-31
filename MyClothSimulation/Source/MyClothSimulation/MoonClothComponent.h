// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "MoonClothComponent.generated.h"

/**
 * 
 */
UCLASS()
class MYCLOTHSIMULATION_API UMoonClothComponent : public UProceduralMeshComponent
{
	GENERATED_BODY()
public:

	UMoonClothComponent(const FObjectInitializer& ObjectInitializer);

	void OnRegister() override;

	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
