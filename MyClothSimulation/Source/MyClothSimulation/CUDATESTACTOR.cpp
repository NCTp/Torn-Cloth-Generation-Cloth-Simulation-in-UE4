// Fill out your copyright notice in the Description page of Project Settings.

#include "MoonClothComponent.h"
#include "CUDATESTACTOR.h"


FVector plus(10, 10, 10);
// Sets default values
ACUDATESTACTOR::ACUDATESTACTOR()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Cloth = CreateDefaultSubobject<UMoonClothComponent>(TEXT("MoonCloth"));
}

// Called when the game starts or when spawned
void ACUDATESTACTOR::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACUDATESTACTOR::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UWorld* world = GetWorld();
	DrawDebugLine(world, GetActorLocation(), GetActorLocation() + plus, FColor(255, 0, 0));
	plus += FVector(1, 1, 1);
}

