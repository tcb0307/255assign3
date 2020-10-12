// Fill out your copyright notice in the Description page of Project Settings.


#include "ATeleporter.h"

AATeleporter::AATeleporter()
{
	OnActorBeginOverlap.AddDynamic(this, &AATeleporter::EnterTeleporter);
	OnActorEndOverlap.AddDynamic(this, &AATeleporter::ExitTeleporter);
	teleporting = false;
}

void AATeleporter::BeginPlay()
{
	Super::BeginPlay();
}

void AATeleporter::EnterTeleporter(class AActor* overlappedActor, class AActor* otherActor)
{
	if (otherActor && otherActor != this)
	{
		if (otherTele)
		{
			AMainCharacter* character = Cast<AMainCharacter>(otherActor);
			if (character && !otherTele->teleporting)
			{
				teleporting = true;
				character->SetActorRotation(otherTele->GetActorRotation());
				character->GetController()->SetControlRotation(character->GetActorRotation()); 
				character->SetActorLocation(otherTele->GetActorLocation());
			}
		}
	}
}

void AATeleporter::ExitTeleporter(class AActor* overlappedActor, class AActor* otherActor)
{
	if (otherActor && otherActor != this)
	{
		if (otherTele && !teleporting)
		{
			otherTele->teleporting = false;
		}
	}
}
