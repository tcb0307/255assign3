// Fill out your copyright notice in the Description page of Project Settings.

//TESTING - 255

#include "AIEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AAIEnemy::AAIEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>("collision component");
	CollisionComponent->SetBoxExtent(FVector(200, 200, 200));
	CollisionComponent->GetGenerateOverlapEvents();
	CollisionComponent->SetupAttachment(GetMesh());
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AAIEnemy::OnComponentBeginOverlap);

	pDead = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 600.0f, 0.0f); //Movement Rotation for the character

}

// Called when the game starts or when spawned
void AAIEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAIEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAIEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAIEnemy::DestroyEnemy()
{
	this->Destroy();
}

void AAIEnemy::Dead()
{
	pDead = true;

	GetMesh()->SetSimulatePhysics(true);

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &AAIEnemy::DestroyEnemy, 3.0f, false);
}

void AAIEnemy::OnComponentBeginOverlap(UPrimitiveComponent* OnComponentBeginOverlap, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AWeapon::StaticClass()))
	{
		//cast the other actor to bullet
		AWeapon* bullet = Cast<AWeapon>(OtherActor);

		bullet->EnemyDetection.AddDynamic(this, &AAIEnemy::AvoidProjectile);

		//destroy the enemy
		this->Dead();
		bullet->Destroy();
		UE_LOG(LogTemp, Warning, TEXT("Overlapped"));
	}

	if (OtherActor->ActorHasTag("Player"))
	{
		bool attackPlayer = true;
		PlayerUnderAttack.Broadcast(attackPlayer);
	}
}

void AAIEnemy::AvoidProjectile(bool projectilePresence)
{
	if (projectilePresence == true)
	{
		// Code where the enemy reacts to the incoming projectile
	}
}