// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HoldableActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent),Blueprintable )
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

	//to prevent actor from picked up during some sections
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bCanBePickedUp = true;

	UFUNCTION(BlueprintCallable)
	virtual void BeDropped();

	UFUNCTION(BlueprintCallable)
    virtual void BeThrown(FVector impulse);

	//to use with custom components(line apex's dstruction)
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void ChildOnBeDropped();
	
	void ChildOnBeDropped_Implementation(){}
	
	//to use with custom components(line apex's dstruction)
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
    void ChildOnBePickedup();
	
	void ChildOnBePickedup_Implementation(){}

	//to use with custom components(line apex's dstruction)
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
    void ChildOnThrown(FVector impulse);
	
	void ChildOnThrown_Implementation(FVector impulse){}
	

	UFUNCTION(BlueprintCallable)
    virtual void BePickedUp();
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
