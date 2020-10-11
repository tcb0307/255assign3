// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectChanging.generated.h"

UCLASS()
class ASSIGNMENT1_API AObjectChanging : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObjectChanging();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere)
		class UMaterial* OffMaterial;

	UPROPERTY(EditAnywhere)
		class UMaterial* OnMaterial;

	UPROPERTY()
		class UBoxComponent* BoxComponent;

	UFUNCTION()
		void OnBeginOverLap(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	bool ChangingMaterial;
};
