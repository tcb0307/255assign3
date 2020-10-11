// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectChanging.h"
#include "DrawDebugHelpers.h"
#include "Components/BoxComponent.h"

// Sets default values
AObjectChanging::AObjectChanging()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	RootComponent = MeshComponent;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	BoxComponent->InitBoxExtent(FVector(100, 100, 100));
	BoxComponent->SetCollisionProfileName("Trigger");
	BoxComponent->SetupAttachment(RootComponent);

	OnMaterial = CreateDefaultSubobject<UMaterial>(TEXT("On Material"));
	OffMaterial = CreateDefaultSubobject<UMaterial>(TEXT("Off Material"));

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AObjectChanging::OnBeginOverLap);

	ChangingMaterial = false;
}

// Called when the game starts or when spawned
void AObjectChanging::BeginPlay()
{
	Super::BeginPlay();
	
	DrawDebugBox(GetWorld(), GetActorLocation(), FVector(100, 100, 100), FColor::White, true, -1, 0, 10);

	MeshComponent->SetMaterial(0, OffMaterial);
}

// Called every frame
void AObjectChanging::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AObjectChanging::OnBeginOverLap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		ChangingMaterial = !ChangingMaterial; //if true will become false, and else false will become true
		if (ChangingMaterial)
		{
			MeshComponent->SetMaterial(0, OnMaterial);
			UE_LOG(LogTemp, Warning, TEXT("Changing into Green Material"));
		}
		else
		{
			MeshComponent->SetMaterial(0, OffMaterial);
			UE_LOG(LogTemp, Warning, TEXT("Changing into Red Material"));
		}
	}

}

