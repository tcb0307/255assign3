// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Weapon.h"
#include "EnemySpawn.generated.h"

UCLASS()
class ASSIGNMENT1_API AEnemySpawn : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawn();

	bool pDead; //determine if the character is that or not

	UPROPERTY(EditAnywhere, BluePrintReadwrite)
		UBoxComponent* CollisionComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* body;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Dead();

	void DestroyEnemy();

	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OnComponentBeginOverlap, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
