// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawn.h"

// Sets default values
AEnemySpawn::AEnemySpawn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//initialise skeletal mesh
	body = CreateDefaultSubobject<USkeletalMeshComponent>("body");
	this->RootComponent = body;

	//initialise CollisionComponent
	CollisionComponent = CreateDefaultSubobject<UBoxComponent>("collision component");
	CollisionComponent->SetBoxExtent(FVector(200, 200, 200));
	CollisionComponent->GetGenerateOverlapEvents();
	CollisionComponent->SetupAttachment(body);
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemySpawn::OnComponentBeginOverlap);

	pDead = false;

}

// Called when the game starts or when spawned
void AEnemySpawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemySpawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemySpawn::DestroyEnemy()
{
	this->Destroy(); //Destroy the enemy mesh
}

void AEnemySpawn::Dead()
{
	pDead = true;

	body->SetSimulatePhysics(true); //Enemy is down when hitting bullet

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &AEnemySpawn::DestroyEnemy, 3.0f, false); //after 3s, the enemy will be destroy
}

void AEnemySpawn::OnComponentBeginOverlap(UPrimitiveComponent* OnComponentBeginOverlap, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor->IsA(AWeapon::StaticClass()))
	{

		//cast the other actor to bullet
		AWeapon* bullet = Cast<AWeapon>(OtherActor);

		//destroy the enemy
		this->Dead();
		bullet->Destroy();
		UE_LOG(LogTemp, Warning, TEXT("Hit Enemy!"));
	}
}


