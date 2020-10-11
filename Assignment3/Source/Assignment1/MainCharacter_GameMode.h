// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MainCharacter_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENT1_API AMainCharacter_GameMode : public AGameMode
{	
	GENERATED_BODY()

	AMainCharacter_GameMode();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<APawn> PlayerHealth;

	float Spawn_Z = 500.0f;


	//Getting random location axis
	UPROPERTY(EditAnywhere)
	float Spawn_X_Min;

	UPROPERTY(EditAnywhere)
	float Spawn_X_Max;

	UPROPERTY(EditAnywhere)
	float Spawn_Y_Min;

	UPROPERTY(EditAnywhere)
	float Spawn_Y_Max;

	void SpawnPlayerHealth();
};
