// Fill out your copyright notice in the Description page of Project Settings.


#include "MoonClothComponent.h"

UMoonClothComponent::UMoonClothComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true; 
	bTickInEditor = true;
}

void UMoonClothComponent::OnRegister()
{
	Super::OnRegister();
}

void UMoonClothComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}
