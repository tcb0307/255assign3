// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Weapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDodgeProjectile, bool, bulletNearby);

class UStaticMeshComponent;

UCLASS()
class ASSIGNMENT1_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* body;

	UPROPERTY(EditAnywhere, BluePrintReadOnly)
		USphereComponent* CollisionComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Projectile movement component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(BlueprintAssignable)
		FDodgeProjectile EnemyDetection;

	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OnComponentBeginOverlap, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Function that initializes the projectile's velocity in the shoot derection
	void FireInDirection(const FVector& ShootDirection);
};