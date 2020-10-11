// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "GameFramework/Actor.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetActorHiddenInGame(true);
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle UnusedHandle;
	//so GetWorldTimerManager is going to set the timer by using UnusedHandle as a parameter inside this class and ALevel1_GameMode is the function I want to execute after 2-5s and will repeat it forever
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &AEnemySpawner::SpawnEnemy, FMath::RandRange(4, 7), true);
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemySpawner::SpawnEnemy() //setting random location spawning enemies
{
	float RandX = FMath::RandRange(Spawn_X_Min, Spawn_X_Max);
	float RandY = FMath::RandRange(Spawn_Y_Min, Spawn_Y_Max);

	FVector SpawnPosition = FVector(RandX, RandY, Spawn_Z);
	FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);

	GetWorld()->SpawnActor(BP_EnemySpawn, &SpawnPosition, &SpawnRotation);
}
