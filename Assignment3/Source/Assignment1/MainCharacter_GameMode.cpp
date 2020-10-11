// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter_GameMode.h"
#include "GameFramework/Actor.h"



AMainCharacter_GameMode::AMainCharacter_GameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMainCharacter_GameMode::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle UnusedHandle;
	//so GetWorldTimerManager is going to set the timer by using UnusedHandle as a parameter inside this class and ALevel1_GameMode is the function I want to execute after 2-5s and will repeat it forever
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &AMainCharacter_GameMode::SpawnPlayerHealth, FMath::RandRange(2, 5), true);
}

void AMainCharacter_GameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMainCharacter_GameMode::SpawnPlayerHealth()
{
	float RandX = FMath::RandRange(Spawn_X_Min, Spawn_X_Max);
	float RandY = FMath::RandRange(Spawn_Y_Min, Spawn_Y_Max);

	FVector SpawnPosition = FVector(RandX, RandY, Spawn_Z);
	FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);

	GetWorld()->SpawnActor(PlayerHealth, &SpawnPosition, &SpawnRotation);
}
