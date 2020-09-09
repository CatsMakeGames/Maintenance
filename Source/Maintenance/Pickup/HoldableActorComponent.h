// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HoldableActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MAINTENANCE_API UHoldableActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHoldableActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
private:
	FVector _impulse;

	FTimerHandle ImpulseApplyTimerHandle;

	void ApplyImpulseToComponents();
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=PhysicsComponents)
	TArray<UStaticMeshComponent*>StaticMeshPhysicsComponents;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=PhysicsComponents)
	TArray<USkeletalMeshComponent*>SkeletalMeshPhysicsComponents;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=PhysicsComponents)
	FName PhysComponentsTag = TEXT("PhysComp");

	UFUNCTION(BlueprintCallable)
	virtual void BeDropped();

	UFUNCTION(BlueprintCallable)
    virtual void BeThrown(FVector impulse);

	UFUNCTION(BlueprintCallable)
    virtual void BePickedUp();
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
