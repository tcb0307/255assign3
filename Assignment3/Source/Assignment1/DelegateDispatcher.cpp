// Fill out your copyright notice in the Description page of Project Settings.

#include "DelegateDispatcher.h"

// Sets default values
ADelegateDispatcher::ADelegateDispatcher(const FObjectInitializer& objectInitializer)
	:Super(objectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_collision = CreateDefaultSubobject<USphereComponent>(TEXT("RootCollision"));

	_collision->SetSphereRadius(250.f);
	_collision->SetHiddenInGame(false);
	_collision->OnComponentBeginOverlap.AddDynamic(this, &ADelegateDispatcher::OnDelegateOverLap);

	RootComponent = _collision;
}

// Called when the game starts or when spawned
void ADelegateDispatcher::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADelegateDispatcher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADelegateDispatcher::OnDelegateOverLap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Something hit me");

	AMainCharacter* overlappedCharacter = Cast<AMainCharacter>(OtherActor);

	if (overlappedCharacter->IsValidLowLevel())
	{
		overlappedCharacter->PlayerHealing.AddDynamic(this, &ADelegateDispatcher::WhenPlayerHeals);
	}
}

void ADelegateDispatcher::WhenPlayerHeals(FVector location)
{
	GEngine->AddOnScreenDebugMessage(-1, 19.f, FColor::Red, "Player is healing");
}